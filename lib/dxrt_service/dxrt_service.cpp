/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 * 
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 */

#include "dxrt/common.h"


#include <csignal>
#include <atomic>
#include <thread>
#include <future>
//  #include <unordered_set>
#include <set>
#include <map>
#include <limits>
#include <array>
#include "memory_service.hpp"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "dxrt/extern/cxxopts.hpp"
#include "service_device.h"
#include "scheduler_service.h"
#include "service_error.h"
#include "process_with_device_info.h"


#ifndef DXRT_DEBUG
// to reduce consol log size
#define DXRT_SERVICE_SIMPLE_CONSOLE_LOG 1

#endif


using std::cout;
using std::endl;
using std::make_shared;
using std::make_pair;


static constexpr uint32_t UINT_MAX_CONST = std::numeric_limits<uint32_t>::max();

void die_check_thread();

enum class DXRT_Schedule
{
    FIFO,
    RoundRobin,
    SJF
};


class DxrtService
{
 private:
    void dequeueAllClientMessageQueue(long msgType);
    std::shared_ptr<dxrt::DxrtServiceErr> _srvErr;
    std::mutex _deviceMutex;

    //packet handler
    dxrt::IPCServerMessage HandleClose(const dxrt::IPCClientMessage& clientMessage);
    dxrt::IPCServerMessage HandleGetMemory(const dxrt::IPCClientMessage& clientMessage);
    dxrt::IPCServerMessage HandleGetMemoryForModel(const dxrt::IPCClientMessage& clientMessage);
    dxrt::IPCServerMessage HandleFreeMemory(const dxrt::IPCClientMessage& clientMessage);

    dxrt::IPCServerMessage HandleViewMemory(const dxrt::IPCClientMessage& clientMessage);
    dxrt::IPCServerMessage HandleViewAvilableDevice(const dxrt::IPCClientMessage& clientMessage);
    dxrt::IPCServerMessage HandleGetUsage(const dxrt::IPCClientMessage& clientMessage);

    bool HandleTaskInit(const dxrt::IPCClientMessage& clientMessage);
    void HandleTaskDeInit(const dxrt::IPCClientMessage& clientMessage);
    bool HandleRequestScheduledInference(const dxrt::IPCClientMessage& clientMessage);
    void HandleDeviceInit(const dxrt::IPCClientMessage& clientMessage);
    void HandleDeviceDeInit(const dxrt::IPCClientMessage& clientMessage);
    void HandleDeallocateTaskMemory(const dxrt::IPCClientMessage& clientMessage);
    void HandleProcessDeInit(const dxrt::IPCClientMessage& clientMessage);

 public:
    void Process(const dxrt::IPCClientMessage& clientMessage);
    explicit DxrtService(DXRT_Schedule scheduler_option = DXRT_Schedule::FIFO);
    explicit DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_, DXRT_Schedule scheduler_option);
    void onCompleteInference(const dxrt::dxrt_response_t& response, int deviceId);
    void ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId);

    void InitDevice(int devId, dxrt::npu_bound_op bound);
    void DeInitDevice(int devId, dxrt::npu_bound_op bound);
    long ClearDevice(int procId);
    void handle_process_die(pid_t pid);
    void die_check_thread();
    int GetDeviceIdByProcId(int procId);
    void Dispose();

    bool IsTaskValid(pid_t pid, int deviceId, int taskId);
    void ClearResidualIPCMessages();
    void PrintManagedTasks();
    bool TaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize);
    void TaskDeInit(int deviceId, int taskId, int pid);

    dxrt::IPCServerWrapper _ipcServerWrapper;
    std::vector<std::shared_ptr<dxrt::ServiceDevice> > _devices;
    std::shared_ptr<SchedulerService> _scheduler;

    std::set<pid_t> _pid_set;

    std::map<std::pair<pid_t, int>, ProcessWithDeviceInfo> _infoMap;

};

DxrtService::DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_, DXRT_Schedule scheduler_option)
: _ipcServerWrapper(dxrt::IPCDefaultType()), _devices(devices_)
{
    switch (scheduler_option)
    {
        case DXRT_Schedule::RoundRobin:
            _scheduler = make_shared<RoundRobinSchedulerService>(devices_);
            break;
        case DXRT_Schedule::SJF:
            _scheduler = make_shared<SJFSchedulerService>(devices_);
            break;
        case DXRT_Schedule::FIFO:
        default:
            _scheduler = make_shared<FIFOSchedulerService>(devices_);
            break;
    }


    for (auto& device : _devices)
    {
        int id = device->id();
        _devices[id]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);

        // callback gets response from device and give it to schdeuler
        device->SetCallback([id, this](const dxrt::dxrt_response_t& resp_) {
            _scheduler->FinishJobs(id, resp_);
        });
    }
    LOG_DXRT_S << "Initialized Devices count=" << _devices.size() << std::endl;

    // callback gets response from scheduler and send it to app proc
    _scheduler->SetCallback([this](const dxrt::dxrt_response_t& resp_, int deviceId) {
        onCompleteInference(resp_, deviceId);
    });
    _scheduler->SetErrorCallback([this](dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId) {
        ErrorBroadCastToClient(err, errCode, deviceId);
    });
    
    // Task validity verification callback
    _scheduler->SetTaskValidator([this](pid_t pid, int deviceId, int taskId) -> bool {
        bool isValid = IsTaskValid(pid, deviceId, taskId);
        if (!isValid) {
            LOG_DXRT_S_ERR("Task validation failed - PID: " + std::to_string(pid) + 
                           ", Device: " + std::to_string(deviceId) + 
                           ", Task: " + std::to_string(taskId));
        }
        return isValid;
    });
    
    LOG_DXRT_S << "Initialized Scheduler" << std::endl;

    if ( _ipcServerWrapper.Initialize() == 0 )
    {
        _srvErr = std::make_shared<dxrt::DxrtServiceErr>(&_ipcServerWrapper);
        LOG_DXRT_S << "Initialized IPC Server" << std::endl;
        
        // Clear any residual messages in IPC queue at startup
        ClearResidualIPCMessages();
    }
    else
    {
        LOG_DXRT_S << "Fail to initialize IPC Server" << std::endl;
    }
}

DxrtService::DxrtService(DXRT_Schedule scheduler_option)
: DxrtService(dxrt::ServiceDevice::CheckServiceDevices(), scheduler_option)
{

}

static std::atomic<int> chLoad{0};
dxrt::RESPONSE_CODE get_ch() {
    int chno = chLoad.load();
    chno %= 3;
    chLoad.store((chno + 1) % 3);
    switch (chno) {
        case 0:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
        case 1:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1;
        case 2:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2;
        default:
            return dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
    }
}

void DxrtService::ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId)
{
    for (auto pid : _pid_set) {
        _srvErr->ErrorReportToClient(err, pid, errCode, deviceId);
    }
}

bool DxrtService::HandleTaskInit(const dxrt::IPCClientMessage& clientMessage)
{
    pid_t pid = clientMessage.pid;
    int deviceId = clientMessage.deviceId;
    int taskId = clientMessage.taskId;
    int bound = clientMessage.data;
    uint64_t modelMemorySize = clientMessage.modelMemorySize;
    bool result = TaskInit(pid, deviceId, taskId, bound, modelMemorySize);
    if (result == true)
        PrintManagedTasks();
    return result;
}
void DxrtService::HandleTaskDeInit(const dxrt::IPCClientMessage& clientMessage)
{
    pid_t pid = clientMessage.pid;
    int deviceId = clientMessage.deviceId;
    int taskId = clientMessage.taskId;

#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    int bound = clientMessage.data;
    LOG_DXRT_S << "Task DeInit - DevId: " << deviceId << ", TaskId: " << taskId
                << ", PID: " << pid << ", Bound: " << bound << endl;
#endif

    // Enhanced Task cleanup with better synchronization
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        TaskDeInit(deviceId, taskId, pid);
    }


    PrintManagedTasks();
}

bool DxrtService::TaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize)
{
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    LOG_DXRT_S << "Task Init - DevId: " << deviceId << ", TaskId: " << taskId
                << ", PID: " << pid << ", Bound: " << bound << ", Model MemSize: " << modelMemorySize << endl;
#endif

    // Enhanced memory availability check before task initialization
    auto memService = dxrt::MemoryService::getInstance(deviceId);
    if (memService != nullptr) {
        uint64_t freeSize = memService->free_size();
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
        uint64_t usedSize = memService->used_size();
        LOG_DXRT_S << "Device " << deviceId << " Memory Status - Free: " << (freeSize / (1024*1024))
                    << "MB, Used: " << (usedSize / (1024*1024)) << "MB, Required: " << (modelMemorySize / (1024*1024)) << "MB" << endl;
#endif
        if (freeSize < modelMemorySize) {
            LOG_DXRT_S_ERR("Insufficient memory for Task " + std::to_string(taskId) +
                            " - Available: " + std::to_string(freeSize / (1024*1024)) + "MB, " +
                            "Required: " + std::to_string(modelMemorySize / (1024*1024)) + "MB");

            // Try memory optimization before rejecting
            memService->OptimizeMemory();
            uint64_t newFreeSize = memService->free_size();
            LOG_DXRT_S << "After optimization - Free: " << (newFreeSize / (1024*1024)) << "MB" << endl;

            if (newFreeSize < modelMemorySize) {
                LOG_DXRT_S_ERR("Task " + std::to_string(taskId) + " initialization failed due to insufficient memory");
                return false;
            }
        }
    }
    else
    {
        LOG_DXRT_S_ERR("Invalid Device number task " + std::to_string(deviceId));
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(_deviceMutex);

        // Check if task already exists
        auto it = _infoMap.find(make_pair(pid, deviceId));
        if (it != _infoMap.end())
        {
            auto& pick = it->second;
            if (pick.hasTask(taskId))
            {
                LOG_DXRT_S_ERR("Task " + std::to_string(taskId) + " already exists for PID " +
                            std::to_string(pid) + " on device " + std::to_string(deviceId));
                return false;
            }
        }
        else
        {
            _infoMap.insert(make_pair(make_pair(pid, deviceId), ProcessWithDeviceInfo()));
        }

        ProcessWithDeviceInfo::eachTaskInfo insertInfo;
        insertInfo.bound = static_cast<dxrt::npu_bound_op>(bound);
        insertInfo.deviceId = deviceId;
        insertInfo.mem_usage = modelMemorySize;
        insertInfo.pid = pid;

        _infoMap.find(make_pair(pid, deviceId))->second.InsertTaskInfo(taskId, insertInfo);


        // Check device availability before task initialization
        if (deviceId >= static_cast<int>(_devices.size())) {
            LOG_DXRT_S_ERR("Invalid device ID: " + std::to_string(deviceId));
            return false;
        }

        if (_devices[deviceId]->isBlocked()) {
            LOG_DXRT_S_ERR("Device " + std::to_string(deviceId) + " is blocked, cannot initialize task");
            return false;
        }

        // Enhanced NPU bound option validation with 3-type limit check


        auto targetDevice = _devices[deviceId];
        int ret = targetDevice->AddBound(static_cast<dxrt::npu_bound_op>(bound));
        if (ret != 0) {
            LOG_DXRT_S_ERR("Failed to set NPU bound " + std::to_string(bound) +
                            " for device " + std::to_string(deviceId) + ", ret: " + std::to_string(ret));
            return false;
        } else {
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
            LOG_DXRT_S << "Successfully set NPU bound " << bound << " for device " << deviceId << endl;
#endif
        }
    }
    return true;
}


void DxrtService::TaskDeInit(int deviceId, int taskId, int pid)
{
    //std::lock_guard<std::mutex> lock(_deviceMutex);

    // Log current state before cleanup
    auto it = _infoMap.find(make_pair(pid, deviceId));
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    if (it != _infoMap.end())
    {
    LOG_DXRT_S << "Before cleanup - PID " << pid << " has "
                << it->second.taskCount() << " tasks on device " << deviceId << endl;
    }
    else
    {
        LOG_DXRT_S << "Before cleanup - PID " << pid << " has "
            << "no" << " tasks on device " << deviceId << endl;
        return;
    }
#endif
    // Stop any ongoing inference requests for this Task
    _scheduler->StopTaskInference(pid, deviceId, taskId);

    dxrt::npu_bound_op bound = it->second.getTaskBound(taskId);
    it->second.deleteTaskFromMap(taskId);
    auto targetDevice = _devices[deviceId];
    int ret = targetDevice->DeleteBound(static_cast<dxrt::npu_bound_op>(bound));
    if (ret == 0) {
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
        LOG_DXRT_S << "Released NPU bound " << bound << " from device " << deviceId;
        LOG_DXRT_S << "Device " << deviceId << " now has " << targetDevice->GetBoundTypeCount()
                    << "/3 bound types after releasing bound " << bound << endl;
#endif
    } else {
        LOG_DXRT_S_ERR("Failed to release NPU bound " + std::to_string(bound) +
                        " from device " + std::to_string(deviceId) + ", ret: " + std::to_string(ret));
    }
}
bool DxrtService::HandleRequestScheduledInference(const dxrt::IPCClientMessage& clientMessage)
{
    LOG_DXRT_S_DBG << clientMessage.msgType << "arrived, reqno" << clientMessage.npu_acc.req_id << endl;

    // Enhanced Task validity verification with device state check
    if (!IsTaskValid(clientMessage.pid, clientMessage.deviceId, clientMessage.npu_acc.task_id)) {
        LOG_DXRT_S_ERR("Invalid task " + std::to_string(clientMessage.npu_acc.task_id) +
                        " for process " + std::to_string(clientMessage.pid) +
                        " on device " + std::to_string(clientMessage.deviceId));
        dxrt::dxrt_response_t resp{};
        resp.req_id = clientMessage.npu_acc.req_id;
        resp.proc_id = clientMessage.npu_acc.proc_id;
        resp.status = -1;
        onCompleteInference(resp, clientMessage.deviceId);
        return false;
    }

    // Check device state before processing inference request
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);

        // Enhanced bound option validation
        pid_t pid = clientMessage.pid;
        int deviceId = clientMessage.deviceId;
        int taskId = clientMessage.npu_acc.task_id;
        int requestId = clientMessage.npu_acc.req_id;
        int requestedBound = clientMessage.npu_acc.bound;
        LOG_DXRT_S_DBG << "Inference request - PID: " << pid
                        << ", DeviceId: " << deviceId
                        << ", TaskId: " << taskId
                        << ", RequestId: " << requestId
                        << ", RequestedBound: " << requestedBound << endl;


        auto it = _infoMap.find(make_pair(pid, deviceId));
        if (it == _infoMap.end())
        {
            // not registerd process
            LOG_DXRT_S_ERR("Not Registered Process " + std::to_string(pid) + " device " + std::to_string(deviceId));
            dxrt::dxrt_response_t resp{};
            resp.req_id = requestId;
            resp.proc_id = clientMessage.npu_acc.proc_id;
            resp.status = -1;
            onCompleteInference(resp, clientMessage.deviceId);
            return false;
        }

        if (it->second.getTaskBound(taskId) != requestedBound)
        {
            LOG_DXRT_S_ERR("Process " + std::to_string(pid) + " device " + std::to_string(deviceId)
                    + ": unregistered bound " + std::to_string(requestedBound) + " for task " + std::to_string(taskId));

            // Log current registered bounds for debugging
            LOG_DXRT_S_ERR("Currently registered bounds for this process/device:");
            auto boundCounts = it->second.getBoundCounts();
            for (size_t i = 0; i < boundCounts.size(); i++)
            {
                LOG_DXRT_S_ERR("  Bound " + std::to_string(i) + " (count: " + std::to_string(boundCounts[i]) + ")");
            }

            dxrt::dxrt_response_t resp{};
            resp.req_id = requestId;
            resp.proc_id = clientMessage.npu_acc.proc_id;
            resp.status = -1;
            onCompleteInference(resp, clientMessage.deviceId);
            return false;
        }

        // Check if device is blocked before adding to scheduler
        if (static_cast<uint32_t>(clientMessage.deviceId) < _devices.size() && _devices[clientMessage.deviceId]->isBlocked()) {
            LOG_DXRT_S_ERR("Device " + std::to_string(clientMessage.deviceId) + " is blocked, rejecting inference request");
            dxrt::dxrt_response_t resp{};
            resp.req_id = clientMessage.npu_acc.req_id;
            resp.proc_id = clientMessage.npu_acc.proc_id;
            resp.status = -2;  // Device blocked error
            onCompleteInference(resp, clientMessage.deviceId);
            return false;
        }

        LOG_DXRT_S_DBG << "Inference request validation passed, adding to scheduler" << endl;
    }

    _scheduler->AddScheduler(clientMessage.npu_acc, clientMessage.deviceId);
    return true;
}
dxrt::IPCServerMessage DxrtService::HandleClose(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    dxrt::MemoryService::DeallocateAllDevice(clientMessage.pid);
    retMsg.code = dxrt::RESPONSE_CODE::CLOSE;
    retMsg.msgType = clientMessage.msgType;
    return retMsg;
}
dxrt::IPCServerMessage DxrtService::HandleGetMemory(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    uint64_t size = clientMessage.data;
    uint64_t result = 0;
    pid_t pid = clientMessage.pid;
    if (clientMessage.taskId != -1)
    {
        result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->AllocateForTask(size, pid, clientMessage.taskId);
        LOG_DXRT_S_DBG << "Allocated memory for Task " << clientMessage.taskId << ", PID " << pid << ", size " << size << endl;
    }
    else
    {
        result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->Allocate(size, pid);
        LOG_DXRT_S_DBG << "Allocated memory for PID " << pid << ", size " << size << endl;
    }

    retMsg.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
    retMsg.data = result;
    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    retMsg.result = (result != static_cast<uint64_t>(-1)) ? 0 : -1;
    _pid_set.insert(pid);
    return retMsg;
}
dxrt::IPCServerMessage DxrtService::HandleGetMemoryForModel(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    uint64_t size = clientMessage.data;
    pid_t pid = clientMessage.pid;
    uint64_t result = 0;

    if (clientMessage.taskId != -1)
    {
        result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->BackwardAllocateForTask(size, pid, clientMessage.taskId);
        LOG_DXRT_S_DBG << "Backward allocated memory for Task " << clientMessage.taskId << ", PID " << pid << ", size " << size << endl;
    }
    else
    {
        result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->BackwardAllocate(size, pid);
        LOG_DXRT_S_DBG << "Backward allocated memory for PID " << pid << ", size " << size << endl;
    }

    retMsg.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
    retMsg.data = result;
    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    retMsg.result = (result != static_cast<uint64_t>(-1)) ? 0 : -1;
    _pid_set.insert(pid);
    return retMsg;
}
dxrt::IPCServerMessage DxrtService::HandleFreeMemory(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    uint64_t address = clientMessage.data;
    pid_t pid = clientMessage.pid;
    auto result = dxrt::MemoryService::getInstance(clientMessage.deviceId)->Deallocate(address, pid);
    retMsg.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_FREE;
    retMsg.data = 123;
    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    retMsg.result = result ? 123: -1;
    return retMsg;
}
void DxrtService::HandleDeviceInit(const dxrt::IPCClientMessage& clientMessage)
{
    pid_t pid = clientMessage.pid;
    int deviceId = clientMessage.deviceId;
    int bound = clientMessage.data;
    dxrt::dxrt_custom_weight_info_t info;
    info.address = clientMessage.npu_acc.datas[0];
    info.size = clientMessage.npu_acc.datas[1];
    info.checksum = clientMessage.npu_acc.datas[2];

    InitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        // _devInfo[clientMessage.pid][deviceId][bound]++;
        _infoMap[make_pair(pid,deviceId)].InsertWeightInfo(info);

        _devices[deviceId]->DoCustomCommand(&info, dxrt::dxrt_custom_sub_cmt_t::DX_ADD_WEIGHT_INFO, sizeof(dxrt::dxrt_custom_weight_info_t));
    }
}
void DxrtService::HandleDeviceDeInit(const dxrt::IPCClientMessage& clientMessage)
{
    pid_t pid = clientMessage.pid;
    int deviceId = clientMessage.deviceId;
    int bound = clientMessage.data;
    dxrt::dxrt_custom_weight_info_t info;
    info.address = clientMessage.npu_acc.datas[0];
    info.size = clientMessage.npu_acc.datas[1];
    info.checksum = clientMessage.npu_acc.datas[2];
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        {
            _infoMap[make_pair(pid, deviceId)].EraseWeightInfo(info);
            _devices[deviceId]->DoCustomCommand(&info,
                dxrt::dxrt_custom_sub_cmt_t::DX_DEL_WEIGHT_INFO,
                sizeof(dxrt::dxrt_custom_weight_info_t));
        }
    }
    DeInitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
}

dxrt::IPCServerMessage DxrtService::HandleViewMemory(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    const dxrt::MemoryService* instance = dxrt::MemoryService::getInstance(clientMessage.deviceId);
    if (instance == nullptr)
    {
        retMsg.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
        retMsg.data = 0;
        retMsg.result = UINT_MAX_CONST;
    }
    else
    {
        uint64_t result = 0;
        if (clientMessage.code == dxrt::REQUEST_CODE::VIEW_FREE_MEMORY)
        {
            result = instance->free_size();
        }
        else if (clientMessage.code == dxrt::REQUEST_CODE::VIEW_USED_MEMORY)
        {
            result = instance->used_size();
        }
        else
        {
            std::stringstream ss;
            ss << "Invalid Message code on HandleViewMemory: ";
            ss << clientMessage.code;
            DXRT_ASSERT(false, ss.str());
        }
        retMsg.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
        retMsg.data = result;
        retMsg.result = 0;
    }
    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    return retMsg;
}
dxrt::IPCServerMessage DxrtService::HandleViewAvilableDevice(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    uint64_t result = 0;
    uint64_t mask = 1;
    for (size_t i = 0; i < _devices.size(); i++)
    {
        if (_devices[i]->isBlocked() == false)
        {
            result |= mask;
        }
        mask = mask << 1;
    }
    retMsg.code = dxrt::RESPONSE_CODE::VIEW_AVAILABLE_DEVICE_RESULT;
    retMsg.data = result;
    retMsg.result = 0;

    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    return retMsg;
}
dxrt::IPCServerMessage DxrtService::HandleGetUsage(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage retMsg;
    double result = _devices[clientMessage.deviceId]->getUsage(clientMessage.data);
    retMsg.code = dxrt::RESPONSE_CODE::GET_USAGE_RESULT;
    retMsg.data = result * 1000;
    retMsg.result = 0;

    retMsg.deviceId = clientMessage.deviceId;
    retMsg.msgType = clientMessage.msgType;
    return retMsg;
}
void DxrtService::HandleDeallocateTaskMemory(const dxrt::IPCClientMessage& clientMessage)
{
    pid_t pid = clientMessage.pid;
    int deviceId = clientMessage.deviceId;
    int taskId = clientMessage.taskId;
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    LOG_DXRT_S << "Deallocate Task Memory - DevId: " << deviceId << ", TaskId: " << taskId
                << ", PID: " << pid << endl;
#endif
    // Check if Task is already deallocated
    if (IsTaskValid(pid, deviceId, taskId)) {
        LOG_DXRT_S_ERR("Task " + std::to_string(taskId) +
                        " is still active, cannot deallocate memory");
        return;
    }

    auto memService = dxrt::MemoryService::getInstance(deviceId);
    if (memService != nullptr)
    {
        bool success = memService->DeallocateTask(pid, taskId);
        if (success)
        {
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
            LOG_DXRT_S << "Successfully deallocated memory for Task " << taskId
                        << ", PID: " << pid << ", Device: " << deviceId << endl;
#endif
        }
        else
        {
            LOG_DXRT_S_ERR("Failed to deallocate memory for Task " + std::to_string(taskId) +
                            ", PID: " + std::to_string(pid) +
                            ", Device: " + std::to_string(deviceId));
        }
    }
    else
    {
        LOG_DXRT_S_ERR("MemoryService not found for device " + std::to_string(deviceId));
    }
}
void DxrtService::HandleProcessDeInit(const dxrt::IPCClientMessage& clientMessage)
{
    int deviceId = clientMessage.deviceId;
    pid_t pid = clientMessage.pid;
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    LOG_DXRT_S << "Process DeInit - DevId: " << deviceId << ", PID: " << pid << endl;
#endif

    // Enhanced process cleanup with better validation
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);

        // Log current state before cleanup
        auto it = _infoMap.find(make_pair(pid, deviceId));
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
        if (it != _infoMap.end())
        {
            LOG_DXRT_S << "Process cleanup - PID " << pid << " task count on device " << deviceId << ": "
                    << it->second.taskCount() << endl;
        }
        else
        {
            LOG_DXRT_S << "Process cleanup - PID " << pid << " task count on device " << deviceId << ": "
                    << "None" << endl;
        }
#endif
        // Stop all inference requests for this process
        _scheduler->StopAllInferenceForProcess(pid, deviceId);

        // Cleanup all tasks for this process on this device

        if (it != _infoMap.end())
        {
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
            LOG_DXRT_S << "Cleaning up " << it->second.taskCount() << " tasks for process " << pid
                        << " on device " << deviceId << endl;
#endif
            // First stop all tasks, then cleanup
            for (int taskId : it->second.getTaskIds())
            {
                TaskDeInit(deviceId, taskId, pid);
            }

            _infoMap.erase(it);
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
            LOG_DXRT_S << "All tasks cleaned up for process " << pid << " on device " << deviceId << endl;
#endif
        }
    }


    // Deallocate all device memory for this process
    auto memService = dxrt::MemoryService::getInstance(deviceId);
    if (memService != nullptr) {
        bool memoryReleased = memService->DeallocateAllForProcess(pid);
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
        if (memoryReleased) {
            LOG_DXRT_S << "Deallocated all memory for process " << pid << " on device " << deviceId << endl;

        } else {
            LOG_DXRT_S_DBG << "No memory to deallocate for process " << pid << " on device " << deviceId << endl;
        }
#else
        std::ignore = memoryReleased;
#endif
    }

    PrintManagedTasks();
}

void DxrtService::Process(const dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage serverMessage;
    // auto requestId = clientMessage.data;

    pid_t pid = clientMessage.pid;
    dxrt::REQUEST_CODE code = clientMessage.code;

    {
        serverMessage.msgType = clientMessage.msgType;
        // Enhanced message validation
        uint32_t codeValue = static_cast<uint32_t>(code);
        if (codeValue > 10000) {  // Sanity check for obviously invalid codes
            LOG_DXRT_S_ERR("Invalid REQUEST_CODE received: " + std::to_string(codeValue) +
                        " from PID: " + std::to_string(pid) +
                        " msgType: " + std::to_string(clientMessage.msgType));
            return;  // Drop invalid messages
        }

        std::string codeStr = _s(code);
        LOG_DXRT_S_DBG << "client-message code=" << codeStr << " (" << codeValue << ")"
                    << " from PID=" << pid << " msgType=" << clientMessage.msgType << endl;

        // Log unknown requests with more details for debugging
        if (codeStr == "REQUEST_Unknown") {
            LOG_DXRT_S_ERR("Unknown REQUEST_CODE: " + std::to_string(codeValue) +
                        " from PID: " + std::to_string(pid) +
                        " deviceId: " + std::to_string(clientMessage.deviceId) +
                        " data: " + std::to_string(clientMessage.data) +
                        " msgType: " + std::to_string(clientMessage.msgType));

            // Send error response for unknown requests
            serverMessage.code = dxrt::RESPONSE_CODE::INVALID_REQUEST_CODE;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.result = static_cast<uint32_t>(-1);
            _ipcServerWrapper.SendToClient(serverMessage);
            return;
        }
    }
    switch (code)
    {
        case dxrt::REQUEST_CODE::CLOSE: {
            serverMessage = HandleClose(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY: {
            serverMessage = HandleGetMemory(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL: {
            serverMessage = HandleGetMemoryForModel(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::FREE_MEMORY: {
            serverMessage = HandleFreeMemory(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE: {
            HandleRequestScheduledInference(clientMessage);
            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_INIT: {
            HandleDeviceInit(clientMessage);
            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_DEINIT: {
            HandleDeviceDeInit(clientMessage);
            return;
        }
        case dxrt::REQUEST_CODE::TASK_INIT: {
            bool result = HandleTaskInit(clientMessage);
            if (result == false) return;
            break;
        }
        case dxrt::REQUEST_CODE::TASK_DEINIT: {
            HandleTaskDeInit(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::DEALLOCATE_TASK_MEMORY: {
            HandleDeallocateTaskMemory(clientMessage);
            return;
        }
        case dxrt::REQUEST_CODE::PROCESS_DEINIT: {
            HandleProcessDeInit(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::DEVICE_RESET: {
            return;
        }
        case dxrt::REQUEST_CODE::INFERENCE_COMPLETED: {
            return;
        }
        case dxrt::REQUEST_CODE::VIEW_FREE_MEMORY:
        case dxrt::REQUEST_CODE::VIEW_USED_MEMORY: {
            serverMessage = HandleViewMemory(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::VIEW_AVAILABLE_DEVICE:
        {
            serverMessage = HandleViewAvilableDevice(clientMessage);
            break;
        }
        case dxrt::REQUEST_CODE::GET_USAGE:
        {
            serverMessage = HandleGetUsage(clientMessage);
            break;
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

    dxrt::IPCServerMessage serverMessage{};
    LOG_DXRT_S_DBG << deviceId << ": " << response.proc_id <<"'s Response " << response.req_id << " completed "<< endl;

    serverMessage.code = dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0;
    serverMessage.data = 333;
    serverMessage.result = 0;
    serverMessage.msgType = response.proc_id;  // Use proc_id as msgType to match client's msgType
    serverMessage.deviceId = deviceId;
    serverMessage.npu_resp = response;

    LOG_DXRT_S_DBG << "Sending response to client with msgType: " << serverMessage.msgType
                   << ", code: " << static_cast<int>(serverMessage.code)
                   << ", deviceId: " << serverMessage.deviceId << endl;
    
    int ret = _ipcServerWrapper.SendToClient(serverMessage);
    if (ret != 0) {
        LOG_DXRT_S_ERR("Failed to send response to client, ret: " + std::to_string(ret));
    } else {
        LOG_DXRT_S_DBG << "Successfully sent response to client" << endl;
    }

}

// Task validity verification function implementation
bool DxrtService::IsTaskValid(pid_t pid, int deviceId, int taskId)
{
    std::lock_guard<std::mutex> lock(_deviceMutex);
    
    // Check Task metadata in DxrtService
    auto it = _infoMap.find(make_pair(pid, deviceId));
    if (it == _infoMap.end())
    {
        return false;
    }
    
    bool taskExists = it->second.hasTask(taskId);
    
    // Check Task validity in MemoryService
    auto memService = dxrt::MemoryService::getInstance(deviceId);
    bool memoryExists = (memService != nullptr) && memService->IsTaskValid(pid, taskId);
    
    return taskExists && memoryExists;
}

void DxrtService::ClearResidualIPCMessages()
{
    LOG_DXRT_S << "Clearing residual IPC messages from previous sessions..." << endl;
    LOG_DXRT_S_DBG << "IPC message queue cleanup will be handled by IPC system" << endl;
}

void DxrtService::PrintManagedTasks()
{
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG  
    std::lock_guard<std::mutex> lock(_deviceMutex);

    LOG_DXRT_S << "==================== Managed Tasks Report ====================" << endl;
    if (_infoMap.empty()) {
        LOG_DXRT_S << "  No tasks are currently managed by the service." << endl;
    } else {
        pid_t current_pid = 0;
        for (const auto& pid_pair_it : _infoMap) {
            pid_t pid = pid_pair_it.first.first;
            int deviceId = pid_pair_it.first.second;
            if (pid != current_pid)
            {
                LOG_DXRT_S << "  [PID: " << pid << "]" << endl;
                current_pid = pid;
            }
            auto task_set = pid_pair_it.second.getTaskIds();

            if (task_set.empty()) {
                LOG_DXRT_S << "    - Device ID: " << deviceId << " -> No tasks." << endl;
            } else {
                std::stringstream ss;
                bool first = true;
                for (int taskId : task_set) {
                    if (!first) {
                        ss << ", ";
                    }
                    ss << taskId;
                    first = false;
                }
                LOG_DXRT_S << "    - Device ID: " << deviceId << " -> Tasks: [ " << ss.str() << "]" << endl;
            }
        }
    }
    LOG_DXRT_S << "============================================================" << endl;
#endif
}

void DxrtService::dequeueAllClientMessageQueue(long msgType)
{
    dxrt::IPCClientWrapper clientWrapper(dxrt::IPCDefaultType(), msgType);
    clientWrapper.ClearMessages();  // clear remained messages
    clientWrapper.Close();  // close
}

int DxrtService::GetDeviceIdByProcId(int procId)
{
    int deviceId = -1;
    for (auto it = _infoMap.begin(); it != _infoMap.end(); it++)
    {
        int pid = it->first.first;
        int deviceIdValue = it->first.second;
        if (pid == procId)
        {
            deviceId = deviceIdValue;
        }
    }
    return deviceId;
}

void DxrtService::InitDevice(int devId, dxrt::npu_bound_op bound)
{
    int ret;
    /* TODO - Send init command to driver to clear internal logic */
    LOG_DXRT_S << "DevId : " << devId << ", add bound : " << bound << endl;

    // Check if device is blocked before adding bound
    if (_devices[devId]->isBlocked()) {
        LOG_DXRT_S_ERR("Device " + std::to_string(devId) + " is blocked, cannot add bound " + std::to_string(bound));
        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, -1, devId);
        return;
    }

    ret = _devices[devId]->AddBound(static_cast<dxrt::npu_bound_op>(bound));
    if (ret != 0)
    {
        LOG_DXRT_S_ERR("Failed to add bound " + std::to_string(bound) + " to device " + std::to_string(devId) + ", ret: " + std::to_string(ret));
        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, ret, devId);
    }
    // DXRT_ASSERT(ret==0, "failed to apply bound option to device");
}

void DxrtService::DeInitDevice(int devId, dxrt::npu_bound_op bound)
{
    int ret;
    /* TODO - Send init command to driver to clear internal logic */
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG  
    LOG_DXRT_S << "DevId : " << devId << ", delete bound : " << bound << endl;
#endif
    ret = _devices[devId]->DeleteBound(static_cast<dxrt::npu_bound_op>(bound));
    if (ret != 0)
    {
        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, ret, devId);
    }
}

#define DXRT_S_DEV_CLR_TIMEOUT_MS     (600)
#define DXRT_S_DEV_CLR_TIMEOUT_CNT    (3)
long DxrtService::ClearDevice(int procId)
{
    LOG_DXRT_S_DBG << endl;

    try {
        const std::chrono::milliseconds timeout(DXRT_S_DEV_CLR_TIMEOUT_MS);
        auto lastLoadCheckTime = std::chrono::steady_clock::now();
        int cnt = 0;
        volatile int prevLoad = _scheduler->GetProcLoad(procId);
        int devId = 0;

        while (true)
        {
            volatile int currLoad = _scheduler->GetProcLoad(procId);
            if (currLoad == 0) break;

            auto currentTime = std::chrono::steady_clock::now();
            if (currentTime - lastLoadCheckTime >= timeout)
            {
                lastLoadCheckTime = currentTime;
                if (currLoad == prevLoad)
                {
                    DXRT_ASSERT(currLoad == _scheduler->GetProcLoad(procId), "Failed by cache");
                    LOG_DXRT_S_ERR("Timeout reached during process termination (" + std::to_string(cnt) + ")"+ std::to_string(procId));
                    _scheduler->ClearAllLoad();
                    devId = GetDeviceIdByProcId(procId);
                    if (devId!= -1)
                        _devices[devId]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
                    break;
                }
                else
                {
                    if (++cnt > DXRT_S_DEV_CLR_TIMEOUT_CNT)
                    {
                        LOG_DXRT_S_ERR("Overall timeout reached.(" + std::to_string(cnt) + ")");
                        _scheduler->ClearAllLoad();
                        devId = GetDeviceIdByProcId(procId);
                        if (devId!= -1)
                            _devices[devId]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
                        break;
                    }
                    else
                    {
                        cnt = 0;
                        prevLoad = currLoad;
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        return 0;
    } catch (const std::exception& e) {
        std::string str = std::string("Exception occurred: ") + e.what();
        LOG_DXRT_S_ERR(str);
        return 999;
    }
    // no need to return since all block has return
}

#ifdef __linux__
static bool IsProcessRunning(pid_t procId)
{
    if (kill(procId, 0) == 0)
    {
        return true;
    }
    else
    {
        if (errno == ESRCH) {
            return false;
        } else if (errno == EPERM) {
            return true;
        } else {
            perror("kill");
            return false;
        }
    }
}

#elif _WIN32

static bool IsProcessRunning(DWORD procId)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, procId);

    if (hProcess == NULL) {
        DWORD error = GetLastError();

        if (error == ERROR_INVALID_PARAMETER) {
            return false;
        }

        LOG_DXRT_ERR("OpenProcess failed for PID " << procId << ". Error: " << error)
        return false;
    }


    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        if (exitCode == STILL_ACTIVE) {
            CloseHandle(hProcess);
            return true;
        }
        else
        {
            CloseHandle(hProcess);
            return false;
        }
    }
    else
    {
        LOG_DXRT_ERR("GetExitCodeProcess failed for PID " << procId << ". Error: " << GetLastError())
            CloseHandle(hProcess);
        return false;
    }
}
#endif
void DxrtService::handle_process_die(pid_t procId)
{
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG  
    LOG_DXRT_S << "Process " << procId << " died, starting cleanup" << endl;
#endif
    // Enhanced cleanup sequence with better synchronization

    // 1. Stop scheduler first (no lock) - prevents new requests
    _scheduler->StopScheduler(procId);
    dequeueAllClientMessageQueue(procId);

    // 2. Clean up Task metadata with enhanced synchronization (Lock order: _deviceMutex first)
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        for (auto pidit = _infoMap.lower_bound(std::make_pair(procId, -1)); pidit != _infoMap.end();)
        {
            if (pidit == _infoMap.end())
            {
                break;
            }
            int pid_in_set = pidit->first.first;
            int deviceId = pidit->first.second;
            if (pid_in_set != procId)
            {
                break;
            }

            auto taskIds = pidit->second.getTaskIds();
            for (int taskId : taskIds)
            {
                TaskDeInit(deviceId, taskId, procId);
            }
            if (pidit->second.taskCount() == 0)
            {
                pidit = _infoMap.erase(pidit);
            }
            else
            {
                pidit++;
            }
        }
    }
    // 2. bound option delete(this is done by TaskDeInit)

    // 3. Deallocate memory with enhanced safety (separate lock to avoid deadlocks)
    dxrt::MemoryService::DeallocateAllDevice(procId);
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    LOG_DXRT_S << "Process " << procId << ": Deallocated all device memory" << endl;
#endif
    // 4. Clean up scheduler state
    _scheduler->cleanDiedProcess(procId);

    // 5. Clean up device state with enhanced error handling (run separately async)
    {
        std::future<long> result = std::async(std::launch::async, &DxrtService::ClearDevice, this, procId);
        long errCode = result.get();
        _scheduler->StartScheduler(procId);
        _scheduler->ClearProcLoad(procId);
        if (errCode != 0)
        {
            if (errCode == 1)
                ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_TERMINATION, errCode, -1);
            else if (errCode == 2)
                ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, errCode, -1);
            else
                ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_UNKNOWN_ERR, errCode, -1);
        }
    }
#ifndef DXRT_SERVICE_SIMPLE_CONSOLE_LOG
    LOG_DXRT_S << "Process " << procId << ": Cleanup completed" << endl;
#endif
}

void DxrtService::die_check_thread()
{
    LOG_DXRT_S << "Started client process status check thread" << std::endl;

    int cycleCount = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Check process status
        for (auto it = _pid_set.begin(); it != _pid_set.end(); )
        {
            pid_t procId = *it;
            if (IsProcessRunning(procId) == false)
            {
                handle_process_die(procId);
                it = _pid_set.erase(it);
            }
            else
            {
                it++;
            }
        }

        // Update device usage
        for (size_t i = 0; i < _devices.size(); i++)
        {
            _devices[i]->usageTimerTick();
            LOG_DXRT_DBG << "Usage of Device " << i << ":" << _devices[i]->getUsage(0)
                << "," << _devices[i]->getUsage(1) << "," << _devices[i]->getUsage(2) << endl;
        }

        // Perform memory optimization every 10 seconds
        cycleCount++;
        if (cycleCount >= 10) {
            cycleCount = 0;

            // Optimize memory for all devices
            for (size_t i = 0; i < _devices.size(); i++) {
                auto memService = dxrt::MemoryService::getInstance(static_cast<int>(i));
                if (memService != nullptr) {
                    memService->OptimizeMemory();
                }
            }

            LOG_DXRT_S_DBG << "Periodic memory optimization completed" << endl;
        }
    }
}

void DxrtService::Dispose()
{
    _ipcServerWrapper.Close();
}


static DxrtService* service_dispose;

void signalHandler(int signalno)
{
    std::ignore = signalno;
    service_dispose->Dispose();
    exit(EXIT_FAILURE);
}


int DXRT_API dxrt_service_main(int argc, char* argv[])
{
    cxxopts::Options options("dxrtd", "dxrtd");
    std::string scheduler_option_str;
    options.add_options()
        ("s, scheduler", "Scheduler Mode(FIFO, RoundRobin, SJF)", cxxopts::value<std::string>(scheduler_option_str));

    auto cmd = options.parse(argc, argv);

    DXRT_Schedule scheduler_option = DXRT_Schedule::FIFO;
    if (scheduler_option_str == "RoundRobin")
    {
        LOG_DXRT_S << "Uses Round Robin Scheduler\n";
        scheduler_option = DXRT_Schedule::RoundRobin;
    }
    else if (scheduler_option_str == "SJF")
    {
        LOG_DXRT_S << "Uses Shortest Jobs First Scheduler\n";

        scheduler_option = DXRT_Schedule::SJF;
    }

    DxrtService service(scheduler_option);
    service_dispose = &service;


    std::thread th(&DxrtService::die_check_thread, &service);
#ifdef __linux__
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGBUS,  signalHandler);
    signal(SIGABRT, signalHandler);

#elif _WIN32
    // not implemented
#endif



    while (true)
    {
        dxrt::IPCClientMessage clientMessage;
        service._ipcServerWrapper.ReceiveFromClient(clientMessage);

        if ( clientMessage.code != dxrt::REQUEST_CODE::CLOSE )
        {
            service.Process(clientMessage);
        }
    }
#ifdef __linux__
    // th.join(); // sonarqube bugs
#elif _WIN32
    // not implemented
#endif


    // singleton cleanup
    // dxrt::Scheduler::GetInstance().Cleanup();
    // dxrt::MemoryManager::GetInstance().Cleanup();
    // dxrt::DeviceStatus::GetInstance().Cleanup();

    //return 0;
}
