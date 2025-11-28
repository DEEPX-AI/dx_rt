/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <set>
#include <map>
#include <iostream>
#include "dxrt/common.h"
//#include "dxrt/ipc_wrapper/ipc_message.h"
//#include "dxrt/ipc_wrapper/ipc_server_wrapper.h"
//#include "dxrt/ipc_wrapper/ipc_server.h"
#include "dxrt/memory.h"
#include "dxrt/device.h"
#include "memory_service.hpp"
#include "service_device.h"

using std::endl;
using std::hex;
using std::dec;

namespace dxrt {

std::vector<MemoryService*> MemoryService::_instances;

static constexpr bool ENABLE_MEMORY_TRACE_LOGS = false;

MemoryService* MemoryService::getInstance(int deviceId)
{
    if (_instances.empty())
    {
        auto device_list = ServiceDevice::CheckServiceDevices();
        if (device_list.size() < 1)
        {
            LOG_DXRT_S_DBG << "no device detected" << errno << endl;
        }
        for (auto device : device_list)
        {
            dxrt_device_info_t info = device->info();
            if (info.mem_size < 1024*1024)
            {
                LOG_DXRT_S_ERR("device " << device->id() << " memory size info get error:" << info.mem_size);
                DXRT_ASSERT(false, "device memory size info error");
            }
            _instances.push_back(new MemoryService(info.mem_addr, info.mem_size));
            LOG_DXRT_S_DBG << "device insert:" << device->id() << endl;
            _instances.back()->_deviceId = device->id();
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
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Requesting allocation of size " << size << " for PID " << pid << endl;
    }
    std::lock_guard<std::mutex> lk(_lock);

    // Quick pre-check for large allocations only (>100MB)
    if (size > MemoryConfig::LARGE_ALLOCATION_THRESHOLD && !_mem->CanAllocateContiguous(size)) {
        auto fragInfo = _mem->GetFragmentationInfo();
        LOG_DXRT_S_ERR("Cannot allocate " + std::to_string(size / (1024*1024)) + "MB for PID " + std::to_string(pid) +
                       " - Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                       "Largest: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB");
        return static_cast<uint64_t>(-1);
    }

    uint64_t addr = _mem->Allocate(size);
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Allocated address " << hex << addr << dec << " of size " << size << " for PID " << pid << endl;
    }

    if (addr != static_cast<uint64_t>(-1)) {
        // Track legacy allocations for backwards compatibility
        _legacyAllocInfo[pid].insert(addr);
        LOG_DXRT_S_DBG << hex << addr << dec  << " is allocated (legacy), size:" << size << endl;
    } else {
        LOG_DXRT_S_ERR("Memory allocation failed for PID " + std::to_string(pid) + ", size " + std::to_string(size));
    }

    return addr;
}

uint64_t MemoryService::BackwardAllocate(uint64_t size, pid_t pid)
{
    std::lock_guard<std::mutex> lk(_lock);

    // Quick pre-check for large allocations only (>100MB)
    if (size > MemoryConfig::LARGE_ALLOCATION_THRESHOLD && !_mem->CanAllocateContiguous(size)) {
        auto fragInfo = _mem->GetFragmentationInfo();
        LOG_DXRT_S_ERR("Cannot backward allocate " + std::to_string(size / (1024*1024)) + "MB for PID " + std::to_string(pid) +
                       " - Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                       "Largest: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB");
        return static_cast<uint64_t>(-1);
    }

    uint64_t addr = _mem->BackwardAllocate(size);

    if (addr != static_cast<uint64_t>(-1)) {
        // Track legacy allocations for backwards compatibility
        _legacyAllocInfo[pid].insert(addr);
        LOG_DXRT_S_DBG << hex << addr << dec  << " is allocated (legacy backward), size:" << size << endl;
    } else {
        LOG_DXRT_S_ERR("Backward memory allocation failed for PID " + std::to_string(pid) + ", size " + std::to_string(size));
    }

    return addr;
}

bool MemoryService::Deallocate(uint64_t addr, pid_t pid)
{
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Requesting deallocation of address " << hex << addr << dec << " for PID " << pid << endl;
    }

    std::lock_guard<std::mutex> lk(_lock);
    auto it1 = _legacyAllocInfo.find(pid);
    if (it1 == _legacyAllocInfo.end())
    {
        LOG_DXRT_S_DBG << "not regestered pid " << pid << " (legacy)" << endl;
        return false;
    }
    auto it2 = it1->second.find(addr);
    if (it2 == it1->second.end())
    {
        LOG_DXRT_S_DBG << "not allocated addr " << hex << addr << dec  << " for pid " << pid << " (legacy)" << endl;
        return false;
    }
    it1->second.erase(it2);
    _mem->Deallocate(addr);
    LOG_DXRT_S_DBG << hex << addr << dec << " is Deallocated (legacy)"<< " for pid " << pid << endl;

    return true;
}

void MemoryService::DeallocateAll(pid_t pid)
{
    std::lock_guard<std::mutex> lk(_lock);

    // Deallocate task-based memory
    auto taskIt = _taskAllocInfo.find(pid);
    if (taskIt != _taskAllocInfo.end()) {
        for (const auto& taskPair : taskIt->second) {
            int taskId = taskPair.first;
            for (auto addr : taskPair.second) {
                _mem->Deallocate(addr);
                LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated for Task " << taskId
                               << ", PID:" << pid << " (cleanup)" << endl;
            }
        }
        _taskAllocInfo.erase(taskIt);
        LOG_DXRT_S_DBG << "All task-based memory deallocated for PID:" << pid << endl;
    }

    // Deallocate legacy PID-based memory
    auto legacyIt = _legacyAllocInfo.find(pid);
    if (legacyIt != _legacyAllocInfo.end()) {
        for (auto addr : legacyIt->second) {
            _mem->Deallocate(addr);
            LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated (legacy cleanup) for PID:" << pid << endl;
        }
        _legacyAllocInfo.erase(legacyIt);
        LOG_DXRT_S_DBG << "All legacy memory deallocated for PID:" << pid << endl;
    }
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

uint64_t MemoryService::AllocateForTask(uint64_t size, pid_t pid, int taskId)
{
    std::lock_guard<std::mutex> lk(_lock);

    // Enhanced logging for memory allocation
    auto fragInfo = _mem->GetFragmentationInfo();
    LOG_DXRT_S_DBG << "AllocateForTask - Task " << taskId << ", PID " << pid
                   << ", Size: " << (size / (1024*1024)) << "MB" << endl;
    LOG_DXRT_S_DBG << "Memory state before allocation - Free: " << (fragInfo.total_free_size / (1024*1024)) << "MB, "
                   << "Used: " << (_mem->used_size() / (1024*1024)) << "MB, "
                   << "Largest block: " << (fragInfo.largest_free_block / (1024*1024)) << "MB, "
                   << "Fragmentation: " << (fragInfo.fragmentation_ratio * 100.0) << "%" << endl;

    // Quick pre-check for large allocations only (>100MB)
    if (size > MemoryConfig::LARGE_ALLOCATION_THRESHOLD && !_mem->CanAllocateContiguous(size)) {
        LOG_DXRT_S_ERR("Cannot allocate " + std::to_string(size / (1024*1024)) + "MB for Task " + std::to_string(taskId) +
                       " - Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                       "Largest: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB");
        return static_cast<uint64_t>(-1);
    }

    uint64_t addr = _mem->Allocate(size);

    if (addr != static_cast<uint64_t>(-1)) {
        // Use task-based tracking only (remove double tracking)
        _taskAllocInfo[pid][taskId].insert(addr);

        LOG_DXRT_S_DBG << hex << addr << dec << " is allocated for Task " << taskId
                       << ", size:" << (size / (1024*1024)) << "MB, PID:" << pid << endl;

        // Log updated memory state
        auto newFragInfo = _mem->GetFragmentationInfo();
        LOG_DXRT_S_DBG << "Memory state after allocation - Free: " << (newFragInfo.total_free_size / (1024*1024)) << "MB, "
                       << "Used: " << (_mem->used_size() / (1024*1024)) << "MB" << endl;
    } else {
        LOG_DXRT_S_ERR("Memory allocation failed for Task " + std::to_string(taskId) +
                       ", PID " + std::to_string(pid) + ", size " + std::to_string(size / (1024*1024)) + "MB");

        // Try to provide helpful diagnosis
        if (fragInfo.fragmentation_ratio > MemoryConfig::LOW_FRAGMENTATION_THRESHOLD) {
            LOG_DXRT_S_ERR("High memory fragmentation detected (" + std::to_string(fragInfo.fragmentation_ratio * 100.0) +
                           "%), consider memory optimization");
        }
    }
    return addr;
}

uint64_t MemoryService::BackwardAllocateForTask(uint64_t size, pid_t pid, int taskId)
{
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Requesting backward allocation of size " << size << " for Task " << taskId << ", PID " << pid << endl;
    }
    std::lock_guard<std::mutex> lk(_lock);

    // Enhanced logging for backward memory allocation
    auto fragInfo = _mem->GetFragmentationInfo();
    LOG_DXRT_S_DBG << "BackwardAllocateForTask - Task " << taskId << ", PID " << pid
                   << ", Size: " << (size / (1024*1024)) << "MB" << endl;
    LOG_DXRT_S_DBG << "Memory state before backward allocation - Free: " << (fragInfo.total_free_size / (1024*1024)) << "MB, "
                   << "Used: " << (_mem->used_size() / (1024*1024)) << "MB, "
                   << "Largest block: " << (fragInfo.largest_free_block / (1024*1024)) << "MB, "
                   << "Fragmentation: " << (fragInfo.fragmentation_ratio * 100.0) << "%" << endl;

    // Quick pre-check for large allocations only (>100MB)
    if (size > MemoryConfig::LARGE_ALLOCATION_THRESHOLD && !_mem->CanAllocateContiguous(size)) {
        LOG_DXRT_S_ERR("Cannot backward allocate " + std::to_string(size / (1024*1024)) + "MB for Task " + std::to_string(taskId) +
                       " - Free: " + std::to_string(fragInfo.total_free_size / (1024*1024)) + "MB, " +
                       "Largest: " + std::to_string(fragInfo.largest_free_block / (1024*1024)) + "MB");
        return static_cast<uint64_t>(-1);
    }

    uint64_t addr = _mem->BackwardAllocate(size);
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Backward allocated address " << hex << addr << dec << " of size " << size << " for Task " << taskId << ", PID " << pid << endl;
    }

    if (addr != static_cast<uint64_t>(-1)) {
        // Use task-based tracking only (remove double tracking)
        _taskAllocInfo[pid][taskId].insert(addr);

        LOG_DXRT_S_DBG << hex << addr << dec << " is backward allocated for Task " << taskId
                       << ", size:" << (size / (1024*1024)) << "MB, PID:" << pid << endl;

        // Log updated memory state
        auto newFragInfo = _mem->GetFragmentationInfo();
        LOG_DXRT_S_DBG << "Memory state after backward allocation - Free: " << (newFragInfo.total_free_size / (1024*1024)) << "MB, "
                       << "Used: " << (_mem->used_size() / (1024*1024)) << "MB" << endl;
    } else {
        LOG_DXRT_S_ERR("Backward memory allocation failed for Task " + std::to_string(taskId) +
                       ", PID " + std::to_string(pid) + ", size " + std::to_string(size / (1024*1024)) + "MB");

        // Try to provide helpful diagnosis
        if (fragInfo.fragmentation_ratio > MemoryConfig::LOW_FRAGMENTATION_THRESHOLD) {
            LOG_DXRT_S_ERR("High memory fragmentation detected (" + std::to_string(fragInfo.fragmentation_ratio * 100.0) +
                           "%), consider memory optimization");
        }
    }

    return addr;
}

bool MemoryService::DeallocateTask(pid_t pid, int taskId)
{
    if (ENABLE_MEMORY_TRACE_LOGS) {
        LOG_DXRT_S << "Requesting deallocation of memory for Task " << taskId << ", PID " << pid << endl;
    }
    std::lock_guard<std::mutex> lk(_lock);

    auto pidIt = _taskAllocInfo.find(pid);
    if (pidIt == _taskAllocInfo.end())
    {
        LOG_DXRT_S_DBG << "Task deallocation: PID " << pid << " not found" << endl;
        return false;
    }

    auto taskIt = pidIt->second.find(taskId);
    if (taskIt == pidIt->second.end())
    {
        LOG_DXRT_S_DBG << "Task deallocation: Task " << taskId << " not found for PID " << pid << endl;
        return false;
    }

    // Enhanced safety check for concurrent deallocation
    if (taskIt->second.empty()) {
        LOG_DXRT_S_DBG << "Task " << taskId << " already has no memory allocations" << endl;
        pidIt->second.erase(taskIt);

        // If the PID has no more tasks, remove the PID entry itself
        if (pidIt->second.empty()) {
            _taskAllocInfo.erase(pidIt);
            LOG_DXRT_S_DBG << "Removed PID " << pid << " from task allocation info (no more tasks)" << endl;
        }
        return true;
    }

    // Check if task is still being used (additional safety check)
    LOG_DXRT_S_DBG << "Deallocating " << taskIt->second.size() << " memory blocks for Task " << taskId << ", PID:" << pid << endl;

    // Deallocate all memory addresses for the task
    std::vector<uint64_t> addressesToDeallocate(taskIt->second.begin(), taskIt->second.end());

    // Clear the task's memory set first to prevent race conditions
    taskIt->second.clear();

    // Now deallocate the memory addresses
    for (auto addr : addressesToDeallocate)
    {
        if (addr != static_cast<uint64_t>(-1)) {  // Check for invalid address
            _mem->Deallocate(addr);

            LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated for Task " << taskId
                           << ", PID:" << pid << endl;
        } else {
            LOG_DXRT_S_ERR("Invalid memory address found for Task " + std::to_string(taskId) +
                           ", PID: " + std::to_string(pid));
        }
    }

    // Remove task metadata
    pidIt->second.erase(taskIt);

    // If the PID has no more tasks, remove the PID entry itself
    if (pidIt->second.empty())
    {
        _taskAllocInfo.erase(pidIt);
        LOG_DXRT_S_DBG << "Removed PID " << pid << " from task allocation info (no more tasks)" << endl;
    }

    LOG_DXRT_S_DBG << "All memory deallocated for Task " << taskId << ", PID:" << pid << endl;
    return true;
}

void MemoryService::DeallocateAllTasks(pid_t pid)
{
    std::lock_guard<std::mutex> lk(_lock);

    auto pidIt = _taskAllocInfo.find(pid);
    if (pidIt == _taskAllocInfo.end())
    {
        LOG_DXRT_S_DBG << "DeallocateAllTasks: PID " << pid << " not found" << endl;
        return;
    }

    for (const auto& taskPair : pidIt->second)
    {
        int taskId = taskPair.first;
        for (auto addr : taskPair.second)
        {
            _mem->Deallocate(addr);
            LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated for Task " << taskId
                           << ", PID:" << pid << endl;
        }
    }

    _taskAllocInfo.erase(pidIt);

    LOG_DXRT_S_DBG << "All tasks memory deallocated for PID:" << pid << endl;
}

bool MemoryService::IsTaskValid(pid_t pid, int taskId) const
{
    std::lock_guard<std::mutex> lk(_lock);

    auto pidIt = _taskAllocInfo.find(pid);
    if (pidIt == _taskAllocInfo.end()) {

        LOG_DXRT_S_ERR("Process " << pid << " device " << _deviceId << " task " << taskId << ": not found in MemoryService TaskAllocInfo");
        return false;
    }

    if (pidIt->second.find(taskId) == pidIt->second.end())
    {
        LOG_DXRT_S_ERR("Process " << pid << " device " << _deviceId << " task " << taskId << ": not found in MemoryService pidIt");
    }
    return true;
}

// Memory fragmentation prevention and cleanup function
void MemoryService::OptimizeMemory()
{
    std::lock_guard<std::mutex> lk(_lock);

    auto fragInfo = _mem->GetFragmentationInfo();

    // Perform cleanup if fragmentation is severe
    if (fragInfo.fragmentation_ratio > MemoryConfig::LOW_FRAGMENTATION_THRESHOLD) {  // 30% or more fragmentation
        LOG_DXRT_S_DBG << "Memory fragmentation detected: "
                       << (fragInfo.fragmentation_ratio * 100.0) << "%" << endl;

        _mem->CompactMemory();

        auto newFragInfo = _mem->GetFragmentationInfo();
        LOG_DXRT_S_DBG << "Memory optimization completed. New fragmentation: "
                       << (newFragInfo.fragmentation_ratio * 100.0) << "%" << endl;
    }
}

// Memory status diagnostic function
void MemoryService::PrintMemoryStatus() const
{
    std::lock_guard<std::mutex> lk(_lock);

    _mem->PrintMemoryMap();

    LOG_DXRT_S << "Task allocation summary:" << endl;
    for (const auto& pidPair : _taskAllocInfo) {
        pid_t pid = pidPair.first;
        size_t totalAllocations = 0;
        for (const auto& taskPair : pidPair.second) {
            totalAllocations += taskPair.second.size();
        }
        LOG_DXRT_S << "  PID " << pid << ": " << pidPair.second.size()
                   << " tasks, " << totalAllocations << " allocations" << endl;
    }
}

bool MemoryService::DeallocateAllForProcess(pid_t pid)
{
    std::lock_guard<std::mutex> lk(_lock);

    bool taskMemoryFound = false;
    bool legacyMemoryFound = false;

    // Deallocate task-based memory
    auto taskIt = _taskAllocInfo.find(pid);
    if (taskIt != _taskAllocInfo.end()) {
        taskMemoryFound = true;
        for (auto& taskPair : taskIt->second) {
            int taskId = taskPair.first;
            for (auto addr : taskPair.second) {
                _mem->Deallocate(addr);
                LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated for Task " << taskId
                               << ", PID:" << pid << " (process cleanup)" << endl;
            }
        }
        _taskAllocInfo.erase(taskIt);
        LOG_DXRT_S_DBG << "All task-based memory deallocated for PID:" << pid << endl;
    }

    // Deallocate legacy PID-based memory
    auto legacyIt = _legacyAllocInfo.find(pid);
    if (legacyIt != _legacyAllocInfo.end()) {
        legacyMemoryFound = true;
        for (auto addr : legacyIt->second) {
            _mem->Deallocate(addr);
            LOG_DXRT_S_DBG << hex << addr << dec << " is deallocated (legacy process cleanup) for PID:" << pid << endl;
        }
        _legacyAllocInfo.erase(legacyIt);
        LOG_DXRT_S_DBG << "All legacy memory deallocated for PID:" << pid << endl;
    }

    return (taskMemoryFound || legacyMemoryFound);
}


}  // namespace dxrt
