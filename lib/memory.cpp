// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/memory.h"

using namespace std;

namespace dxrt {

Memory::Memory(dxrt_device_info_t &info, void *data_)
{
    _start = info.mem_addr;
    _cur = info.mem_addr;
    _end = info.mem_addr + info.mem_size;
    _size = info.mem_size;
    _data = reinterpret_cast<uint64_t>(data_);
    _dataEnd = _data + _size;
    _pool[0].addr = 0;
    _pool[0].size = _size;
    _pool[0].status = 0;
}
void* Memory::GetBuffer(uint32_t required)
{
    uint32_t offset;
    unique_lock<mutex> lk(_lock);
    if(required>_size)
    {
        return nullptr;
    }
    if(_cur + required > _end)
    {
        _cur = _start;
    }
    offset = _cur - _start;
    _cur += required;
    return (reinterpret_cast<void*>(_data + offset));
}
uint32_t Memory::GetBufferAsOffset(uint32_t required)
{
    uint32_t offset;
    unique_lock<mutex> lk(_lock);
    if(required>_size)
    {
        return 0;
    }
    if(_cur + required > _end)
    {
        _cur = _start;
    }
    offset = _cur - _start;
    _cur += required;
    LOG_DXRT_DBG << "offset : "<< offset << ", required : "<< required<< endl;
    return offset;
}
int64_t Memory::Allocate(uint64_t required)
{
    LOG_DXRT_DBG << endl;
    unique_lock<mutex> lk(_lock);
    for(auto &pair:_pool)
    {
        auto &node = pair.second;
        if(node.status==0)
        {
            if(required <= node.size)
            {
                uint64_t addr = node.addr;
                // LOG_VALUE_HEX(ret);
                if(required < node.size)
                {
                    _pool[addr + required].addr = addr + required;
                    _pool[addr + required].size = node.size - required;
                    _pool[addr + required].status = 0;
                }
                _pool[addr].addr = addr;
                _pool[addr].size = required;
                _pool[addr].status = 1;
                return addr;
            }
        }
    }
    cout << "failed to allocate memory" << endl;
    return -1;
}
int64_t Memory::BackwardAllocate(uint64_t required)
{
    LOG_DXRT_DBG << endl;
    unique_lock<mutex> lk(_lock);
    for(auto it = _pool.rbegin(); it != _pool.rend(); it++)
    {
        auto &node = it->second;
        if(node.status==0)
        {
            if(required <= node.size)
            {
                uint64_t addr = node.addr;
                // LOG_VALUE_HEX(ret);
                if(required < node.size)
                {
                    uint64_t remain = node.size - required;
                    _pool[addr].addr = addr;
                    _pool[addr].size = remain;
                    _pool[addr].status = 0;

                    addr += remain;
                }

                _pool[addr].addr = addr;
                _pool[addr].size = required;
                _pool[addr].status = 1;
                return addr;

            }
        }
    }
    cout << "failed to allocate memory" << endl;
    return -1;    
}


int64_t Memory::Allocate(dxrt_meminfo_t &meminfo)
{
    LOG_DXRT_DBG << endl;
    if(meminfo.data==0)
    {
        /* allocate, new */
        LOG_DXRT_DBG << "allocate, new" << endl;
        meminfo.base = _start;
        meminfo.offset = Allocate(meminfo.size);
        meminfo.data = _data + meminfo.offset;
    }
    else if(meminfo.data < _data || meminfo.data >_dataEnd)
    {       
        /* allocate, out of area, don't know phy addr, need memcpy */
        LOG_DXRT_DBG << "allocate, out of area" << endl;
        if(meminfo.base==0)
        {
            meminfo.base = _start;
        }
        meminfo.offset = Allocate(meminfo.size);
    }
    else
    {
        LOG_DXRT_DBG << "skip allocate, update base, offset" << endl;
        /* no allocate, just compute base, offset */
        meminfo.base = _start;
        meminfo.offset = meminfo.data - _data;
    }

    return 0;
}
int64_t Memory::Allocate(dxrt_request_t &inf)
{
    LOG_DXRT_DBG << endl;
    auto &input = inf.input;
    auto &output = inf.output;
    Allocate(input);
    Allocate(output);
    return 0;
}
void Memory::Deallocate(uint64_t addr)
{
    // LOG_DXRT_DBG << hex << addr << dec << endl;
    unique_lock<mutex> lk(_lock);
    auto it = _pool.find(addr);
    if(it != _pool.end())
    {
        it->second.status = 0;
        MergeAdjacentNodes(it);
    }
}
void Memory::Deallocate(dxrt_meminfo_t &meminfo)
{
    // LOG_DXRT_DBG << endl;
    if(meminfo.base == _start)
    {        
        Deallocate(meminfo.offset);
    }
    else
    {
        LOG_DXRT_DBG << "skip" << endl;
    }
}
void Memory::Deallocate(dxrt_request_t &inf)
{
    // LOG_DXRT_DBG << endl;
    auto &input = inf.input;
    auto &output = inf.output;
    Deallocate(input);
    Deallocate(output);    
}
void Memory::MergeAdjacentNodes(std::map<uint64_t, MemoryNode>::iterator it) {
    // LOG_DXRT_DBG << endl;
    if (it != _pool.begin()) {
        auto prev_it = prev(it);
        if (prev_it->second.status == 0) {
            uint64_t new_addr = prev_it->first;
            uint64_t new_size = prev_it->second.size + it->second.size;
            _pool.erase(prev_it);
            _pool.erase(it);
            MemoryNode& newNode = _pool[new_addr];
            newNode.addr = new_addr;
            newNode.size = new_size;
            newNode.status = 0;
            MergeAdjacentNodes(_pool.find(new_addr));
            LOG_DXRT_DBG << *this << endl;
            return;
        }
    }
    auto next_it = next(it);
    if (next_it != _pool.end() && next_it->second.status == 0) {
        uint64_t new_addr = it->first;
        uint64_t new_size = it->second.size + next_it->second.size;
        _pool.erase(it);
        _pool.erase(next_it);
        MemoryNode& newNode = _pool[new_addr];
        newNode.addr = new_addr;
        newNode.size = new_size;
        newNode.status = 0;
        MergeAdjacentNodes(_pool.find(new_addr));
    }
}
void Memory::ResetBuffer()
{
    _cur = _start;
}
uint64_t Memory::start(void)
{
    return _start;
}
uint64_t Memory::end(void)
{
    return _end;
}
uint64_t Memory::size(void)
{
    return _size;
}
uint64_t Memory::data(void)
{
    return _data;
}


ostream& operator<<(std::ostream& os, const MemoryNode& node)
{
    os << hex << "[" << node.addr 
        << ", " << node.size 
        << ", " << node.status << "]";
    os << dec;
    return os;
}
ostream& operator<<(std::ostream& os, const Memory& memory)
{
    os << "      Memory @ " << hex << memory._start << " ~ " << memory._end 
        << "(" << memory._data << " ~ " << memory._dataEnd << "), " << memory._size << ", cur " << memory._cur << ", ";
    for(const auto &pair:memory._pool)
    {
        os << pair.second << ", ";
    }
    os << dec;
    return os;
}

} // namespace dxrt
