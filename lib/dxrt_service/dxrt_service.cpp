// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"


#include <csignal>
#include "memory_service.hpp"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include <atomic>
#include <thread>
#include "service_device.h"
#include "scheduler_service.h"
#include "service_error.h"


void die_check_thread();

class DxrtService
{

private:
    void dequeueAllClientMessageQueue(long msgType);
    std::shared_ptr<dxrt::DxrtServiceErr> _srvErr;

public:
    void Process(dxrt::IPCClientMessage& clientMessage);
    DxrtService();
    explicit DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_);
    void onCompleteInference(const dxrt::dxrt_response_t& response, int deviceId);
    void ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode);

    void handle_process_die();
    void die_check_thread();

    dxrt::IPCServerWrapper _ipcServerWrapper;
    std::vector<std::shared_ptr<dxrt::ServiceDevice> > _devices;
    SchedulerService _scheduler;

    std::set<pid_t> _pid_set;
};

DxrtService::DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_)
: _ipcServerWrapper(dxrt::IPC_TYPE::MESSAE_QUEUE), _devices(devices_), _scheduler(devices_)
{
    for (auto& device : _devices)
    {
        int id = device->id();

        // callback gets response from device and give it to schdeuler
        device->SetCallback([id, this](const dxrt::dxrt_response_t& resp_) {
            _scheduler.FinishJobs(id, resp_);
        });
    }
    LOG_DXRT_S << "Initialized Devices count=" << _devices.size() << std::endl;

    // callback gets response from scheduler and send it to app proc
    _scheduler.SetCallback([this](const dxrt::dxrt_response_t& resp_, int deviceId) {
        onCompleteInference(resp_, deviceId);
    });
    _scheduler.SetErrorCallback([this](dxrt::dxrt_server_err_t err, uint32_t errCode) {
        ErrorBroadCastToClient(err, errCode);
    });
    LOG_DXRT_S << "Initialized Scheduler" << std::endl;

    if ( _ipcServerWrapper.Initialize() == 0 ) 
    {
        _srvErr = std::make_shared<dxrt::DxrtServiceErr>(&_ipcServerWrapper);
        LOG_DXRT_I << "Initialized IPC Server" << std::endl;
    }
    else
    {
        LOG_DXRT_I << "Fail to initialize IPC Server" << std::endl;
    }

}

DxrtService::DxrtService()
: DxrtService(dxrt::ServiceDevice::CheckServiceDevices())
{

}

static std::atomic<int> ch = {0};
dxrt::RESPONSE_CODE get_ch()
{
    int chno = ch;
    chno %= 3;
    ch++;
    switch (ch)
    {
        case 0:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
        case 1:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1;
        case 2:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2;
    }
    return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
}

void DxrtService::ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode)
{
    for (auto pid : _pid_set) {
        _srvErr->ErrorReportToClient(err, pid, errCode);
    }
}

void DxrtService::Process(dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage serverMessage;
    pid_t pids = clientMessage.pid;
    dxrt::REQUEST_CODE code = clientMessage.code;

    switch (code)
    {
        case dxrt::REQUEST_CODE::CLOSE: {
            dxrt::MemoryService::DeallocateAllDevice(pids);
            serverMessage.code = dxrt::RESPONSE_CODE::CLOSE;
            serverMessage.msgType = clientMessage.msgType;
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY: {
            uint64_t alloc_size = clientMessage.data;

            auto addr = dxrt::MemoryService::getInstance(clientMessage.deviceId)->Allocate(alloc_size, pids);
            serverMessage.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
            serverMessage.data = addr;
            serverMessage.result = 0;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.seqId = clientMessage.seqId;
            serverMessage.seqId = clientMessage.seqId;

            serverMessage.deviceId = clientMessage.deviceId;
            if (addr == static_cast<uint64_t>(-1))
                serverMessage.result = -1;
            _pid_set.insert(pids);
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL: {
            uint64_t alloc_size = clientMessage.data;

            auto addr = dxrt::MemoryService::getInstance(clientMessage.deviceId)->BackwardAllocate(alloc_size, pids);
            serverMessage.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
            serverMessage.data = addr;
            serverMessage.result = 0;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.seqId = clientMessage.seqId;

            serverMessage.deviceId = clientMessage.deviceId;
            if (addr == static_cast<uint64_t>(-1))
                serverMessage.result = -1;
            _pid_set.insert(pids);
            break;
        }
        case dxrt::REQUEST_CODE::FREE_MEMORY: {
            uint64_t address = clientMessage.data;
            pid_t pid = clientMessage.pid;
            auto result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->Deallocate(address, pid);
            serverMessage.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_FREE;
            serverMessage.data = 123;
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.result = result ? 123: -1;
            break;
        }
        case dxrt::REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE: {
            // auto requestId = clientMessage.data;

            LOG_DXRT_S_DBG << clientMessage.msgType << "arrived, reqno" << clientMessage.npu_acc.req_id <<endl;
            _scheduler.AddScheduler(clientMessage.npu_acc, clientMessage.deviceId);

            return;
        }
        case dxrt::REQUEST_CODE::INF_STOP_REQUEST: {
            LOG_DXRT_S << "Receive Stop command" << endl;
            _scheduler.StopScheduler(clientMessage.pid);

            // serverMessage.code = dxrt::RESPONSE_CODE::INF_STOP_REQUEST_RESP;
            // serverMessage.data = 1;
            // break;
            return;
        }
        case dxrt::REQUEST_CODE::INF_STOP_END_REQUEST: {
            LOG_DXRT_S << "Receive Stop End command" << endl;
            _scheduler.StartScheduler();
            return;
        }
        case dxrt::REQUEST_CODE::INFERENCE_COMPLETED: {
            return;
        }
        default: {
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.code = dxrt::RESPONSE_CODE::INVALID_REQUEST_CODE;
            break;
        }
    }

    _ipcServerWrapper.SendToClient(serverMessage);
}

void DxrtService::onCompleteInference(const dxrt::dxrt_response_t& response, int deviceId)
{

    dxrt::IPCServerMessage serverMessage;
    LOG_DXRT_S_DBG << deviceId << ": " << response.proc_id <<"'s Response " << response.req_id << " completed "<< endl;

    serverMessage.code =  dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
    serverMessage.data = 333;
    serverMessage.result = 0;
    serverMessage.msgType = response.proc_id;
    serverMessage.deviceId = deviceId;
    serverMessage.npu_resp = response;

    _ipcServerWrapper.SendToClient(serverMessage);

}

void DxrtService::dequeueAllClientMessageQueue(long msgType)
{
    dxrt::IPCClientWrapper clientWrapper(dxrt::IPC_TYPE::MESSAE_QUEUE, msgType);
    clientWrapper.ClearMessages(); // clear remained messages
    clientWrapper.Close(); // close
}

void DxrtService::handle_process_die()
{

    for (auto it = _pid_set.begin(); it != _pid_set.end(); )
    {
        pid_t pid = *it;

        if (kill(pid, 0) == 0) {
            //std::cout << "Process " << pid << " is still running." << std::endl;
            ++it;
        } else {
            if (errno == ESRCH) {
                //std::cout << "Process " << pid << " has terminated. Cleaning up resources." << std::endl;
                dequeueAllClientMessageQueue(pid);
                dxrt::MemoryService::DeallocateAllDevice(pid);
                _scheduler.cleanDiedProcess(pid);
                it = _pid_set.erase(it);
            } else if (errno == EPERM) {
                ++it;
            } else {
                perror("kill");
                ++it;
            }
        }
    }
}

void DxrtService::die_check_thread()
{
    LOG_DXRT_S << "Started client process status check thread" << std::endl;
    
    while (true)
    {
        sleep(1);
        handle_process_die();
    }
}


int dxrt_service_main()
{
    LOG_DXRT_S << "Started dxrtd" << std::endl;

    DxrtService service;
   
    std::thread th(&DxrtService::die_check_thread, &service);
    while (true)
    {
        dxrt::IPCClientMessage clientMessage;
        
        service._ipcServerWrapper.ReceiveFromClient(clientMessage);

        if ( clientMessage.code != dxrt::REQUEST_CODE::CLOSE ) 
        {
            service.Process(clientMessage);
        }

    }
    th.join();
    

    // singleton cleanup
    //dxrt::Scheduler::GetInstance().Cleanup();
    //dxrt::MemoryManager::GetInstance().Cleanup();
    //dxrt::DeviceStatus::GetInstance().Cleanup();

    return 0;
}