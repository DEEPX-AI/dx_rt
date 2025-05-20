// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once


#include <map>
#include <set>
#include <vector>
#include <cstdint>
#include <memory>
#include <mutex>
#include "dxrt/common.h"
#include "dxrt/memory.h"

namespace dxrt {

class MemoryService
{
 public:
    static MemoryService* getInstance(int deviceId);
    uint64_t Allocate(uint64_t size, pid_t pid);
    uint64_t BackwardAllocate(uint64_t size, pid_t pid);
    bool Deallocate(uint64_t ptr, pid_t pid);
    void DeallocateAll(pid_t pid);
    static void DeallocateAllDevice(pid_t pid);
    uint64_t free_size() const;
    uint64_t used_size() const;
 private:
    MemoryService(uint64_t start, uint64_t size);
    dxrt::Memory* _mem;
    std::map<pid_t, std::set<uint64_t> > _allocInfo;
    static std::vector<MemoryService*> _instances;
    std::mutex _lock;
};

}  // namespace dxrt
