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
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include <mutex>
#include <unordered_map>
#include <map>
#include <queue>

namespace dxrt {

// Memory management configuration
namespace MemoryConfig {
    // Defragmentation thresholds
    constexpr double HIGH_FRAGMENTATION_THRESHOLD = 0.75;     // 75% - trigger aggressive defrag
    constexpr double MEDIUM_FRAGMENTATION_THRESHOLD = 0.5;   // 50% - trigger light defrag
    constexpr double LOW_FRAGMENTATION_THRESHOLD = 0.3;      // 30% - trigger warning and basic cleanup
    constexpr uint64_t LARGE_ALLOCATION_THRESHOLD = 100 * 1024 * 1024;  // 100MB - considered large
    
    // Memory alignment for better performance
    constexpr uint64_t MEMORY_ALIGNMENT = 64;  // 64-byte alignment for NPU efficiency
}

struct device_info;

struct DXRT_API MemoryNode
{
    uint64_t addr;
    uint64_t size;
    int status = 0; /* 0: available, 1: busy, 2: permanent */
    friend DXRT_API std::ostream& operator<<(std::ostream& os, const MemoryNode& node);
};

// Memory fragmentation info
struct DXRT_API MemoryFragmentationInfo
{
    uint64_t total_free_size;
    uint64_t largest_free_block;
    uint64_t smallest_free_block;
    size_t free_block_count;
    double fragmentation_ratio; // (total_free_size - largest_free_block) / total_free_size
};

class DXRT_API Memory
{
public:
    Memory(struct device_info &, void *);
    int64_t Allocate(uint64_t required);
    int64_t BackwardAllocate(uint64_t required);
    int64_t Allocate(dxrt_meminfo_t &meminfo);
    int64_t Allocate(dxrt_request_t &inference);
    void Deallocate(uint64_t addr);
    void Deallocate(dxrt_meminfo_t &meminfo);
    void Deallocate(dxrt_request_t &inference);
    void MergeAdjacentNodes(std::map<uint64_t, MemoryNode>::iterator it);
    void ResetBuffer();
    uint64_t start() const;
    uint64_t end() const;
    uint64_t size() const;
    uint64_t data() const;
    uint64_t free_size() const;
    uint64_t used_size() const;
    
    // Enhanced fragmentation management
    MemoryFragmentationInfo GetFragmentationInfo() const;
    bool CanAllocateContiguous(uint64_t required) const;
    void PrintMemoryMap() const;
    
    // Smart defragmentation functions
    bool TryDefragmentation(uint64_t required_size);
    void CompactMemory();
    uint64_t GetLargestFreeBlock() const;
    
    friend DXRT_API std::ostream& operator<<(std::ostream& os, const Memory& memory);
    
private:
    // Core memory management
    std::map<uint64_t, MemoryNode> _pool;
    uint64_t _start = 0;
    uint64_t _cur = 0;
    uint64_t _end = 0;
    uint64_t _size = 0;
    uint64_t _data;  // pointer to addr
    uint64_t _dataEnd;  // pointer to addr
    uint64_t _used_size = 0;
    std::mutex _lock;
    
    // Helper functions
    uint64_t AlignSize(uint64_t size) const;
    std::map<uint64_t, MemoryNode>::iterator FindBestFit(uint64_t required);
    void MergeAllAdjacentFreeBlocks();
};

} // namespace dxrt