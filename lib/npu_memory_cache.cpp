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
#include "dxrt/npu_memory_cache.h"
#include "dxrt/device.h"


using std::endl;

namespace dxrt {

TaskNpuMemoryCacheManager::TaskNpuMemoryCacheManager(int64_t size, int count, int64_t offset)
{

    LOG_DXRT_DBG << "init: " << offset << " is inited" << endl;
    _npuMemoryCacheOffset = offset;
    for (int i = 0; i < count; i++)
    {
        _npuMemoryCaches.push_back(offset + size * i);
        LOG_DXRT_DBG << " init: " << _npuMemoryCaches.back() << " is pushed" << endl;
    }


}
TaskNpuMemoryCacheManager::~TaskNpuMemoryCacheManager()
{
    _cv.notify_all();
}

int64_t TaskNpuMemoryCacheManager::getOffset()
{
    return _npuMemoryCacheOffset;
}
void TaskNpuMemoryCacheManager::returnNpuMemoryCache(int64_t addr)
{
    std::unique_lock<std::mutex> lock(_lock);
    _npuMemoryCaches.push_back(addr);
    _cv.notify_one(); 
}
int64_t TaskNpuMemoryCacheManager::getNpuMemoryCache()
{
    std::unique_lock<std::mutex> lock(_lock);
    _cv.wait(lock, [this] {
        return _npuMemoryCaches.empty() == false;
    });


    int64_t retval = _npuMemoryCaches.back();
    _npuMemoryCaches.pop_back();
    return retval;
}

NpuMemoryCacheManager::NpuMemoryCacheManager(Device* device_)
: _device(device_)
{
}


bool NpuMemoryCacheManager::registerMemoryCache(int taskId, int64_t size, int count)
{
    UniqueLock lock(_npuMemoryCacheLock);


    int64_t offset = _device->Allocate(size * count);

    if (offset != -1)
    {
        LOG_DXRT_DBG << "init: " << offset << " is inited" << endl;
        _taskNpuMemoryCaches.emplace(taskId,
            std::make_shared<TaskNpuMemoryCacheManager>(size, count, offset));
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
    _device->Deallocate(it->second->getOffset());
    _taskNpuMemoryCaches.erase(it);
}
bool NpuMemoryCacheManager::canGetCache(int taskId)
{
    SharedLock lock(_npuMemoryCacheLock);

    return _taskNpuMemoryCaches.find(taskId) != _taskNpuMemoryCaches.end();
}
int64_t NpuMemoryCacheManager::getNpuMemoryCache(int taskId)
{
    SharedLock lock(_npuMemoryCacheLock);
    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it != _taskNpuMemoryCaches.end())
    {
        auto manager =  it->second;
        lock.unlock();
        return manager->getNpuMemoryCache();
    }
    return -1;
}
void NpuMemoryCacheManager::returnNpuMemoryCache(int taskId, int64_t addr)
{
    SharedLock lock(_npuMemoryCacheLock);
    auto it = _taskNpuMemoryCaches.find(taskId);
    if (it != _taskNpuMemoryCaches.end())
    {
        return it->second->returnNpuMemoryCache(addr);
    }
}


}  // namespace dxrt
