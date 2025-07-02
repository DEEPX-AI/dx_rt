// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include <mutex>
#include <unordered_map>
#include <map>

namespace dxrt {
struct device_info;

struct DXRT_API MemoryNode
{
    uint64_t addr;
    uint64_t size;
    int status = 0; /* 0: available, 1: busy, 2: permanent */
    friend DXRT_API std::ostream& operator<<(std::ostream& os, const MemoryNode& node);
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
    friend DXRT_API std::ostream& operator<<(std::ostream& os, const Memory& memory);
private:
    std::map<uint64_t, MemoryNode> _pool;
    uint64_t _start = 0;
    uint64_t _cur = 0;
    uint64_t _end = 0;
    uint64_t _size = 0;
    uint64_t _data;  // pointer to addr
    uint64_t _dataEnd;  // pointer to addr
    uint64_t _used_size = 0;
    std::mutex _lock;
};

} // namespace dxrt