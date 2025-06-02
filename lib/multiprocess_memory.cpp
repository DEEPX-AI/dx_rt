// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.


#include "dxrt/driver.h"
#include "dxrt/common.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/exception/exception.h"

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

using namespace std;

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
        //DXRT_ASSERT(ipcClientWrapper.Initialize() == 0, "Failed to connect to dxrt memory manager service (IPC/Async)");
        if ( ipcClientWrapper.Initialize() != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Async)"));

        //DXRT_ASSERT(ipcClientWrapperSync.Initialize(false) == 0, "Failed to connect to dxrt memory manager service (IPC/Sync)");
        if ( ipcClientWrapperSync.Initialize(false) != 0 )
            throw ServiceIOException(EXCEPTION_MESSAGE("Failed to connect to dxrt memory manager service (IPC/Sync)"));
    }

    uint64_t MultiprocessMemory::Allocate(int deviceId, uint64_t required)
    {
        mpConnect_once_wrapper();
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for(int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            //ipcClientWrapper.SendToServer(serverMessage, clientMessage);
            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }
            std::this_thread::sleep_for(chrono::seconds(2));
        }
        DXRT_ASSERT(isDone,"ran out of NPU memory");
        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1,"allocate error");
        //DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != 0,"allocate error");
        return serverMessage.data;
    }

    uint64_t MultiprocessMemory::BackwardAllocate(int deviceId, uint64_t required)
    {
        mpConnect_once_wrapper();   
        dxrt::IPCClientMessage clientMessage;
        dxrt::IPCServerMessage serverMessage;
        bool isDone = false;
        for(int i = 0; i < 20; i++)
        {

            clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL;
            clientMessage.deviceId = deviceId;
            clientMessage.data = required;
            clientMessage.pid = getpid();


            //ipcClientWrapper.SendToServer(serverMessage, clientMessage);
            ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
            if (serverMessage.result == 0)
            {
                isDone = true;
                break;
            }

            std::this_thread::sleep_for(chrono::seconds(2));
        //#ifdef __linux__
        //        sleep(5);
        //#elif _WIN32
        //        this_thread::sleep_for(chrono::microseconds(50));
        //#endif
        }
        DXRT_ASSERT(isDone,"allocateB timeout");
        LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
        DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != -1,"allocate error");
        //DXRT_ASSERT(static_cast<int64_t>(serverMessage.data) != 0,"allocate error");
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

    void MultiprocessMemory::SignalDeviceInit(int deviceId, npu_bound_op bound)
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";
        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_INIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;

        ipcClientWrapper.SendToServer(clientMessage);
        return;
    }

    void MultiprocessMemory::SignalDeviceDeInit(int deviceId, npu_bound_op bound)
    {
        dxrt::IPCClientMessage clientMessage;
        LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";
        clientMessage.code = dxrt::REQUEST_CODE::DEVICE_DEINIT;
        clientMessage.deviceId = deviceId;
        clientMessage.pid = getpid();
        clientMessage.data = bound;

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
}  // namespace dxrt
