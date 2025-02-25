// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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




namespace dxrt {
class Device;

class TaskNpuMemoryCacheManager
{
public:
    TaskNpuMemoryCacheManager(int64_t size, int count, int64_t offset);
    int64_t getNpuMemoryCache();
    void returnNpuMemoryCache(int64_t addr);
    int64_t getOffset();
    ~TaskNpuMemoryCacheManager();
private:
    std::vector<int64_t> _npuMemoryCaches;
    int64_t _npuMemoryCacheOffset;
    std::mutex _lock;
    std::condition_variable _cv;
};

class NpuMemoryCacheManager
{
public:
    NpuMemoryCacheManager(Device* device_);
    bool registerMemoryCache(int taskId, int64_t size, int count);
    void unRegisterMemoryCache(int taskId);
    bool canGetCache(int taskId);
    int64_t getNpuMemoryCache(int taskId);
    void returnNpuMemoryCache(int taskId, int64_t addr);
private:
    std::unordered_map<int, std::shared_ptr<TaskNpuMemoryCacheManager> > _taskNpuMemoryCaches;
    SharedMutex _npuMemoryCacheLock;
    Device* _device;
};

}  // namespace dxrt
