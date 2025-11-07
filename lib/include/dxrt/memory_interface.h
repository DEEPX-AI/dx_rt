/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once
// Minimal abstract interface to decouple concrete memory implementations.
#include <cstdint>

#include "dxrt/driver.h"

namespace dxrt {

class MemoryInterface {
 public:
    virtual ~MemoryInterface() = default;
    virtual uint64_t Allocate(int deviceId, uint64_t required) = 0;
    virtual uint64_t BackwardAllocate(int deviceId, uint64_t required) = 0;
    virtual void Deallocate(int deviceId, uint64_t addr) = 0;
    virtual void DeallocateAll(int deviceId) = 0;
    virtual uint64_t start() = 0;
    virtual uint64_t end() = 0;
    virtual uint64_t size() = 0;
    virtual uint64_t AllocateForTask(int deviceId, int taskId, uint64_t required) = 0;
    virtual uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) = 0;
    virtual void SignalScheduller(int deviceId, const dxrt_request_acc_t &req) = 0;
    virtual void SignalEndJobs(int deviceId) = 0;
    virtual void SignalDeviceReset(int deviceId) = 0;
    virtual void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) = 0;
    virtual void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) = 0;
    virtual void DeallocateTaskMemory(int deviceId, int taskId) = 0;
};

}  // namespace dxrt
