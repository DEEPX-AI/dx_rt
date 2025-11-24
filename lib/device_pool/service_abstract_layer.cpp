/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/service_abstract_layer.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/service_util.h"
#include "dxrt/exception/exception.h"

namespace dxrt
{

// ServiceLayer --------------------------------------------------
ServiceLayer::ServiceLayer(std::shared_ptr<MultiprocessMemory> mem) : _mem(std::move(mem)) {}
void ServiceLayer::HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->SignalScheduller(deviceId, acc);
}

void ServiceLayer::SignalDeviceReset(int id)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->SignalDeviceReset(id);
}

uint64_t ServiceLayer::Allocate(int deviceId, uint64_t size)
{
    std::lock_guard<std::mutex> lock(_lock);
    return _mem->Allocate(deviceId, size);
}

uint64_t ServiceLayer::BackwardAllocateForTask(int deviceId, int taskId, uint64_t required)
{
    std::lock_guard<std::mutex> lock(_lock);
    return _mem->BackwardAllocateForTask(deviceId, taskId, required);
}

void ServiceLayer::DeAllocate(int deviceId, int64_t addr)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->Deallocate(deviceId, addr);
}

void ServiceLayer::SignalEndJobs(int id)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->SignalEndJobs(id);
}

void ServiceLayer::CheckServiceRunning()
{
    if (!isDxrtServiceRunning())
    {
        throw dxrt::ServiceIOException("dxrt service is not running");
    }
}

bool ServiceLayer::isRunOnService() const { return true; }

void ServiceLayer::RegisterDeviceCore(DeviceCore *core) { std::ignore = core; }

void ServiceLayer::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->SignalTaskInit(deviceId, taskId, bound, modelMemorySize);
}

void ServiceLayer::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
{
    std::lock_guard<std::mutex> lock(_lock);
    _mem->SignalTaskDeInit(deviceId, taskId, bound);

    _mem->DeallocateTaskMemory(deviceId, taskId);
}

extern uint8_t DEBUG_DATA;
// NoServiceLayer ------------------------------------------------


#ifdef __linux__
    constexpr static int HandleInferenceAcc_BUSY_VALUE = -EBUSY;  // write done, but failed to enqueue
#elif _WIN32
    constexpr static int HandleInferenceAcc_BUSY_VALUE = ERROR_BUSY;
#endif

void NoServiceLayer::HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId)
{
    LOG_DXRT_DBG << "NoServiceLayer::HandleInferenceAcc deviceId=" << deviceId << " acc=" << acc << std::endl;
    DeviceCore *core = _ptr[deviceId];
    dxrt_request_acc_t acc_cp = acc;
    int ret = -1;
    do
    {
        ret = core->Process(DXRT_CMD_NPU_RUN_REQ, &acc_cp);

        if (ret == HandleInferenceAcc_BUSY_VALUE)
        {
            acc_cp.input.data = 0;
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        // if stoppes, return required;
    } while (ret != 0);
}

void NoServiceLayer::RegisterDeviceCore(DeviceCore* core)
{
    int id = core->id();
    _ptr[id] = core;
    dxrt_device_info_t info = core->info();
    _mems.emplace(id, std::make_shared<Memory>(info, nullptr));
}

void NoServiceLayer::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize)
{
    std::ignore = taskId;
    std::ignore = modelMemorySize;
    _ptr[deviceId]->BoundOption(DX_SCHED_ADD, bound);
}
void NoServiceLayer::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
{
    std::ignore = taskId;
    _ptr[deviceId]->BoundOption(DX_SCHED_DELETE, bound);
}



void NoServiceLayer::SignalDeviceReset(int id) { std::ignore = id; }

uint64_t NoServiceLayer::Allocate(int deviceId, uint64_t size) { return _mems[deviceId]->Allocate(size); }

uint64_t NoServiceLayer::BackwardAllocateForTask(int deviceId, int taskId, uint64_t required)
{
    std::ignore = taskId;
    return _mems[deviceId]->BackwardAllocate(required);
}

void NoServiceLayer::DeAllocate(int deviceId, int64_t addr) { _mems[deviceId]->Deallocate(addr); }

void NoServiceLayer::SignalEndJobs(int id) { std::ignore = id; }

void NoServiceLayer::CheckServiceRunning() {}

bool NoServiceLayer::isRunOnService() const { return false; }
}  // namespace dxrt
