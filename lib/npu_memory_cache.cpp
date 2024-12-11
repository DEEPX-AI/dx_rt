#include <cstdint>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>
#include "dxrt/common.h"
#include "dxrt/npu_memory_cache.h"
#include "dxrt/device.h"




namespace dxrt {



NpuMemoryCacheManager::NpuMemoryCacheManager(Device* device_)
: _device(device_)
{
}


bool NpuMemoryCacheManager::registerMemoryCache(int taskId, int64_t size, int count)
{
    std::unique_lock<std::mutex> lock(_npuMemoryCacheLock);


    int64_t offset = _device->Allocate(size * count);

    if (offset != -1)
    {
        LOG_DXRT_DBG << "init: " << offset << " is inited" << endl;
        _npuMemoryCacheOffset[taskId] = offset;
        for (int i = 0; i < count; i++)
        {
            _npuMemoryCaches[taskId].push_back(offset + size * i);
            LOG_DXRT_DBG << taskId << " init: " << _npuMemoryCaches[taskId].back() << " is pushed" << endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}
void NpuMemoryCacheManager::unRegisterMemorgCache(int taskId)
{
    std::unique_lock<std::mutex> lock(_npuMemoryCacheLock);

    auto it = _npuMemoryCacheOffset.find(taskId);
    if (it == _npuMemoryCacheOffset.end())
    {
        return;
    }
    _device->Deallocate(it->second);
    _npuMemoryCacheOffset.erase(it);
    _npuMemoryCaches.erase(taskId);
}
bool NpuMemoryCacheManager::canGetCache(int taskId)
{
    std::unique_lock<std::mutex> lock(_npuMemoryCacheLock);

    return _npuMemoryCacheOffset.find(taskId) != _npuMemoryCacheOffset.end();
}
int64_t NpuMemoryCacheManager::getNpuMemoryCache(int taskId)
{
    while(_npuMemoryCaches[taskId].empty())
    {
        usleep(10);
    }
    std::unique_lock<std::mutex> lock(_npuMemoryCacheLock);
    int64_t retval = _npuMemoryCaches[taskId].back();
    _npuMemoryCaches[taskId].pop_back();
    return retval;
}
void NpuMemoryCacheManager::returnNpuMemoryCache(int taskId, int64_t addr)
{
    std::unique_lock<std::mutex> lock(_npuMemoryCacheLock);
    _npuMemoryCaches[taskId].push_back(addr);
}


}  // namespace dxrt
