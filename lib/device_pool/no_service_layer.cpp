/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/service_abstract_layer.h"
#include "shared_memory_writer.h"
#include "../dynamic_ipc/protocol/memory_type.hpp"
#include <cerrno>
#include <cstring>
#include <sstream>
#include "inference_context.h"

namespace dxrt
{
namespace {

std::string ToHexU64(uint64_t value)
{
    std::ostringstream oss;
    oss << std::hex << value;
    return oss.str();
}

}  // namespace

extern uint8_t DEBUG_DATA;  // NOSONAR
// NoServiceLayer ------------------------------------------------

NoServiceLayer::NoServiceLayer()
{
    // Initialize shared memory writer
    _shmWriter = std::make_unique<SharedMemoryWriter>();
    if (!_shmWriter->Initialize()) {
        LOG_DXRT_DBG << "Failed to initialize shared memory writer for monitoring" << std::endl;
    }

    // Start monitoring thread
    _usageMonitorRunning.store(true, std::memory_order_release);
    _usageMonitorThread = std::thread(&NoServiceLayer::UsageMonitorThread, this);
}

NoServiceLayer::~NoServiceLayer()
{
    // Stop monitoring thread
    _usageMonitorRunning.store(false, std::memory_order_release);
    if (_usageMonitorThread.joinable())
    {
        _usageMonitorThread.join();
    }

    // Cleanup shared memory
    if (_shmWriter) {
        _shmWriter->Cleanup();
    }
}

#ifdef __linux__
    constexpr static int HandleInferenceAcc_BUSY_VALUE = -EBUSY;  // write done, but failed to enqueue
#elif _WIN32
    constexpr static int HandleInferenceAcc_BUSY_VALUE = ERROR_BUSY;
#endif

void NoServiceLayer::HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut)
{
    std::ignore = responseOut;
    DeviceCore *core = _ptr[deviceId];
    dxrt_request_acc_t acc_cp = acc;
    int ret = -1;
    do
    {
        ret = core->Process(DXRT_CMD_NPU_RUN_REQ, &acc_cp);

        if (ret == HandleInferenceAcc_BUSY_VALUE)
        {
            LOG_DXRT_DBG << "Device " << deviceId << " is busy. Retrying HandleInferenceAcc..." << std::endl;
            acc_cp.input.data = 0;
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        else if (ret != 0)
        {
            LOG_DXRT_ERR("HandleInferenceAcc for device " << deviceId << " returned " << ret);
        }
        // if stoppes, return required;
    } while (ret != 0);
}

void NoServiceLayer::RegisterDeviceCore(DeviceCore* core)
{
    int id = core->id();
    _ptr[id] = core;
    dxrt_device_info_t info = core->info();
    _mems.emplace(id, std::make_shared<Memory>(info, nullptr));

    // Register device in shared memory
    if (_shmWriter && _shmWriter->IsInitialized()) {
        _shmWriter->SetDeviceActive(id, true);
        _shmWriter->UpdateDeviceMemory(id, info.mem_size, 0, info.mem_size);
    }
}

void NoServiceLayer::RegisterInferenceResponseHandler(
    int deviceId,
    std::function<void(const dxrt_response_t &)> handler)
{
    std::ignore = deviceId;
    std::ignore = handler;
}

void NoServiceLayer::RegisterErrorHandler(
    int deviceId,
    std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler)
{
     std::ignore = deviceId;
     std::ignore = handler;
}

void NoServiceLayer::RegisterThrottleHandler(
    int deviceId,
    std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler)
{
    std::ignore = deviceId;
    std::ignore = handler;
}

void NoServiceLayer::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config)
{
    std::ignore = taskId;
    std::ignore = modelMemorySize;
    std::ignore = config;
    _ptr[deviceId]->BoundOption(DX_SCHED_ADD, bound);
}
void NoServiceLayer::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
{
    std::ignore = taskId;
    _ptr[deviceId]->BoundOption(DX_SCHED_DELETE, bound);
}

void NoServiceLayer::SignalStoppedDmaToWaitRecovery(int deviceId)
{
    std::ignore = deviceId;
}

void NoServiceLayer::SignalStoppedDmaToWaitRecovery(int deviceId, uint32_t recoveryId)
{
    std::ignore = recoveryId;
    SignalStoppedDmaToWaitRecovery(deviceId);
}

void NoServiceLayer::RegisterRecoveryCallbacks(
    int deviceId,
    std::function<void()> onPause,
    std::function<void()> onResume)
{
    _pauseCallbacks[deviceId]  = std::move(onPause);
    _resumeCallbacks[deviceId] = std::move(onResume);
}

void NoServiceLayer::PauseForRecovery(int deviceId)
{
    auto it = _pauseCallbacks.find(deviceId);
    if (it != _pauseCallbacks.end() && it->second)
        it->second();
}

void NoServiceLayer::ResumeAfterRecovery(int deviceId)
{
    auto it = _resumeCallbacks.find(deviceId);
    if (it != _resumeCallbacks.end() && it->second)
        it->second();
}

[[noreturn]] void NoServiceLayer::OnRecoveryFailed(int deviceId)
{
    LOG_DXRT_ERR("Recovery failed for device " + std::to_string(deviceId) + ". Aborting.");
    std::abort();
}


namespace {

bool ShouldUseBackwardAllocation(MemoryType memoryType)
{
    return memoryType == MemoryType::Model_weight
        || memoryType == MemoryType::Model_rmap
        || memoryType == MemoryType::Model_ppu_binary;
}

}  // namespace



void NoServiceLayer::SignalDeviceReset(int id) { std::ignore = id; }

SharedMemoryInfo NoServiceLayer::AllocateInfo(int deviceId, int taskId, MemoryType memoryType, uint64_t size)
{
    std::lock_guard<std::mutex> lock(_sharedMemoryLock);

    const uint64_t addr = ShouldUseBackwardAllocation(memoryType)
        ? static_cast<uint64_t>(_mems[deviceId]->BackwardAllocate(size))
        : static_cast<uint64_t>(_mems[deviceId]->Allocate(size));
    const uint64_t blockId = _nextNoServiceBlockId.fetch_add(1, std::memory_order_relaxed);

    SharedMemoryInfo info{};
    info.block_id = blockId;
    info.size = size;
    info.ptr = nullptr;
    info.set_phys_addr(addr);
    info.deviceid = deviceId;
    info.taskId = taskId;
    info.pid = current_pid();
    info.fd = -1;

    _blockIdToAddr[blockId] = addr;

    return info;
}

void NoServiceLayer::DeAllocateInfo(int deviceId, const SharedMemoryInfo& info)
{
    std::lock_guard<std::mutex> lock(_sharedMemoryLock);

    uint64_t addr = info.phys_addr();
    if (info.block_id != 0)
    {
        const auto it = _blockIdToAddr.find(info.block_id);
        if (it != _blockIdToAddr.end())
        {
            addr = it->second;
            _blockIdToAddr.erase(it);
        }
    }

    if (addr != static_cast<uint64_t>(-1) && addr != 0)
    {
        _mems[deviceId]->Deallocate(static_cast<int64_t>(addr));
    }
}

void NoServiceLayer::SignalEndJobs(int id) { std::ignore = id; }

void NoServiceLayer::CheckServiceRunning() { /* no service, always running */ }

bool NoServiceLayer::isRunOnService() const { return false; }

void NoServiceLayer::addUsage(int deviceId, int coreId, double value)
{
    // Initialize timer array for this device if not exists (default constructed)
    // No need for explicit assignment - std::map creates default-constructed value
    _usageTimers[deviceId][coreId].add(value);

    // Increment inference count in shared memory
    if (_shmWriter && _shmWriter->IsInitialized()) {
        _shmWriter->IncrementInferenceCount(deviceId);
    }
}

double NoServiceLayer::getUsage(int deviceId, int coreId)
{
    if (_usageTimers.find(deviceId) == _usageTimers.end())
    {
        return 0.0;
    }
    return _usageTimers[deviceId][coreId].getUsage();
}

void NoServiceLayer::onTick(int deviceId, int coreId)
{
    if (_usageTimers.find(deviceId) != _usageTimers.end())
    {
        _usageTimers[deviceId][coreId].onTick();
    }
}

void NoServiceLayer::UsageMonitorThread()
{
    while (_usageMonitorRunning.load(std::memory_order_acquire))
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Call onTick() for all registered devices and cores
        for (const auto& device_pair : _usageTimers)
        {
            int device_id = device_pair.first;
            std::array<double, 3> utilization = {0.0, 0.0, 0.0};

            for (int core_id = 0; core_id < 3; core_id++)
            {
                onTick(device_id, core_id);
                utilization[core_id] = getUsage(device_id, core_id);
            }

            // Write to shared memory for external monitoring tools
            if (_shmWriter && _shmWriter->IsInitialized())
            {
                _shmWriter->UpdateDeviceUtilization(device_id, utilization);

                // Update memory information
                if (_mems.find(device_id) != _mems.end())
                {
                    const auto& mem = _mems[device_id];
                    _shmWriter->UpdateDeviceMemory(
                        device_id,
                        mem->size(),
                        mem->used_size(),
                        mem->free_size()
                    );
                }

                // Update core stats (voltage, clock, temperature)
                if (_ptr.find(device_id) != _ptr.end())
                {
                    auto device_core = _ptr[device_id];
                    auto status = device_core->Status();

                    // Convert C-arrays to std::array (use first 3 elements)
                    std::array<uint32_t, 3> voltage_arr = {status.voltage[0], status.voltage[1], status.voltage[2]};
                    std::array<uint32_t, 3> clock_arr = {status.clock[0], status.clock[1], status.clock[2]};
                    std::array<uint32_t, 3> temp_arr = {status.temperature[0], status.temperature[1], status.temperature[2]};

                    _shmWriter->UpdateDeviceCoreStats(device_id, voltage_arr, clock_arr, temp_arr);
                }
            }
        }
    }
}

void NoServiceLayer::CommitMemory(const SharedMemoryInfo &info)
{
    if (info.ptr == nullptr || info.size == 0)
    {
        return;
    }

    auto coreIt = _ptr.find(info.deviceid);
    if (coreIt == _ptr.end() || coreIt->second == nullptr)
    {
        LOG_DXRT_ERR(
            "NoServiceLayer::CommitMemory missing device core: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size));
        return;
    }

    dxrt_meminfo_t meminfo{};
    meminfo.data = reinterpret_cast<uint64_t>(info.ptr);
    meminfo.base = info.phys_addr_base;
    meminfo.offset = static_cast<uint32_t>(info.phys_addr_offset);
    meminfo.size = static_cast<uint32_t>(info.size);

    if (coreIt->second->Write(meminfo) != 0)
    {
        LOG_DXRT_ERR(
            "NoServiceLayer::CommitMemory core->Write failed: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size));
    }
}

void NoServiceLayer::InvalidateMemory(const SharedMemoryInfo &info)
{
    if (info.ptr == nullptr || info.size == 0)
    {
        return;
    }

    auto coreIt = _ptr.find(info.deviceid);
    if (coreIt == _ptr.end() || coreIt->second == nullptr)
    {
        LOG_DXRT_ERR(
            "NoServiceLayer::InvalidateMemory missing device core: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size));
        return;
    }

    dxrt_meminfo_t meminfo{};
    meminfo.data = reinterpret_cast<uint64_t>(info.ptr);
    meminfo.base = info.phys_addr_base;
    meminfo.offset = static_cast<uint32_t>(info.phys_addr_offset);
    meminfo.size = static_cast<uint32_t>(info.size);

    if (coreIt->second->Read(meminfo) != 0)
    {
        LOG_DXRT_ERR(
            "NoServiceLayer::InvalidateMemory core->Read failed: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size));
    }
}

int NoServiceLayer::DMAWrite(SharedMemoryView view)
{
    if (view.hostPtr() == nullptr || !view.isValid())
    {
        errno = EINVAL;
        return -1;
    }

    const int deviceId = view.info.deviceid;
    const uint64_t sourceAddress = reinterpret_cast<uint64_t>(view.hostPtr());
    const uint64_t destinationAddress = view.deviceAddress();

    auto coreIt = _ptr.find(deviceId);
    if (coreIt == _ptr.end() || coreIt->second == nullptr)
    {
        return -1;
    }

    dxrt_meminfo_t meminfo{};
    meminfo.data = sourceAddress;

    const uint64_t deviceBase = coreIt->second->info().mem_addr;
    // Memory pool uses 0-based offsets (pool starts at addr=0, not mem_addr).
    // destinationAddress may be either an absolute device address (>= deviceBase)
    // or a pool-relative offset (< deviceBase). Handle both cases.
    meminfo.base = deviceBase;
    if (destinationAddress >= deviceBase)
        meminfo.offset = static_cast<uint32_t>(destinationAddress - deviceBase);
    else
        meminfo.offset = static_cast<uint32_t>(destinationAddress);
    meminfo.size = static_cast<uint32_t>(view.size);

    return coreIt->second->Write(meminfo);
}

int NoServiceLayer::DMARead(SharedMemoryView view)
{
    if (view.hostPtr() == nullptr || !view.isValid())
    {
        errno = EINVAL;
        return -1;
    }

    const int deviceId = view.info.deviceid;
    const uint64_t sourceAddress = view.deviceAddress();
    const uint64_t destinationAddress = reinterpret_cast<uint64_t>(view.hostPtr());

    auto coreIt = _ptr.find(deviceId);
    if (coreIt == _ptr.end() || coreIt->second == nullptr)
    {
        return -1;
    }

    dxrt_meminfo_t meminfo{};
    meminfo.data = destinationAddress;

    const uint64_t deviceBase = coreIt->second->info().mem_addr;
    // Memory pool uses 0-based offsets (pool starts at addr=0, not mem_addr).
    // sourceAddress may be either an absolute device address (>= deviceBase)
    // or a pool-relative offset (< deviceBase). Handle both cases.
    meminfo.base = deviceBase;
    if (sourceAddress >= deviceBase)
        meminfo.offset = static_cast<uint32_t>(sourceAddress - deviceBase);
    else
        meminfo.offset = static_cast<uint32_t>(sourceAddress);
    meminfo.size = static_cast<uint32_t>(view.size);

    return coreIt->second->Read(meminfo);
}
int NoServiceLayer::DMAReadWithFaultInjection(SharedMemoryView view)
{
    return NoServiceLayer::DMARead(view);
}

}  // namespace dxrt
