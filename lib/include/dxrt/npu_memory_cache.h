/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <memory>
#include "../../dynamic_ipc/shm/shm.h"



namespace dxrt {
class DeviceTaskLayer;

struct NpuMemoryCacheSlice
{
    SharedMemoryView view;
    int sliceId;  // Unique identifier for this slice, used for tracking and debugging.

    void* hostPtr() const {return view.hostPtr(); }

    uint64_t deviceAddress() const { return view.deviceAddress(); }
    bool isValid() const { return view.isValid(); }
};

class TaskNpuMemoryCacheManager
{
public:
    TaskNpuMemoryCacheManager(int64_t size, int count, SharedMemoryInfo info);
    NpuMemoryCacheSlice getNpuMemoryCache();
    void returnNpuMemoryCache(const NpuMemoryCacheSlice &slice);
    SharedMemoryInfo getMemoryInfo() const;
    ~TaskNpuMemoryCacheManager();

    TaskNpuMemoryCacheManager(const TaskNpuMemoryCacheManager&) = delete;
    TaskNpuMemoryCacheManager& operator=(const TaskNpuMemoryCacheManager&) = delete;
    TaskNpuMemoryCacheManager(TaskNpuMemoryCacheManager&&) = delete;
    TaskNpuMemoryCacheManager& operator=(TaskNpuMemoryCacheManager&&) = delete;

private:
    std::vector<NpuMemoryCacheSlice> _npuMemoryCaches;
    SharedMemoryInfo _npuMemoryInfo;
    std::mutex _lock;
    std::condition_variable _cv;
};

class NpuMemoryCacheManager
{
public:
    explicit NpuMemoryCacheManager(DeviceTaskLayer* device_);
    bool registerMemoryCache(int taskId, int64_t size, int count);
    void unRegisterMemoryCache(int taskId);
    bool canGetCache(int taskId);
    NpuMemoryCacheSlice getNpuMemoryCache(int taskId);
    void returnNpuMemoryCache(int taskId, const NpuMemoryCacheSlice &slice);
    SharedMemoryInfo getBackingInfo(int taskId);
private:
    std::unordered_map<int, std::shared_ptr<TaskNpuMemoryCacheManager> > _taskNpuMemoryCaches;
    SharedMutex _npuMemoryCacheLock;
    DeviceTaskLayer* _device;
};

}  // namespace dxrt
