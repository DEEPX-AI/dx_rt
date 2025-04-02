

#include <memory>
#include <set>
#include <map>
#include <iostream>
#include "dxrt/common.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "dxrt/ipc_wrapper/ipc_server.h"
#include "dxrt/memory.h"
#include "dxrt/device.h"
#include "memory_service.hpp"
#include "service_device.h"

using std::endl;

namespace dxrt {

std::vector<MemoryService*> MemoryService::_instances;

MemoryService* MemoryService::getInstance(int deviceId)
{
    if(_instances.empty())
    {
        auto device_list = ServiceDevice::CheckServiceDevices();
        if (device_list.size() < 1)
        {
            LOG_DXRT_S_DBG << "no device detected" << errno << endl;
        }
        for (auto device : device_list)
        {
            dxrt_device_info_t info = device->info();
            //std::cout << "device insert:" << device->id() << endl;
            _instances.push_back(new MemoryService(info.mem_addr, info.mem_size));
            LOG_DXRT_S_DBG << "device insert:" << device->id() << endl;
        }
        LOG_DXRT_S_DBG << "device count:" << _instances.size() << endl;
    }
    if ((_instances.size() <= static_cast<size_t>(deviceId)) || (deviceId < 0))
    {
        return nullptr;
    }
    return _instances[deviceId];
}


MemoryService::MemoryService(uint64_t start, uint64_t size)
{
    dxrt_device_info_t info;
    info.mem_addr = start;
    info.mem_size = size;

    _mem = new dxrt::Memory(info, nullptr);
}

uint64_t MemoryService::Allocate(uint64_t size, pid_t pid)
{
    uint64_t addr = _mem->Allocate(size);
    _allocInfo[pid].insert(addr);
    LOG_DXRT_S_DBG << hex << addr << dec  << " is allocated, size:" << size << endl;
    return addr;
}
uint64_t MemoryService::BackwardAllocate(uint64_t size, pid_t pid)
{
    uint64_t addr = _mem->BackwardAllocate(size);
    _allocInfo[pid].insert(addr);
    LOG_DXRT_S_DBG << hex << addr << dec  << " is allocated, size:" << size << endl;
    return addr;
}
bool MemoryService::Deallocate(uint64_t addr, pid_t pid)
{
    auto it1 = _allocInfo.find(pid);
    if (it1 == _allocInfo.end())
    {
        LOG_DXRT_S << "not regestered pid " << pid << endl;
        return false;
    }
    auto it2 = it1->second.find(addr);
    if (it2 == it1->second.end())
    {
        LOG_DXRT_S << "not allocated addr " << hex << addr << dec  << " for pid " << pid <<endl;
        return false;
    }
    it1->second.erase(it2);
    _mem->Deallocate(addr);
    LOG_DXRT_S_DBG << hex << addr << dec << " is Deallocated"<< " for pid " << pid << endl;

    return true;
}
void MemoryService::DeallocateAll(pid_t pid)
{
    auto it1 = _allocInfo.find(pid);
    if (it1 == _allocInfo.end())
    {
        LOG_DXRT_DBG << "not regestered pid " << pid << endl;
        return;
    }
    for (auto it2 : it1->second)
    {
        _mem->Deallocate(it2);
    }
    _allocInfo.erase(it1);
}
void MemoryService::DeallocateAllDevice(pid_t pid)
{
    for (size_t i = 0; i < _instances.size(); i++)
    {
        _instances[i]->DeallocateAll(pid);
    }
}

uint64_t MemoryService::free_size() const
{
    return _mem->free_size();
}

uint64_t MemoryService::used_size() const
{
    return _mem->used_size();
}



}  // namespace dxrt
