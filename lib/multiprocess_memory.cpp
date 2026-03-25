/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/driver.h"
#include "dxrt/common.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/exception/exception.h"
#include "dxrt/runtime_event_dispatcher.h"
#include "dxrt/tsan_annotations.h"
#include "../resource/log_messages.h"

#ifdef __linux__
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <iostream>
#include <vector>

#include <chrono>
#include <thread>


// for debug
// #define LOG_DXRT_DBG std::cout

namespace dxrt
{

    MultiprocessMemory::MultiprocessMemory()
    : ipcClientWrapper(dxrt::IPCDefaultType(), getpid()),   // NOSONAR:S3230
      ipcClientWrapperSync(dxrt::IPCDefaultType(), getpid() + IPCClientWrapper::MAX_PID)    // NOSONAR:S3230
    {
    }

    int32_t callback(dxrt::IPCServerMessage& msg, void* ptr)
    {
        std::ignore = msg;
        std::ignore = ptr;
        return 0;
    }

    void MultiprocessMemory::mpConnect_internal()
    {
        // TSAN annotation: IPC thread creation synchronized by call_once
        ANNOTATE_HAPPENS_BEFORE(this);

        if ( ipcClientWrapper.Initialize() != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Async)"));

        if ( ipcClientWrapperSync.Initialize(false) != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Sync)"));

        ANNOTATE_HAPPENS_AFTER(this);
    }

    uint64_t MultiprocessMemory::Allocate(int deviceId, uint64_t required)
    {
        Connect();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {
            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        if (!isDone) {
            LOG_DXRT_ERR("Failed to allocate NPU memory " + std::to_string(required) + "byte after retries");
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_MEMORY,
                RuntimeEventDispatcher::CODE::MEMORY_OVERFLOW,
                LogMessages::RuntimeDispatch_RanOutOfNPUMemory());
        }

        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error");

        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::BackwardAllocate(int deviceId, uint64_t required)
    {
        Connect();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        DXRT_ASSERT(isDone, "allocateB timeout");
        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error");
        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::BackwardAllocateForTask(int deviceId, int taskId, uint64_t required)
    {
        Connect();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();
            clientMessage.taskId = taskId;

            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        DXRT_ASSERT(isDone, "allocateB timeout for Task " + std::to_string(taskId));
        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service for Task " << taskId << "\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error for Task " + std::to_string(taskId));
        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::AllocateForTask(int deviceId, int taskId, uint64_t required)
    {
        Connect();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();
            clientMessage.taskId = taskId;

            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        if (!isDone) {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_MEMORY,
                RuntimeEventDispatcher::CODE::MEMORY_OVERFLOW,
                LogMessages::RuntimeDispatch_RanOutOfNPUMemoryForTask(taskId));
        }

        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service for Task " << taskId << "\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error for Task " + std::to_string(taskId));
        return serverMessage.data;
    }

    void MultiprocessMemory::Deallocate(int deviceId, uint64_t addr)
    {
        Connect();
        dxrt::IPCClientMessage clientMessage;

        clientMessage.code = dxrt::REQUEST_CODE::FREE_MEMORY;
        clientMessage.deviceId = deviceId;
        clientMessage.data = addr;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::Connect()
    {
        std::call_once(_connectFlag, &MultiprocessMemory::mpConnect_internal, this);
    }

    void MultiprocessMemory::SignalScheduller(int deviceId, const dxrt_request_acc_t& req)  // NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.npu_acc = req;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalEndJobs(int deviceId)  // NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::INFERENCE_COMPLETED;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalDeviceInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum)  // NOSONAR:S5817 due to false positive
    {
        LOG_DXRT_DBG << "WARNING: SignalDeviceInit() is deprecated. Use SignalTaskInit() for proper Task-based initialization." << std::endl;

        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";
        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_INIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;
        clientMessage.npu_acc.datas[0] = weightOffset;
        clientMessage.npu_acc.datas[1] = weightSize;
        clientMessage.npu_acc.datas[2] = checksum;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalDeviceDeInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum)  // NOSONAR:S5817 due to false positive
    {
        LOG_DXRT_DBG << "WARNING: SignalDeviceDeInit() is deprecated. Use SignalTaskDeInit() for proper Task-based cleanup." << std::endl;

        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";
        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_DEINIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;
        clientMessage.npu_acc.datas[0] = weightOffset;
        clientMessage.npu_acc.datas[1] = weightSize;
        clientMessage.npu_acc.datas[2] = checksum;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalDeviceReset(int deviceId)  // NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_RESET;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize)  // NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << ", Task ID : " << taskId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::TASK_INIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;
        clientMessage.taskId = taskId;
        clientMessage.modelMemorySize = modelMemorySize;
        clientMessage.npu_acc.npu_id = 5353;

        dxrt::IPCServerMessage serverMessage;
        serverMessage.code = static_cast<dxrt::RESPONSE_CODE>(123456); // default to failed, will be overwritten by service response
        ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
        if (serverMessage.code != dxrt::RESPONSE_CODE::TASK_INIT_SUCCESS)
        {
            LOG_DXRT_ERR("Task initialization failed for Task " + std::to_string(taskId) + " on Device " + std::to_string(deviceId));
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to initialize task on device"));
        }
        return;
    }

    void MultiprocessMemory::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)// NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << ", Task ID : " << taskId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::TASK_DEINIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;
        clientMessage.taskId = taskId;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::DeallocateTaskMemory(int deviceId, int taskId)// NOSONAR:S5817 due to false positive
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << ", Task ID : " << taskId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::DEALLOCATE_TASK_MEMORY;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.taskId = taskId;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }
}  // namespace dxrt
