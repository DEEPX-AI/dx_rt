/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once


#include <map>
#include <set>
#include <vector>
#include <cstdint>
//#include <memory>
#include <mutex>
//#include "dxrt/common.h"
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

    uint64_t AllocateForTask(uint64_t size, pid_t pid, int taskId);
    uint64_t BackwardAllocateForTask(uint64_t size, pid_t pid, int taskId);
    bool DeallocateTask(pid_t pid, int taskId);
    void DeallocateAllTasks(pid_t pid);
    bool DeallocateAllForProcess(pid_t pid);


    bool IsTaskValid(pid_t pid, int taskId) const;

    void OptimizeMemory();
    void PrintMemoryStatus() const;
    int deviceId() const { return _deviceId; }

private:
    MemoryService(uint64_t start, uint64_t size);
    dxrt::Memory* _mem;

    // std::map<pid_t, std::set<uint64_t> > _allocInfo;
    std::map<pid_t, std::set<uint64_t> > _legacyAllocInfo;

    std::map<pid_t, std::map<int, std::set<uint64_t>>> _taskAllocInfo;

    static std::vector<MemoryService*> _instances;
    mutable std::mutex _lock;

    int _deviceId;
};

}  // namespace dxrt
