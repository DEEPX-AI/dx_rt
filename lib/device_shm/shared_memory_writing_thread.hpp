/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "dxrt/common.h"
#include "shared_memory_writer.h"


namespace dxrt {

class DeviceDispatcher;

class SharedMemoryWritingThread
{
 public:
    using MemoryStatsProvider =
        std::function<bool(uint64_t* total, uint64_t* used, uint64_t* free)>;
    using UtilizationProvider =
        std::function<bool(std::array<double, 3>* utilization)>;
    using CoreStatsProvider = std::function<bool(
        std::array<uint32_t, 3>* voltage,
        std::array<uint32_t, 3>* clock,
        std::array<uint32_t, 3>* temperature)>;

    SharedMemoryWritingThread() = default;
    ~SharedMemoryWritingThread() = default;
    void RegisterDevice(DeviceDispatcher* deviceCore);
    void RegisterDeviceCallbacks(
        int deviceId,
        DeviceDispatcher* dispatcher,
        UtilizationProvider utilizationProvider,
        CoreStatsProvider coreStatsProvider,
        MemoryStatsProvider memoryProvider = MemoryStatsProvider{});
    bool Start();
    void Stop();

 private:
    struct DeviceEntry
    {
        int deviceId = -1;
        DeviceDispatcher* dispatcher = nullptr;
        UtilizationProvider utilizationProvider;
        CoreStatsProvider coreStatsProvider;
        MemoryStatsProvider memoryProvider;
    };

    void threadFunc();
    std::vector<DeviceEntry> _devices;

    std::thread _thread;
    std::atomic<bool> _running{false};
    std::mutex _mutex;
    SharedMemoryWriter _shmWriter;
};

}  // namespace dxrt
