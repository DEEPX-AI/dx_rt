/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/memory.h"
#include <iostream>
#include <iomanip>
#include <mutex>
using std::endl;
using std::hex;
using std::dec;

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
    _used_size = 0;
}

uint64_t Memory::AlignSize(uint64_t size) const
{
    return (size + MemoryConfig::MEMORY_ALIGNMENT - 1) & ~(MemoryConfig::MEMORY_ALIGNMENT - 1);
}

std::map<uint64_t, MemoryNode>::iterator Memory::FindBestFit(uint64_t required)
{
    auto best_it = _pool.end();
    uint64_t best_size = UINT64_MAX;

    for (auto it = _pool.begin(); it != _pool.end(); ++it)
    {
        auto &node = it->second;
        if (node.status == 0 && node.size >= required)
        {
            if (node.size < best_size)
            {
                best_size = node.size;
                best_it = it;
            }
        }
    }

    return best_it;
}

int64_t Memory::Allocate(uint64_t required)
{
    LOG_DXRT_DBG << endl;
    std::unique_lock<std::mutex> lk(_lock);

    if (required == 0)
    {
        LOG_DXRT << "required size is 0 !!!" << endl;
        required = 64;
    }


    // Align the requested size for better performance
    required = AlignSize(required);

    // First attempt: Try normal Best-Fit allocation
    auto best_it = FindBestFit(required);

    if (best_it == _pool.end())
    {
        // Second attempt: Check if defragmentation can help for large allocations
        if (required >= MemoryConfig::LARGE_ALLOCATION_THRESHOLD)
        {
            auto fragInfo = GetFragmentationInfo();
            if (fragInfo.fragmentation_ratio > MemoryConfig::MEDIUM_FRAGMENTATION_THRESHOLD)
            {
                LOG_DXRT_DBG << "Attempting defragmentation for " << (required / (1024*1024)) << "MB allocation" << endl;
                if (TryDefragmentation(required))
                {
                    best_it = FindBestFit(required);
                }
            }
        }
    }

    if (best_it != _pool.end())
    {
        auto &node = best_it->second;
        uint64_t addr = node.addr;

        if (required < node.size)
        {
            _pool[addr + required].addr = addr + required;
            _pool[addr + required].size = node.size - required;
            _pool[addr + required].status = 0;
        }
        _pool[addr].addr = addr;
        _pool[addr].size = required;
        _pool[addr].status = 1;
        _used_size += required;
        LOG_DXRT_DBG << required << " byte Allocated (Best-Fit) 0x" <<  hex << addr << dec << endl;
        return addr;
    }

    // Memory allocation failed - provide detailed diagnosis
    auto fragInfo = GetFragmentationInfo();
    LOG_DXRT_ERR("Failed to allocate " + std::to_string(required / (1024*1024)) + "MB. " +
                 "Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                 "Largest block: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB, " +
                 "Fragmentation: " + std::to_string(fragInfo.fragmentation_ratio * 100.0) + "%")

    return -1;
}

int64_t Memory::BackwardAllocate(uint64_t required)
{
    LOG_DXRT_DBG << endl;
    std::unique_lock<std::mutex> lk(_lock);

    // Align the requested size
    required = AlignSize(required);

    // Best-Fit algorithm for backward allocation
    auto best_it = _pool.end();
    uint64_t best_size = UINT64_MAX;

    for (auto it = _pool.rbegin(); it != _pool.rend(); ++it)
    {
        auto &node = it->second;
        if (node.status == 0 && node.size >= required)
        {
            if (node.size < best_size)
            {
                best_size = node.size;
                best_it = it.base();
                --best_it;
            }
        }
    }

    if (best_it == _pool.end() && required >= MemoryConfig::LARGE_ALLOCATION_THRESHOLD)
    {
        // Try defragmentation for large backward allocations
        auto fragInfo = GetFragmentationInfo();
        if (fragInfo.fragmentation_ratio > MemoryConfig::MEDIUM_FRAGMENTATION_THRESHOLD)
        {
            if (TryDefragmentation(required))
            {
                // Retry after defragmentation
                for (auto it = _pool.rbegin(); it != _pool.rend(); ++it)
                {
                    auto &node = it->second;
                    if (node.status == 0 && node.size >= required)
                    {
                        if (node.size < best_size)
                        {
                            best_size = node.size;
                            best_it = it.base();
                            --best_it;
                        }
                    }
                }
            }
        }
    }

    if (best_it != _pool.end())
    {
        auto &node = best_it->second;
        uint64_t addr = node.addr;

        if (required < node.size)
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
        _used_size += required;
        LOG_DXRT_DBG << required << " byte Allocated B (Best-Fit) 0x" <<  hex << addr << dec << endl;
        return addr;
    }

    // Memory allocation failed
    auto fragInfo = GetFragmentationInfo();
    LOG_DXRT_ERR("Failed to backward allocate " + std::to_string(required / (1024*1024)) + "MB. " +
                 "Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                 "Largest block: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB")

    return -1;
}

int64_t Memory::Allocate(dxrt_meminfo_t &meminfo)
{
    LOG_DXRT_DBG << endl;
    if (meminfo.data == 0)
    {
        /* allocate, new */
        LOG_DXRT_DBG << "allocate, new" << endl;
        meminfo.base = _start;
        meminfo.offset = Allocate(meminfo.size);
        meminfo.data = _data + meminfo.offset;
    }
    else if (meminfo.data < _data || meminfo.data >_dataEnd)
    {
        /* allocate, out of area, don't know phy addr, need memcpy */
        LOG_DXRT_DBG << "allocate, out of area" << endl;
        if (meminfo.base == 0)
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
    std::unique_lock<std::mutex> lk(_lock);

    auto it = _pool.find(addr);
    if (it != _pool.end())
    {
        _used_size -= it->second.size;
        it->second.status = 0;
        LOG_DXRT_DBG << it->second.size << " byte Deallocated 0x" << hex << addr << dec << endl;

        MergeAdjacentNodes(it);
    }
}
void Memory::Deallocate(dxrt_meminfo_t &meminfo)
{
    if (meminfo.base == _start)
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
uint64_t Memory::start(void) const
{
    return _start;
}
uint64_t Memory::end(void) const
{
    return _end;
}
uint64_t Memory::size(void) const
{
    return _size;
}
uint64_t Memory::data(void) const
{
    return _data;
}
uint64_t Memory::free_size(void) const
{
    return _size - _used_size;
}
uint64_t Memory::used_size(void) const
{
    return _used_size;
}

MemoryFragmentationInfo Memory::GetFragmentationInfo() const
{
    std::unique_lock<std::mutex> lk(const_cast<std::mutex&>(_lock));
    MemoryFragmentationInfo info = {0, 0, UINT64_MAX, 0, 0.0};

    for (const auto &pair : _pool)
    {
        const auto &node = pair.second;
        if (node.status == 0)  // free block
        {
            info.total_free_size += node.size;
            info.free_block_count++;

            if (node.size > info.largest_free_block)
                info.largest_free_block = node.size;

            if (node.size < info.smallest_free_block)
                info.smallest_free_block = node.size;
        }
    }

    if (info.free_block_count == 0)
    {
        info.smallest_free_block = 0;
        info.fragmentation_ratio = 0.0;
    }
    else if (info.total_free_size > 0)
    {
        info.fragmentation_ratio = (double)(info.total_free_size - info.largest_free_block) / info.total_free_size;
    }

    return info;
}

bool Memory::CanAllocateContiguous(uint64_t required) const
{
    std::unique_lock<std::mutex> lk(const_cast<std::mutex&>(_lock));
    for (const auto &pair : _pool)
    {
        const auto &node = pair.second;
        if (node.status == 0 && node.size >= required)
        {
            return true;
        }
    }
    return false;
}

void Memory::PrintMemoryMap() const
{
    std::unique_lock<std::mutex> lk(const_cast<std::mutex&>(_lock));
    LOG_DXRT << "Memory Map (Start: 0x" << hex << _start << ", Size: " << dec << _size << " bytes)" << endl;
    LOG_DXRT << "Used: " << _used_size << " bytes, Free: " << (_size - _used_size) << " bytes" << endl;

    for (const auto &pair : _pool)
    {
        const auto &node = pair.second;
        const char* status_str = (node.status == 0) ? "FREE" : "USED";
        LOG_DXRT << "  [0x" << hex << node.addr << " - 0x" << (node.addr + node.size)
                 << "] Size: " << dec << node.size << " bytes, Status: " << status_str << endl;
    }

    auto fragInfo = GetFragmentationInfo();
    LOG_DXRT << "Fragmentation Info:" << endl;
    LOG_DXRT << "  Total Free: " << fragInfo.total_free_size << " bytes" << endl;
    LOG_DXRT << "  Largest Free Block: " << fragInfo.largest_free_block << " bytes" << endl;
    LOG_DXRT << "  Smallest Free Block: " << fragInfo.smallest_free_block << " bytes" << endl;
    LOG_DXRT << "  Free Block Count: " << fragInfo.free_block_count << endl;
    LOG_DXRT << "  Fragmentation Ratio: " << (fragInfo.fragmentation_ratio * 100.0) << "%" << endl;
}

bool Memory::TryDefragmentation(uint64_t required_size)
{
    LOG_DXRT_DBG << "Starting defragmentation for " << (required_size / (1024*1024)) << "MB" << endl;

    // Step 1: Merge all adjacent free blocks
    MergeAllAdjacentFreeBlocks();

    // Step 2: Check if we now have a large enough block
    uint64_t largest_free = GetLargestFreeBlock();
    if (largest_free >= required_size)
    {
        LOG_DXRT_DBG << "Defragmentation successful: largest free block now "
                     << (largest_free / (1024*1024)) << "MB" << endl;
        return true;
    }

    // Step 3: For aggressive defragmentation, we could implement memory compaction
    // but this would require moving allocated blocks (complex and risky)
    LOG_DXRT_DBG << "Defragmentation completed but insufficient: largest block "
                 << (largest_free / (1024*1024)) << "MB" << endl;
    return false;
}

void Memory::CompactMemory()
{
    // This is a complex operation that would require:
    // 1. Moving allocated blocks to eliminate gaps
    // 2. Updating all references to moved blocks
    // 3. Coordinating with NPU hardware
    // For now, we only implement the safer merge operation
    MergeAllAdjacentFreeBlocks();
}

uint64_t Memory::GetLargestFreeBlock() const
{
    uint64_t largest = 0;
    for (const auto &pair : _pool)
    {
        const auto &node = pair.second;
        if (node.status == 0 && node.size > largest)
        {
            largest = node.size;
        }
    }
    return largest;
}

// Merges all adjacent free blocks in the memory pool.
void Memory::MergeAllAdjacentFreeBlocks()
{
    // A flag to track if a merge occurred during a full scan.
    bool merged = true;
    
    // Keep looping as long as merges are happening.
    while (merged)
    {
        // Assume no merge will happen in this iteration.
        merged = false;

        // _pool is assumed to be an std::map keyed by memory address,
        // which keeps the blocks sorted and makes finding adjacent blocks easy.
        for (auto it = _pool.begin(); it != _pool.end(); ++it)
        {
            // Check if the current block is free (status == 0).
            if (it->second.status == 0)
            {
                // Get an iterator to the next block in the pool.
                auto next_it = next(it);

                // [Core Condition] Check if:
                // 1. A next block exists.
                // 2. The next block is also free.
                // 3. The next block is physically adjacent to the current one.
                if (next_it != _pool.end() &&
                    next_it->second.status == 0 &&
                    it->first + it->second.size == next_it->first)
                {
                    // Calculate the combined size and store the starting address.
                    uint64_t new_size = it->second.size + next_it->second.size;
                    uint64_t addr = it->first;
                    
                    // Erase the two old, smaller blocks.
                    _pool.erase(it);
                    _pool.erase(next_it);
                    
                    // Create a new, merged block in the pool.
                    MemoryNode& newNode = _pool[addr];
                    newNode.addr = addr;
                    newNode.size = new_size;
                    newNode.status = 0; // Set its status to free.
                    
                    // A merge has occurred, so set the flag to true.
                    merged = true;

                    // Since the pool was modified, break the loop and restart the scan.
                    break;
                }
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const MemoryNode& node)
{
    os << hex << "[" << node.addr
        << ", " << node.size
        << ", " << node.status << "]";
    os << dec;
    return os;
}
std::ostream& operator<<(std::ostream& os, const Memory& memory)
{
    os << "      Memory @ " << hex << memory._start << " ~ " << memory._end
        << "(" << memory._data << " ~ " << memory._dataEnd << "), " << memory._size << ", cur " << memory._cur << ", ";
    for (const auto &pair : memory._pool)
    {
        os << pair.second << ", ";
    }
    os << dec;
    return os;
}

}  // namespace dxrt
