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

#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

#include "dxrt/device_core.h"
#include "dxrt/driver.h"
#include "dxrt/exception/exception.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/memory.h"
#include "dxrt/service_util.h"

#ifdef __linux__
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace dxrt {

class DXRT_API ServiceLayerInterface {
public:
    virtual void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId) = 0;
    virtual void SignalDeviceReset(int id) = 0;
    virtual uint64_t Allocate(int deviceId, uint64_t size) = 0;
    virtual void DeAllocate(int deviceId, int64_t addr) = 0;
    virtual uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) = 0;
    virtual void SignalEndJobs(int id) = 0;
    virtual void CheckServiceRunning() = 0;
    virtual bool isRunOnService() const = 0;
    virtual void RegisterDeviceCore(DeviceCore *core) = 0;
    virtual void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) = 0;
    virtual void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) = 0;
};

class DXRT_API ServiceLayer : public ServiceLayerInterface {
public:
    explicit ServiceLayer(std::shared_ptr<MultiprocessMemory> mem);
    void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId) override;
    void SignalDeviceReset(int id) override;
    uint64_t Allocate(int deviceId, uint64_t size) override;
    uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) override;
    void DeAllocate(int deviceId, int64_t addr) override;
    void SignalEndJobs(int id) override;
    void CheckServiceRunning() override;
    bool isRunOnService() const override;
    void RegisterDeviceCore(DeviceCore *core) override;
    void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) override;
    void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
private:
    std::shared_ptr<MultiprocessMemory> _mem;
    std::mutex _lock;
};

class DXRT_API NoServiceLayer : public ServiceLayerInterface {
public:
    void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId) override;
    void SignalDeviceReset(int id) override;
    uint64_t Allocate(int deviceId, uint64_t size) override;
    uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required) override;
    void DeAllocate(int deviceId, int64_t addr) override;
    void SignalEndJobs(int id) override;
    void CheckServiceRunning() override;
    bool isRunOnService() const override;
    void RegisterDeviceCore(DeviceCore *core) override;
    void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize) override;
    void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
private:
    std::map<int, std::shared_ptr<Memory>> _mems;
    std::map<int, DeviceCore*> _ptr;
};

} // namespace dxrt
