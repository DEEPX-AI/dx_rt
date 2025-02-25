// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.


#include "dxrt/driver.h"
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

namespace dxrt {

using std::unique_lock;
using std::mutex;


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
    //unique_lock<mutex> _lk(_lock);
    dxrt::IPCClientMessage clientMessage;
    dxrt::IPCServerMessage serverMessage;
    do{


        clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY;
        clientMessage.deviceId = deviceId;
        clientMessage.data = required;
        clientMessage.pid = getpid();


        //ipcClientWrapper.SendToServer(serverMessage, clientMessage);
        ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
        if (serverMessage.result == 0)
        {
            break;
        }
        std::this_thread::sleep_for(chrono::seconds(2));

//#ifdef __linux__
        //sleep(2);
//#elif _WIN32
//        std::this_thread::sleep_for(chrono::seconds(2));
//        Sleep(2);
//#endif
    } while (true);
    LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
    return serverMessage.data;
}
uint64_t MultiprocessMemory::BackwardAllocate(int deviceId, uint64_t required)
{
    mpConnect_once_wrapper();
    //unique_lock<mutex> _lk(_lock);
    dxrt::IPCClientMessage clientMessage;
    dxrt::IPCServerMessage serverMessage;
    do{


        clientMessage.code = dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL;
        clientMessage.deviceId = deviceId;
        clientMessage.data = required;
        clientMessage.pid = getpid();


        //ipcClientWrapper.SendToServer(serverMessage, clientMessage);
        ipcClientWrapperSync.SendToServer(serverMessage, clientMessage);
        if (serverMessage.result == 0)
        {
            break;
        }

        std::this_thread::sleep_for(chrono::seconds(2));
//#ifdef __linux__
//        sleep(5);
//#elif _WIN32
//        this_thread::sleep_for(chrono::microseconds(50));
//#endif
    } while (true);
    LOG_DXRT_DBG << std::hex << serverMessage.data << std::dec << " is allocated from service\n";
    return serverMessage.data;
}

void MultiprocessMemory::Deallocate(int deviceId, uint64_t addr)
{
    mpConnect_once_wrapper();
    //unique_lock<mutex> _lk(_lock);
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
    //unique_lock<mutex> _lk(_lock);
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
    //unique_lock<mutex> _lk(_lock);
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
    //unique_lock<mutex> _lk(_lock);
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
    //unique_lock<mutex> _lk(_lock);
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
    //unique_lock<mutex> _lk(_lock);
    dxrt::IPCClientMessage clientMessage;
    LOG_DXRT_DBG << "Dev Id : " << deviceId << "\n";

    clientMessage.code = dxrt::REQUEST_CODE::DEVICE_RESET;
    clientMessage.deviceId = deviceId;
    clientMessage.pid = getpid();

    ipcClientWrapper.SendToServer(clientMessage);
    return;
}
}  // namespace dxrt
