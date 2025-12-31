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


#define SOCKET_NAME "/tmp/dxrt_memory_socket"

// for debug
// #define LOG_DXRT_DBG std::cout

namespace dxrt
{

    MultiprocessMemory::MultiprocessMemory()
    : ipcClientWrapper(dxrt::IPCDefaultType(), getpid()),
    ipcClientWrapperSync(dxrt::IPCDefaultType(), getpid() + IPCClientWrapper::MAX_PID)
    {
    }

    int32_t callback(dxrt::IPCServerMessage& msg, void* ptr)
    {
        std::ignore = msg;
        std::ignore = ptr;
        return 0;
    }

    void MultiprocessMemory::mpConnect()
    {
        // DXRT_ASSERT(ipcClientWrapper.Initialize() == 0, "Failed to connect to dxrt memory manager service (IPC/Async)");
        if ( ipcClientWrapper.Initialize() != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Async)"));

        // DXRT_ASSERT(ipcClientWrapperSync.Initialize(false) == 0, "Failed to connect to dxrt memory manager service (IPC/Sync)");
        if ( ipcClientWrapperSync.Initialize(false) != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Sync)"));
    }

    uint64_t MultiprocessMemory::Allocate(int deviceId, uint64_t required)
    {
        mpConnect_once_wrapper();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {
            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            // ipcClientWrapper.SendToServer(serverMessage, clientMessage);
            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        //DXRT_ASSERT(isDone, "ran out of NPU memory");
        if (!isDone) {
            LOG_DXRT_ERR("Failed to allocate NPU memory " + std::to_string(required) + "byte after retries");
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_MEMORY,
                RuntimeEventDispatcher::CODE::MEMORY_OVERFLOW,
                LogMessages::RuntimeDispatch_RanOutOfNPUMemory());
        }
        /* TODO
        // Fix: Return -1 instead of aborting to allow retry with smaller buffer count
        if (!isDone) {
            LOG_DXRT_ERR("Failed to allocate NPU memory after retries");
            return static_cast<uint64_t>(-1);
        }
        */

        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error");
        // DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != 0,"allocate error");
        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::BackwardAllocate(int deviceId, uint64_t required)
    {
        mpConnect_once_wrapper();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for (int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            // ipcClientWrapper.SendToServer(serverMessage, clientMessage);
            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(2));
        //#ifdef __linux__
        //        sleep(5);
        //#elif _WIN32
        //        this_thread::sleep_for(chrono::microseconds(50));
        //#endif
        }
        DXRT_ASSERT(isDone, "allocateB timeout");
        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error");
        // DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != 0,"allocate error");
        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::BackwardAllocateForTask(int deviceId, int taskId, uint64_t required)
    {
        mpConnect_once_wrapper();
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

            //ipcClientWrapper.SendToServer(serverMessage, clientMessage);
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
        mpConnect_once_wrapper();
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

            // ipcClientWrapper.SendToServer(serverMessage, clientMessage);
            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }

        // DXRT_ASSERT(isDone, "ran out of NPU memory for Task " + std::to_string(taskId));
        if (!isDone) {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_MEMORY,
                RuntimeEventDispatcher::CODE::MEMORY_OVERFLOW,
                LogMessages::RuntimeDispatch_RanOutOfNPUMemoryForTask(taskId));
        }
        /* TODO
        // Fix: Return -1 instead of aborting to allow retry with smaller buffer count
        if (!isDone) {
            LOG_DXRT_ERR("Failed to allocate NPU memory for Task " + std::to_string(taskId) + " after retries");
            return static_cast<uint64_t>(-1);
        }
        */

        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service for Task " << taskId << "\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1, "allocate error for Task " + std::to_string(taskId));
        return serverMessage.data;
    }

    void MultiprocessMemory::Deallocate(int deviceId, uint64_t addr)
    {
        mpConnect_once_wrapper();
        dxrt::IPCClientMessage clientMessage;

        clientMessage.code = dxrt::REQUEST_CODE::FREE_MEMORY;
        clientMessage.deviceId = deviceId;
        clientMessage.data = addr;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::mpConnect_once_wrapper()
    {
        std::call_once(_connectFlag, &MultiprocessMemory::mpConnect, this);
    }

    void MultiprocessMemory::SignalScheduller(int deviceId, const dxrt_request_acc_t& req)
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

    void MultiprocessMemory::SignalEndJobs(int deviceId)
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::INFERENCE_COMPLETED;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalDeviceInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum)
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

    void MultiprocessMemory::SignalDeviceDeInit(int deviceId, npu_bound_op bound, int weightSize, int weightOffset, uint32_t checksum)
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

    void MultiprocessMemory::SignalDeviceReset(int deviceId)
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_RESET;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize)
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << ", Task ID : " << taskId << "\n";

        clientMessage.code = dxrt::REQUEST_CODE::TASK_INIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;
        clientMessage.taskId = taskId;
        clientMessage.modelMemorySize = modelMemorySize;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
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

    void MultiprocessMemory::DeallocateTaskMemory(int deviceId, int taskId)
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
