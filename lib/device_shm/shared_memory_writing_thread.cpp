/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "shared_memory_writing_thread.hpp"

#include "dxrt/common.h"
#include "../device_pool/device_dispatcher.h"

#include <utility>

namespace dxrt {

void SharedMemoryWritingThread::RegisterDevice(DeviceDispatcher* deviceCore)
{
    if (deviceCore == nullptr)
    {
        return;
    }

    RegisterDeviceCallbacks(
        deviceCore->GetDeviceId(),
        deviceCore,
        [deviceCore](std::array<double, 3>* utilization) -> bool {
            if (utilization == nullptr)
            {
                return false;
            }

            deviceCore->OnTimerTick();
            for (int i = 0; i < 3; ++i)
            {
                (*utilization)[i] = deviceCore->GetUsage(i);
            }
            return true;
        },
        [deviceCore](
            std::array<uint32_t, 3>* voltage,
            std::array<uint32_t, 3>* clock,
            std::array<uint32_t, 3>* temperature) -> bool {
            if (voltage == nullptr || clock == nullptr || temperature == nullptr)
            {
                return false;
            }
            const auto status = deviceCore->GetStatus();
            *voltage = {status.voltage[0], status.voltage[1], status.voltage[2]};
            *clock = {status.clock[0], status.clock[1], status.clock[2]};
            *temperature = {status.temperature[0], status.temperature[1], status.temperature[2]};
            return true;
        },
        [deviceCore](uint64_t* total, uint64_t* used, uint64_t* free) -> bool {
            return deviceCore->GetMemoryStats(total, used, free);
        });
}

void SharedMemoryWritingThread::RegisterDeviceCallbacks(
    int deviceId,
    DeviceDispatcher* dispatcher,
    UtilizationProvider utilizationProvider,
    CoreStatsProvider coreStatsProvider,
    MemoryStatsProvider memoryProvider)
{
    if (!utilizationProvider || !coreStatsProvider)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    _devices.push_back(DeviceEntry{
        deviceId,
        dispatcher,
        std::move(utilizationProvider),
        std::move(coreStatsProvider),
        std::move(memoryProvider)});
}

bool SharedMemoryWritingThread::Start()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_running.load())
    {
        return false;  // already running
    }
    _running.store(true);
    _shmWriter.Initialize();

    if (_shmWriter.IsInitialized())
    {
        for (const auto& entry : _devices)
        {
            if (entry.deviceId < 0 || entry.dispatcher == nullptr)
            {
                continue;
            }

            dxrt_device_info_t spec = {};
            dxrt_dev_info_t devInfo = {};
            if (entry.dispatcher->FillDeviceSpec(&spec, &devInfo))
            {
                _shmWriter.UpdateDeviceSpec(entry.deviceId, spec, devInfo);
            }
        }
    }

    _thread = std::thread(&SharedMemoryWritingThread::threadFunc, this);
    return true;
}
void SharedMemoryWritingThread::Stop()
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (!_running.load())
        {
            return;  // not running
        }
        _running.store(false);
    }
    if (_thread.joinable())
    {
        _thread.join();
    }
    else
    {
        LOG_DXRT_ERR("SharedMemoryWritingThread::Stop called but thread is not joinable\n");
    }
}
void SharedMemoryWritingThread::threadFunc()
{
    while (_running.load())
    {
        std::vector<DeviceEntry> devicesSnapshot;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            devicesSnapshot = _devices;
        }

        for (size_t i = 0; i < devicesSnapshot.size(); i++)
        {
            const auto& entry = devicesSnapshot[i];

            std::array<double, 3> utilization = {0.0, 0.0, 0.0};
            if (!entry.utilizationProvider(&utilization))
            {
                continue;
            }

            const int device_id = entry.deviceId;
            if (device_id < 0)
            {
                continue;
            }

            if (_shmWriter.IsInitialized())
            {
                _shmWriter.UpdateDeviceUtilization(device_id, utilization);

                // Update memory information
                if (entry.memoryProvider)
                {
                    uint64_t total = 0;
                    uint64_t used = 0;
                    uint64_t free = 0;
                    if (entry.memoryProvider(&total, &used, &free))
                    {
                        _shmWriter.UpdateDeviceMemory(device_id, total, used, free);
                    }
                }

                // Update core stats (voltage, clock, temperature)
                std::array<uint32_t, 3> voltage_arr = {0, 0, 0};
                std::array<uint32_t, 3> clock_arr = {0, 0, 0};
                std::array<uint32_t, 3> temp_arr = {0, 0, 0};
                if (entry.coreStatsProvider(&voltage_arr, &clock_arr, &temp_arr))
                {
                    _shmWriter.UpdateDeviceCoreStats(device_id, voltage_arr, clock_arr, temp_arr);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


}  // namespace dxrt
