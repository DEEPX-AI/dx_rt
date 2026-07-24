/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#define NOMINMAX

#include "dxrt/multiprocess_memory.h"

#include <cstdint>
#include <limits>

#include "dxrt/exception/exception.h"
#include "dxrt/tsan_annotations.h"

namespace dxrt
{
namespace {

uint64_t LegacyIpcUnavailableAddress()
{
    return (std::numeric_limits<uint64_t>::max)();
}

void ThrowLegacyIpcDisabled()
{
    throw ServiceIOException(EXCEPTION_MESSAGE(
        "MultiprocessMemory legacy IPC is disabled. Use ServiceLayer Dynamic IPC."));
}

}  // namespace

MultiprocessMemory::MultiprocessMemory() = default;

void MultiprocessMemory::mpConnect_internal()
{
    ANNOTATE_HAPPENS_BEFORE(this);
    ANNOTATE_HAPPENS_AFTER(this);
}

uint64_t MultiprocessMemory::Allocate(int deviceId, uint64_t required)
{
    (void)deviceId;
    (void)required;
    Connect();
    return LegacyIpcUnavailableAddress();
}

uint64_t MultiprocessMemory::BackwardAllocate(int deviceId, uint64_t required)
{
    (void)deviceId;
    (void)required;
    Connect();
    return LegacyIpcUnavailableAddress();
}

uint64_t MultiprocessMemory::BackwardAllocateForTask(int deviceId, int taskId, uint64_t required)
{
    (void)deviceId;
    (void)taskId;
    (void)required;
    Connect();
    return LegacyIpcUnavailableAddress();
}

uint64_t MultiprocessMemory::AllocateForTask(int deviceId, int taskId, uint64_t required)
{
    (void)deviceId;
    (void)taskId;
    (void)required;
    Connect();
    return LegacyIpcUnavailableAddress();
}

void MultiprocessMemory::Deallocate(int deviceId, uint64_t addr)
{
    (void)deviceId;
    (void)addr;
    Connect();
}

void MultiprocessMemory::Connect()
{
    std::call_once(_connectFlag, &MultiprocessMemory::mpConnect_internal, this);
}

void MultiprocessMemory::SignalScheduller(int deviceId, const dxrt_request_acc_t& req)
{
    (void)deviceId;
    (void)req;
    ThrowLegacyIpcDisabled();
}

void MultiprocessMemory::SignalEndJobs(int deviceId)
{
    (void)deviceId;
}

void MultiprocessMemory::SignalDeviceInit(
    int deviceId,
    npu_bound_op bound,
    int weightSize,
    int weightOffset,
    uint32_t checksum)
{
    (void)deviceId;
    (void)bound;
    (void)weightSize;
    (void)weightOffset;
    (void)checksum;
    ThrowLegacyIpcDisabled();
}

void MultiprocessMemory::SignalDeviceDeInit(
    int deviceId,
    npu_bound_op bound,
    int weightSize,
    int weightOffset,
    uint32_t checksum)
{
    (void)deviceId;
    (void)bound;
    (void)weightSize;
    (void)weightOffset;
    (void)checksum;
    ThrowLegacyIpcDisabled();
}

void MultiprocessMemory::SignalDeviceReset(int deviceId)
{
    (void)deviceId;
}

void MultiprocessMemory::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize)
{
    (void)deviceId;
    (void)taskId;
    (void)bound;
    (void)modelMemorySize;
    ThrowLegacyIpcDisabled();
}

void MultiprocessMemory::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
{
    (void)deviceId;
    (void)taskId;
    (void)bound;
    ThrowLegacyIpcDisabled();
}

void MultiprocessMemory::DeallocateTaskMemory(int deviceId, int taskId)
{
    (void)deviceId;
    (void)taskId;
}

}  // namespace dxrt
