/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>
#include "dxrt/common.h"
#include "include/dxrt/npu_memory_cache.h"
#include "include/dxrt/device_task_layer.h"


using std::endl;

namespace dxrt {

TaskNpuMemoryCacheManager::TaskNpuMemoryCacheManager(int64_t size, int count, SharedMemoryInfo info)
{
    LOG_DXRT_DBG << "init: " << info.phys_addr_offset << " is inited" << endl;
    _npuMemoryInfo = info;
    for (int i = 0; i < count; i++)
    {
        NpuMemoryCacheSlice slice;
        slice.view.info   = info;
        slice.view.offset = static_cast<uint64_t>(size * i);
        slice.view.size   = static_cast<uint64_t>(size);
        _npuMemoryCaches.push_back(slice);
        LOG_DXRT_DBG << " init: " << slice.view.offset << " is pushed" << endl;
    }
}
TaskNpuMemoryCacheManager::~TaskNpuMemoryCacheManager()
{
    _cv.notify_all();
}

SharedMemoryInfo TaskNpuMemoryCacheManager::getMemoryInfo() const
{
    return _npuMemoryInfo;
}
void TaskNpuMemoryCacheManager::returnNpuMemoryCache(const NpuMemoryCacheSlice &slice)
{
    std::unique_lock<std::mutex> lock(_lock);
    _npuMemoryCaches.push_back(slice);
    _cv.notify_one();
}
NpuMemoryCacheSlice TaskNpuMemoryCacheManager::getNpuMemoryCache()
{
    std::unique_lock<std::mutex> lock(_lock);
    _cv.wait(lock, [this] {
        return _npuMemoryCaches.empty() == false;
    });

    NpuMemoryCacheSlice retval = _npuMemoryCaches.back();
    _npuMemoryCaches.pop_back();
    return retval;
}

NpuMemoryCacheManager::NpuMemoryCacheManager(DeviceTaskLayer* device_)
: _device(device_)
{
}


bool NpuMemoryCacheManager::registerMemoryCache(int taskId, int64_t size, int count)
{
    UniqueLock lock(_npuMemoryCacheLock);

    SharedMemoryInfo info = _device->AllocateInfo(taskId, MemoryType::Input_output, size * count);

    // Success: fd >= 0 (service mode shared memory) or block_id >= 0 (no-service mode pool allocation)
    if (info.fd >= 0 || info.block_id >= 0)
    {
        LOG_DXRT_DBG << "init: " << info.phys_addr_offset << " is inited" << endl;
        _taskNpuMemoryCaches.emplace(taskId,
            std::make_shared<TaskNpuMemoryCacheManager>(size, count, info));
        return true;
    }
    else
    {
        return false;
    }
}
void NpuMemoryCacheManager::unRegisterMemoryCache(int taskId)
{
    UniqueLock lock(_npuMemoryCacheLock);

    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it == _taskNpuMemoryCaches.end())
    {
        return;
    }
    _device->DeallocateInfo(it->second->getMemoryInfo());
    _taskNpuMemoryCaches.erase(it);
}
bool NpuMemoryCacheManager::canGetCache(int taskId)
{
    SharedLock lock(_npuMemoryCacheLock);

    return _taskNpuMemoryCaches.find(taskId) != _taskNpuMemoryCaches.end();
}
NpuMemoryCacheSlice NpuMemoryCacheManager::getNpuMemoryCache(int taskId)
{
    SharedLock lock(_npuMemoryCacheLock);
    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it != _taskNpuMemoryCaches.end())
    {
        auto manager =  it->second;
        lock.unlock();
        return manager->getNpuMemoryCache();
    }
    return NpuMemoryCacheSlice{};
}
SharedMemoryInfo NpuMemoryCacheManager::getBackingInfo(int taskId)
{
    SharedLock lock(_npuMemoryCacheLock);
    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it != _taskNpuMemoryCaches.end())
    {
        return it->second->getMemoryInfo();
    }
    return SharedMemoryInfo{};
}

void NpuMemoryCacheManager::returnNpuMemoryCache(int taskId, const NpuMemoryCacheSlice &slice)
{
    SharedLock lock(_npuMemoryCacheLock);
    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it != _taskNpuMemoryCaches.end())
    {
        it->second->returnNpuMemoryCache(slice);
    }
}


}  // namespace dxrt
