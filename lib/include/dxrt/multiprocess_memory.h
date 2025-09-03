/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once
#include <cstdint>
#include <mutex>
#include "dxrt/driver.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"

namespace dxrt {

enum class memoryRequestCode : int {
    REGISTESR_PROCESS = 0,  //set msg to pid
    GET_MEMORY = 1,  //set msg to size
    FREE_MEMORY = 2,  //set msg to value returned by GET_MEMORY

};
enum class memoryErrorCode : int {
    MEMORY_OK = 0,              //msg is allocated memory if GET_MEMORY, for REGISTER_PROCESS, it is start
    NOT_ENOUGH_MEMORY = 1,
    NOT_ALLOCATED = 2,
};

struct memoryMsg
{
    int code;
    int deviceId;
    int pid;
    uint64_t data;
};

struct memoryResult
{
    int code;
    int result;
    uint64_t data;
};


class MultiprocessMemory
{
public:
    explicit MultiprocessMemory();
    uint64_t Allocate(int deviceId, uint64_t required);
    uint64_t BackwardAllocate(int deviceId, uint64_t required);
    void Deallocate(int deviceId, uint64_t addr);
    void DeallocateAll(int deviceId);
    uint64_t start();
    uint64_t end();
    uint64_t size();
    uint64_t AllocateForTask(int deviceId, int taskId, uint64_t required);
    uint64_t BackwardAllocateForTask(int deviceId, int taskId, uint64_t required);
    void SignalScheduller(int deviceId, const dxrt_request_acc_t& req);
    void SignalEndJobs(int deviceId);
    void SignalDeviceInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum);
    void SignalDeviceDeInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum);
    void SignalDeviceReset(int deviceId);
    void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize);
    void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound);
    void DeallocateTaskMemory(int deviceId, int taskId);

private:
    void mpConnect();
    void mpConnect_once_wrapper();

    std::once_flag _connectFlag;

    // IPC
    dxrt::IPCClientWrapper ipcClientWrapper;
    dxrt::IPCClientWrapper ipcClientWrapperSync;


};

bool other_running(bool release);

}  // namespace dxrt