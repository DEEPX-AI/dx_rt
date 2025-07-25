// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"


#include <csignal>
#include <atomic>
#include <thread>
#include <future>
#include <unordered_set>
#include <set>
#include <map>
#include <limits>
#include "memory_service.hpp"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "dxrt/extern/cxxopts.hpp"
#include "service_device.h"
#include "scheduler_service.h"
#include "service_error.h"


using std::cout;
using std::endl;
using std::make_shared;


static constexpr uint32_t UINT_MAX_CONST = std::numeric_limits<uint32_t>::max();

void die_check_thread();

enum class DXRT_Schedule
{
    FIFO,
    RoundRobin,
    SJF
};

namespace dxrt{
bool operator==(const dxrt::dxrt_custom_weight_info_t& a, const dxrt::dxrt_custom_weight_info_t& b) {
    return (a.address == b.address &&
            a.size == b.size &&
            a.checksum == b.checksum);
}
}
namespace std {
template <>
struct hash<dxrt::dxrt_custom_weight_info_t> {
    size_t operator()(const dxrt::dxrt_custom_weight_info_t& info) const {
        size_t h1 = std::hash<uint32_t>()(info.address);
        size_t h2 = std::hash<uint32_t>()(info.size);
        size_t h3 = std::hash<uint32_t>()(info.checksum);

        size_t seed = 0;
        seed ^= h1 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
}


class DxrtService
{
 private:
    void dequeueAllClientMessageQueue(long msgType);
    std::shared_ptr<dxrt::DxrtServiceErr> _srvErr;
    std::mutex _deviceMutex;

 public:
    void Process(dxrt::IPCClientMessage& clientMessage);
    explicit DxrtService(DXRT_Schedule scheduler_option = DXRT_Schedule::FIFO);
    explicit DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_, DXRT_Schedule scheduler_option);
    void onCompleteInference(const dxrt::dxrt_response_t& response, int deviceId);
    void ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId);

    void InitDevice(int devId, dxrt::npu_bound_op bound);
    void DeInitDevice(int devId, dxrt::npu_bound_op bound);
    long ClearDevice(int procId);
    void handle_process_die(DxrtService *service);
    void die_check_thread();
    int GetDeviceIdByProcId(int procId);
    void Dispose();
    
    bool IsTaskValid(pid_t pid, int deviceId, int taskId);
    void ClearResidualIPCMessages();
    void PrintManagedTasks();

    dxrt::IPCServerWrapper _ipcServerWrapper;
    std::vector<std::shared_ptr<dxrt::ServiceDevice> > _devices;
    std::shared_ptr<SchedulerService> _scheduler;

    std::set<pid_t> _pid_set;
    /* process id / device id / bounding option */
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> _devInfo;
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<dxrt::dxrt_custom_weight_info_t> > > _weightInfo;

    /* process id / device id / task id -> {bound, memory_size} */
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::pair<int, uint64_t>>>> _taskInfo;
};

DxrtService::DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_,DXRT_Schedule scheduler_option)
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

void DxrtService::Process(dxrt::IPCClientMessage& clientMessage)
{
    dxrt::IPCServerMessage serverMessage;
    // auto requestId = clientMessage.data;

    pid_t pid = clientMessage.pid;
    dxrt::REQUEST_CODE code = clientMessage.code;

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
    
    switch (code)
    {
        case dxrt::REQUEST_CODE::CLOSE: {
            dxrt::MemoryService::DeallocateAllDevice(pid);
            serverMessage.code = dxrt::RESPONSE_CODE::CLOSE;
            serverMessage.msgType = clientMessage.msgType;
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY: {
            uint64_t size = clientMessage.data;
            pid_t pid = clientMessage.pid;
            uint64_t result = 0;
            
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
            
            serverMessage.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
            serverMessage.data = result;
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.result = (result != static_cast<uint64_t>(-1)) ? 0 : -1;
            _pid_set.insert(pid);
            break;
        }
        case dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL: {
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
            
            serverMessage.code = dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION;
            serverMessage.data = result;
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            serverMessage.result = (result != static_cast<uint64_t>(-1)) ? 0 : -1;
            _pid_set.insert(pid);
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
                return;
            }
            
            // Check device state before processing inference request
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                
                // Enhanced bound option validation
                int requestedBound = clientMessage.npu_acc.bound;
                LOG_DXRT_S_DBG << "Inference request - PID: " << clientMessage.pid 
                               << ", DeviceId: " << clientMessage.deviceId 
                               << ", TaskId: " << clientMessage.npu_acc.task_id 
                               << ", RequestId: " << clientMessage.npu_acc.req_id 
                               << ", RequestedBound: " << requestedBound << endl;
                
                if (_devInfo[clientMessage.pid][clientMessage.deviceId].find(requestedBound) == _devInfo[clientMessage.pid][clientMessage.deviceId].end())
                {
                    LOG_DXRT_S_ERR("Process " + std::to_string(clientMessage.pid) + " device " + std::to_string(clientMessage.deviceId)
                         + ": unregistered bound " + std::to_string(requestedBound) + " for task " + std::to_string(clientMessage.npu_acc.task_id));
                    
                    // Log current registered bounds for debugging
                    LOG_DXRT_S_ERR("Currently registered bounds for this process/device:");
                    for (const auto& boundPair : _devInfo[clientMessage.pid][clientMessage.deviceId]) {
                        LOG_DXRT_S_ERR("  Bound " + std::to_string(boundPair.first) + " (count: " + std::to_string(boundPair.second) + ")");
                    }
                    
                    dxrt::dxrt_response_t resp{};
                    resp.req_id = clientMessage.npu_acc.req_id;
                    resp.proc_id = clientMessage.npu_acc.proc_id;
                    resp.status = -1;
                    onCompleteInference(resp, clientMessage.deviceId);
                    return;
                }
                
                // Check if device is blocked before adding to scheduler
                if (static_cast<uint32_t>(clientMessage.deviceId) < _devices.size() && _devices[clientMessage.deviceId]->isBlocked()) {
                    LOG_DXRT_S_ERR("Device " + std::to_string(clientMessage.deviceId) + " is blocked, rejecting inference request");
                    dxrt::dxrt_response_t resp{};
                    resp.req_id = clientMessage.npu_acc.req_id;
                    resp.proc_id = clientMessage.npu_acc.proc_id;
                    resp.status = -2;  // Device blocked error
                    onCompleteInference(resp, clientMessage.deviceId);
                    return;
                }
                
                LOG_DXRT_S_DBG << "Inference request validation passed, adding to scheduler" << endl;
            }
            
            _scheduler->AddScheduler(clientMessage.npu_acc, clientMessage.deviceId);
            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_INIT: {
            int deviceId = clientMessage.deviceId;
            int bound = clientMessage.data;
            dxrt::dxrt_custom_weight_info_t info;
            info.address = clientMessage.npu_acc.datas[0];
            info.size = clientMessage.npu_acc.datas[1];
            info.checksum = clientMessage.npu_acc.datas[2];

            InitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                _devInfo[clientMessage.pid][deviceId][bound]++;
                _weightInfo[clientMessage.pid][deviceId].insert(info);
                _devices[deviceId]->DoCustomCommand(&info, dxrt::dxrt_custom_sub_cmt_t::DX_ADD_WEIGHT_INFO, sizeof(dxrt::dxrt_custom_weight_info_t));
            }
            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_DEINIT: {
            int deviceId = clientMessage.deviceId;
            int bound = clientMessage.data;
            dxrt::dxrt_custom_weight_info_t info;
            info.address = clientMessage.npu_acc.datas[0];
            info.size = clientMessage.npu_acc.datas[1];
            info.checksum = clientMessage.npu_acc.datas[2];
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                _devInfo[clientMessage.pid][deviceId][bound]--;
                if (_devInfo[clientMessage.pid][deviceId][bound] == 0)
                {
                    _devInfo[clientMessage.pid][deviceId].erase(bound);
                    _weightInfo[clientMessage.pid][deviceId].erase(info);
                    _devices[deviceId]->DoCustomCommand(&info, dxrt::dxrt_custom_sub_cmt_t::DX_DEL_WEIGHT_INFO, sizeof(dxrt::dxrt_custom_weight_info_t));
                }

            }
            DeInitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
            return;
        }
        case dxrt::REQUEST_CODE::TASK_INIT: {
            int deviceId = clientMessage.deviceId;
            int taskId = clientMessage.taskId;
            int bound = clientMessage.data;
            uint64_t modelMemorySize = clientMessage.modelMemorySize;

            LOG_DXRT_S << "Task Init - DevId: " << deviceId << ", TaskId: " << taskId
                      << ", PID: " << clientMessage.pid << ", Bound: " << bound << ", Model MemSize: " << modelMemorySize << endl;

            // Enhanced memory availability check before task initialization
            auto memService = dxrt::MemoryService::getInstance(deviceId);
            if (memService != nullptr) {
                uint64_t freeSize = memService->free_size();
                uint64_t usedSize = memService->used_size();

                LOG_DXRT_S << "Device " << deviceId << " Memory Status - Free: " << (freeSize / (1024*1024))
                           << "MB, Used: " << (usedSize / (1024*1024)) << "MB, Required: " << (modelMemorySize / (1024*1024)) << "MB" << endl;

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
                        return;
                    }
                }
            }

            {
                std::lock_guard<std::mutex> lock(_deviceMutex);

                // Check if task already exists
                auto& pidTasks = _taskInfo[clientMessage.pid];
                auto& deviceTasks = pidTasks[deviceId];
                if (deviceTasks.find(taskId) != deviceTasks.end()) {
                    LOG_DXRT_S_ERR("Task " + std::to_string(taskId) + " already exists for PID " +
                                   std::to_string(clientMessage.pid) + " on device " + std::to_string(deviceId));
                    return;
                }

                // Check device availability before task initialization
                if (deviceId >= static_cast<int>(_devices.size())) {
                    LOG_DXRT_S_ERR("Invalid device ID: " + std::to_string(deviceId));
                    return;
                }
                
                if (_devices[deviceId]->isBlocked()) {
                    LOG_DXRT_S_ERR("Device " + std::to_string(deviceId) + " is blocked, cannot initialize task");
                    return;
                }
                
                // Enhanced NPU bound option validation with 3-type limit check
                std::set<int> currentBoundTypes;
                for (const auto& pidPair : _devInfo) {
                    auto deviceIt = pidPair.second.find(deviceId);
                    if (deviceIt != pidPair.second.end()) {
                        for (const auto& boundPair : deviceIt->second) {
                            if (boundPair.second > 0) { // Only count active bounds
                                currentBoundTypes.insert(boundPair.first);
                            }
                        }
                    }
                }
                
                LOG_DXRT_S << "Device " << deviceId << " current bound types: ";
                for (int boundType : currentBoundTypes) {
                    LOG_DXRT_S << boundType << " ";
                }
                LOG_DXRT_S << "(total: " << currentBoundTypes.size() << " types)" << endl;
                
                // Check if adding this bound would exceed the limit
                bool isNewBoundType = (currentBoundTypes.find(bound) == currentBoundTypes.end());
                if (isNewBoundType && currentBoundTypes.size() >= 3) {
                    LOG_DXRT_S_ERR("Cannot add NPU bound " + std::to_string(bound) + 
                                   " to device " + std::to_string(deviceId) + 
                                   " - Maximum 3 different bound types allowed per device. " +
                                   "Current types: " + std::to_string(currentBoundTypes.size()));
                    
                    LOG_DXRT_S_ERR("Current bound types on device " + std::to_string(deviceId) + ":");
                    for (int boundType : currentBoundTypes) {
                        LOG_DXRT_S_ERR("  Bound type " + std::to_string(boundType));
                    }
                    
                    // Send error response for bound limit exceeded
                    return;
                }
                
                auto& devInfo = _devInfo[clientMessage.pid][deviceId];
                if (devInfo.find(bound) != devInfo.end()) {
                    LOG_DXRT_S << "NPU bound " << bound << " already in use by PID " << clientMessage.pid 
                               << " on device " << deviceId << ", count: " << devInfo[bound] << endl;
                } else if (isNewBoundType) {
                    LOG_DXRT_S << "Adding new NPU bound type " << bound << " to device " << deviceId 
                               << " (will be " << (currentBoundTypes.size() + 1) << "/3 bound types)" << endl;
                }
                
                // Check for potential conflicts with other processes
                for (const auto& pidPair : _devInfo) {
                    if (pidPair.first != clientMessage.pid) { // Different process
                        auto deviceIt = pidPair.second.find(deviceId);
                        if (deviceIt != pidPair.second.end()) {
                            auto boundIt = deviceIt->second.find(bound);
                            if (boundIt != deviceIt->second.end() && boundIt->second > 0) {
                                LOG_DXRT_S << "NPU bound " << bound << " is also used by PID " << pidPair.first 
                                           << " on device " << deviceId << " (shared usage)" << endl;
                            }
                        }
                    }
                }
                
                deviceTasks[taskId] = std::make_pair(bound, modelMemorySize);
                
                // Register bound in _devInfo for inference request validation
                _devInfo[clientMessage.pid][deviceId][bound]++;
                
                LOG_DXRT_S << "NPU Bound " << bound << " registered for PID " << clientMessage.pid 
                           << " on device " << deviceId << ", usage count: " << _devInfo[clientMessage.pid][deviceId][bound] << endl;

                auto targetDevice = _devices[deviceId];
                int ret = targetDevice->BoundOption(dxrt::DX_SCHED_ADD, static_cast<dxrt::npu_bound_op>(bound));
                if (ret != 0) {
                    LOG_DXRT_S_ERR("Failed to set NPU bound " + std::to_string(bound) + 
                                   " for device " + std::to_string(deviceId) + ", ret: " + std::to_string(ret));
                    // Rollback
                    deviceTasks.erase(taskId);
                    _devInfo[clientMessage.pid][deviceId][bound]--;
                    if (_devInfo[clientMessage.pid][deviceId][bound] <= 0) {
                        _devInfo[clientMessage.pid][deviceId].erase(bound);
                    }
                    return;
                } else {
                    LOG_DXRT_S << "Successfully set NPU bound " << bound << " for device " << deviceId << endl;
                }
                
                // Log final bound status for this device
                currentBoundTypes.clear();
                for (const auto& pidPair : _devInfo) {
                    auto deviceIt = pidPair.second.find(deviceId);
                    if (deviceIt != pidPair.second.end()) {
                        for (const auto& boundPair : deviceIt->second) {
                            if (boundPair.second > 0) {
                                currentBoundTypes.insert(boundPair.first);
                            }
                        }
                    }
                }
                LOG_DXRT_S << "Device " << deviceId << " final bound types: " << currentBoundTypes.size() << "/3" << endl;
            }

            PrintManagedTasks();
            break;
        }
        case dxrt::REQUEST_CODE::TASK_DEINIT: {
            int deviceId = clientMessage.deviceId;
            int taskId = clientMessage.taskId;
            int bound = clientMessage.data;
            
            LOG_DXRT_S << "Task DeInit - DevId: " << deviceId << ", TaskId: " << taskId 
                      << ", PID: " << clientMessage.pid << ", Bound: " << bound << endl;
            
            // Enhanced Task cleanup with better synchronization
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                
                // Log current state before cleanup
                LOG_DXRT_S << "Before cleanup - PID " << clientMessage.pid << " has " 
                           << _taskInfo[clientMessage.pid][deviceId].size() << " tasks on device " << deviceId << endl;
                
                // Stop any ongoing inference requests for this Task
                _scheduler->StopTaskInference(clientMessage.pid, deviceId, taskId);
                
                auto pidIt = _taskInfo.find(clientMessage.pid);
                if (pidIt != _taskInfo.end())
                {
                    auto deviceIt = pidIt->second.find(deviceId);
                    if (deviceIt != pidIt->second.end())
                    {
                        auto taskIt = deviceIt->second.find(taskId);
                        if (taskIt != deviceIt->second.end()) {
                            LOG_DXRT_S << "Removing Task " << taskId << " from device " << deviceId 
                                       << " for PID " << clientMessage.pid << endl;
                            deviceIt->second.erase(taskIt);
                        } else {
                            LOG_DXRT_S_ERR("Task " + std::to_string(taskId) + " not found for cleanup");
                        }
                        
                        // Clean up device binding if no more tasks
                        if (deviceIt->second.empty()) {
                            pidIt->second.erase(deviceIt);
                            LOG_DXRT_S << "No more tasks for device " << deviceId << ", removed device entry for PID " << clientMessage.pid << endl;
                        }
                    }
                    
                    // Clean up PID if no more devices
                    if (pidIt->second.empty()) {
                        _taskInfo.erase(pidIt);
                        LOG_DXRT_S << "No more devices for PID " << clientMessage.pid << ", removed PID entry" << endl;
                    }
                }
                
                // Enhanced bound management with reference counting and 3-type limit tracking
                auto devInfoPidIt = _devInfo.find(clientMessage.pid);
                if (devInfoPidIt != _devInfo.end())
                {
                    auto devInfoDeviceIt = devInfoPidIt->second.find(deviceId);
                    if (devInfoDeviceIt != devInfoPidIt->second.end())
                    {
                        auto boundIt = devInfoDeviceIt->second.find(bound);
                        if (boundIt != devInfoDeviceIt->second.end())
                        {
                            LOG_DXRT_S << "Current NPU bound " << bound << " usage count: " << boundIt->second << endl;
                            
                            boundIt->second--;
                            if (boundIt->second <= 0)
                            {
                                devInfoDeviceIt->second.erase(bound);
                                LOG_DXRT_S << "Removed NPU bound " << bound << " from PID " << clientMessage.pid 
                                           << " on device " << deviceId << " (count reached 0)" << endl;
                                
                                // Check if other processes or tasks still use this bound
                                bool otherProcessesUsingBound = false;
                                for (const auto& otherPidPair : _devInfo) {
                                    if (otherPidPair.first != clientMessage.pid) {
                                        auto otherDeviceIt = otherPidPair.second.find(deviceId);
                                        if (otherDeviceIt != otherPidPair.second.end()) {
                                            if (otherDeviceIt->second.find(bound) != otherDeviceIt->second.end()) {
                                                otherProcessesUsingBound = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                
                                // Check if other tasks in same process still use this bound
                                bool otherTasksUsingBound = false;
                                if (_taskInfo.find(clientMessage.pid) != _taskInfo.end()) {
                                    auto& deviceTasks = _taskInfo[clientMessage.pid][deviceId];
                                    for (const auto& taskPair : deviceTasks) {
                                        if (taskPair.second.first == bound) {
                                            otherTasksUsingBound = true;
                                            break;
                                        }
                                    }
                                }
                                
                                if (!otherProcessesUsingBound && !otherTasksUsingBound) {
                                    auto targetDevice = _devices[deviceId];
                                    int ret = targetDevice->BoundOption(dxrt::DX_SCHED_DELETE, static_cast<dxrt::npu_bound_op>(bound));
                                    if (ret == 0) {
                                        LOG_DXRT_S << "Released NPU bound " << bound << " from device " << deviceId 
                                                   << " (no other users)" << endl;
                                        
                                        // Log updated bound type count
                                        std::set<int> remainingBoundTypes;
                                        for (const auto& pidPair : _devInfo) {
                                            auto deviceIt = pidPair.second.find(deviceId);
                                            if (deviceIt != pidPair.second.end()) {
                                                for (const auto& boundPair : deviceIt->second) {
                                                    if (boundPair.second > 0) {
                                                        remainingBoundTypes.insert(boundPair.first);
                                                    }
                                                }
                                            }
                                        }
                                        LOG_DXRT_S << "Device " << deviceId << " now has " << remainingBoundTypes.size() 
                                                   << "/3 bound types after releasing bound " << bound << endl;
                                        
                                    } else {
                                        LOG_DXRT_S_ERR("Failed to release NPU bound " + std::to_string(bound) + 
                                                       " from device " + std::to_string(deviceId) + ", ret: " + std::to_string(ret));
                                    }
                                } else {
                                    LOG_DXRT_S << "Keeping NPU bound " << bound << " on device " << deviceId 
                                               << " (other users: processes=" << (otherProcessesUsingBound ? "yes" : "no") 
                                               << ", tasks=" << (otherTasksUsingBound ? "yes" : "no") << ")" << endl;
                                }
                            } else {
                                LOG_DXRT_S << "NPU bound " << bound << " usage count decreased to " << boundIt->second << endl;
                            }
                        } else {
                            LOG_DXRT_S_ERR("NPU bound " + std::to_string(bound) + " not found for PID " + 
                                           std::to_string(clientMessage.pid) + " device " + std::to_string(deviceId));
                        }
                        
                        // Clean up device entry if no bounds left
                        if (devInfoDeviceIt->second.empty()) {
                            devInfoPidIt->second.erase(devInfoDeviceIt);
                            LOG_DXRT_S << "No more bounds for device " << deviceId << ", removed device entry from devInfo" << endl;
                        }
                    }
                    
                    // Clean up PID entry if no devices left
                    if (devInfoPidIt->second.empty()) {
                        _devInfo.erase(devInfoPidIt);
                        LOG_DXRT_S << "No more devices in devInfo for PID " << clientMessage.pid << ", removed PID entry" << endl;
                    }
                }
            }
            
            PrintManagedTasks();
            break;
        }
        case dxrt::REQUEST_CODE::DEALLOCATE_TASK_MEMORY: {
            int deviceId = clientMessage.deviceId;
            int taskId = clientMessage.taskId;
            pid_t pid = clientMessage.pid;
            
            LOG_DXRT_S << "Deallocate Task Memory - DevId: " << deviceId << ", TaskId: " << taskId 
                      << ", PID: " << pid << endl;
            
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
                    LOG_DXRT_S << "Successfully deallocated memory for Task " << taskId 
                               << ", PID: " << pid << ", Device: " << deviceId << endl;
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
            return;
        }
        case dxrt::REQUEST_CODE::PROCESS_DEINIT: {
            int deviceId = clientMessage.deviceId;
            int pid = clientMessage.pid;
            
            LOG_DXRT_S << "Process DeInit - DevId: " << deviceId << ", PID: " << pid << endl;
            
            // Enhanced process cleanup with better validation
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                
                // Log current state before cleanup
                LOG_DXRT_S << "Process cleanup - PID " << pid << " task count on device " << deviceId << ": " 
                           << (_taskInfo.find(pid) != _taskInfo.end() && _taskInfo[pid].find(deviceId) != _taskInfo[pid].end() 
                               ? _taskInfo[pid][deviceId].size() : 0) << endl;
                
                // Stop all inference requests for this process
                _scheduler->StopAllInferenceForProcess(pid, deviceId);
                
                // Cleanup all tasks for this process on this device
                auto pidIt = _taskInfo.find(pid);
                if (pidIt != _taskInfo.end())
                {
                    auto deviceIt = pidIt->second.find(deviceId);
                    if (deviceIt != pidIt->second.end())
                    {
                        LOG_DXRT_S << "Cleaning up " << deviceIt->second.size() << " tasks for process " << pid 
                                   << " on device " << deviceId << endl;
                        
                        // First stop all tasks, then cleanup
                        for (const auto& taskPair : deviceIt->second) {
                            int taskId = taskPair.first;
                            _scheduler->StopTaskInference(pid, deviceId, taskId);
                        }
                        
                        deviceIt->second.clear();
                        pidIt->second.erase(deviceIt);
                        
                        LOG_DXRT_S << "All tasks cleaned up for process " << pid << " on device " << deviceId << endl;
                    }
                    
                    // Clean up PID if no more devices
                    if (pidIt->second.empty()) {
                        _taskInfo.erase(pidIt);
                        LOG_DXRT_S << "No more devices for PID " << pid << ", removed PID entry from taskInfo" << endl;
                    }
                }
                
                // Enhanced bound option cleanup
                auto devInfoPidIt = _devInfo.find(pid);
                if (devInfoPidIt != _devInfo.end())
                {
                    auto devInfoDeviceIt = devInfoPidIt->second.find(deviceId);
                    if (devInfoDeviceIt != devInfoPidIt->second.end())
                    {
                        LOG_DXRT_S << "Releasing " << devInfoDeviceIt->second.size() << " bound options for process " << pid 
                                   << " on device " << deviceId << endl;
                        
                        // Release all bound options for this process/device
                        for (const auto& boundPair : devInfoDeviceIt->second) {
                            int bound = boundPair.first;
                            
                            // Check if other processes still use this bound
                            bool otherProcessesUsingBound = false;
                            for (const auto& otherPidPair : _devInfo) {
                                if (otherPidPair.first != pid) {
                                    auto otherDeviceIt = otherPidPair.second.find(deviceId);
                                    if (otherDeviceIt != otherPidPair.second.end()) {
                                        if (otherDeviceIt->second.find(bound) != otherDeviceIt->second.end()) {
                                            otherProcessesUsingBound = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            
                            if (!otherProcessesUsingBound) {
                                auto targetDevice = _devices[deviceId];
                                int ret = targetDevice->BoundOption(dxrt::DX_SCHED_DELETE, static_cast<dxrt::npu_bound_op>(bound));
                                if (ret == 0) {
                                    LOG_DXRT_S << "Released NPU bound " << bound << " from device " << deviceId 
                                               << " (no other processes using)" << endl;
                                } else {
                                    LOG_DXRT_S_ERR("Failed to release NPU bound " + std::to_string(bound) + 
                                                   " from device " + std::to_string(deviceId) + ", ret: " + std::to_string(ret));
                                }
                            } else {
                                LOG_DXRT_S << "Keeping NPU bound " << bound << " on device " << deviceId 
                                           << " (other processes still using)" << endl;
                            }
                        }
                        
                        devInfoPidIt->second.erase(devInfoDeviceIt);
                        LOG_DXRT_S << "Removed device " << deviceId << " from devInfo for process " << pid << endl;
                    }
                    
                    // Clean up PID entry if no devices left
                    if (devInfoPidIt->second.empty()) {
                        _devInfo.erase(devInfoPidIt);
                        LOG_DXRT_S << "No more devices in devInfo for PID " << pid << ", removed PID entry" << endl;
                    }
                }
            }
            
            // Deallocate all device memory for this process
            auto memService = dxrt::MemoryService::getInstance(deviceId);
            if (memService != nullptr) {
                bool memoryReleased = memService->DeallocateAllForProcess(pid);
                if (memoryReleased) {
                    LOG_DXRT_S << "Deallocated all memory for process " << pid << " on device " << deviceId << endl;
                    
                } else {
                    LOG_DXRT_S_DBG << "No memory to deallocate for process " << pid << " on device " << deviceId << endl;
                }
            }
            
            PrintManagedTasks();
            break;
        }
        case dxrt::REQUEST_CODE::DEVICE_RESET: {
            return;
        }
        case dxrt::REQUEST_CODE::INFERENCE_COMPLETED: {
            return;
        }
        case dxrt::REQUEST_CODE::VIEW_FREE_MEMORY: {
            const dxrt::MemoryService* instance = dxrt::MemoryService::getInstance(clientMessage.deviceId);
            if (instance == nullptr)
            {
                serverMessage.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
                serverMessage.data = 0;
                serverMessage.result = UINT_MAX_CONST;
            }
            else
            {
                auto result = instance->free_size();
                serverMessage.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
                serverMessage.data = result;
                serverMessage.result = 0;
            }
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            break;
        }
        case dxrt::REQUEST_CODE::VIEW_USED_MEMORY: {
            const dxrt::MemoryService* instance = dxrt::MemoryService::getInstance(clientMessage.deviceId);
            if (instance == nullptr)
            {
                serverMessage.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
                serverMessage.data = 0;
                serverMessage.result = UINT_MAX_CONST;
            }
            else
            {
                auto result = instance->used_size();
                serverMessage.code = dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT;
                serverMessage.data = result;
                serverMessage.result = 0;
            }
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            break;
        }
        case dxrt::REQUEST_CODE::VIEW_AVAILABLE_DEVICE:
        {
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
            serverMessage.code = dxrt::RESPONSE_CODE::VIEW_AVAILABLE_DEVICE_RESULT;
            serverMessage.data = result;
            serverMessage.result = 0;
            
            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
            break;
        }
        case dxrt::REQUEST_CODE::GET_USAGE:
        {
            double result = _devices[clientMessage.deviceId]->getUsage(clientMessage.data);
            serverMessage.code = dxrt::RESPONSE_CODE::GET_USAGE_RESULT;
            serverMessage.data = result * 1000;
            serverMessage.result = 0;

            serverMessage.deviceId = clientMessage.deviceId;
            serverMessage.msgType = clientMessage.msgType;
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
    auto pidIt = _taskInfo.find(pid);
    if (pidIt == _taskInfo.end()) {
        return false;
    }
    
    auto deviceIt = pidIt->second.find(deviceId);
    if (deviceIt == pidIt->second.end()) {
        return false;
    }
    
    bool taskExists = deviceIt->second.find(taskId) != deviceIt->second.end();
    
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
    std::lock_guard<std::mutex> lock(_deviceMutex);
    
    LOG_DXRT_S << "==================== Managed Tasks Report ====================" << endl;
    if (_taskInfo.empty()) {
        LOG_DXRT_S << "  No tasks are currently managed by the service." << endl;
    } else {
        for (const auto& pid_pair : _taskInfo) {
            pid_t pid = pid_pair.first;
            const auto& device_map = pid_pair.second;

            LOG_DXRT_S << "  [PID: " << pid << "]" << endl;
            
            if (device_map.empty()) {
                LOG_DXRT_S << "    -> No devices associated with this process." << endl;
            } else {
                for (const auto& device_pair : device_map) {
                    int deviceId = device_pair.first;
                    const auto& task_set = device_pair.second;
                    
                    if (task_set.empty()) {
                        LOG_DXRT_S << "    - Device ID: " << deviceId << " -> No tasks." << endl;
                    } else {
                        std::stringstream ss;
                        bool first = true;
                        for (const auto& task_pair : task_set) {
                            int taskId = task_pair.first;
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
        }
    }
    LOG_DXRT_S << "============================================================" << endl;
}

void DxrtService::dequeueAllClientMessageQueue(long msgType)
{
    dxrt::IPCClientWrapper clientWrapper(dxrt::IPCDefaultType(), msgType);
    clientWrapper.ClearMessages();  // clear remained messages
    clientWrapper.Close();  // close
}

int DxrtService::GetDeviceIdByProcId(int procId)
{
    auto it = _devInfo.find(procId);
    int deviceId = -1;

    if (it != _devInfo.end()) {
        auto &deviceMap = it->second;
        for (const auto &device : deviceMap) {
            deviceId = device.first;
        }
    } else {
        LOG_DXRT_S_ERR("Process ID not found.");
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
    
    ret = _devices[devId]->BoundOption(dxrt::DX_SCHED_ADD, static_cast<dxrt::npu_bound_op>(bound));
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
    LOG_DXRT_S << "DevId : " << devId << ", delete bound : " << bound << endl;
    ret = _devices[devId]->BoundOption(dxrt::DX_SCHED_DELETE, static_cast<dxrt::npu_bound_op>(bound));
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
        std::lock_guard<std::mutex> lock(_deviceMutex);
        {
            for (const auto& dev : _devInfo[procId])
            {
                devId = dev.first;
                const auto& bounds = dev.second;
                for (auto boundIt : bounds)
                {
                    int bound = boundIt.first;
                    LOG_DXRT_S << "DevId : " << devId << ", delete bound : " << bound << endl;
                    int ret = _devices[devId]->BoundOption(
                        dxrt::DX_SCHED_DELETE, static_cast<dxrt::npu_bound_op>(bound));
                    if (ret != 0)
                    {
                        LOG_DXRT_S_ERR("failed to apply bound option to device");
                        return 2;
                    }
                }
            }
            _devInfo[procId].clear();
            _devInfo.erase(procId);
        }
        return 0;
    } catch (const std::exception& e) {
        std::string str = std::string("Exception occurred: ") + e.what();
        LOG_DXRT_S_ERR(str);
        return 999;
    }
    // no need to return since all block has return
}

#ifdef _WIN32

bool IsProcessRunning(DWORD procId) {
    
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
        else {
            CloseHandle(hProcess);
            return false;
        }
    }
    else {
        LOG_DXRT_ERR("GetExitCodeProcess failed for PID " << procId << ". Error: " << GetLastError())
            CloseHandle(hProcess);
        return false;
    }
}
#endif
void DxrtService::handle_process_die(DxrtService *service)
{
#ifdef __linux__
    for (auto it = _pid_set.begin(); it != _pid_set.end(); )
    {
        pid_t pid = *it;

        if (kill(pid, 0) == 0) {
            ++it;
        } else {
            if (errno == ESRCH) {
                LOG_DXRT_S << "Process " << pid << " died, starting cleanup" << endl;
                
                // Enhanced cleanup sequence with better synchronization
                
                // 1. Stop scheduler first (no lock) - prevents new requests
                _scheduler->StopScheduler(pid);
                dequeueAllClientMessageQueue(pid);
                
                // 2. Clean up Task metadata with enhanced synchronization (Lock order: _deviceMutex first)
                {
                    std::lock_guard<std::mutex> lock(_deviceMutex);
                    auto pidTaskIt = _taskInfo.find(pid);
                    if (pidTaskIt != _taskInfo.end())
                    {
                        // Enhanced cleanup for task-based processes
                        for (auto& devicePair : pidTaskIt->second)
                        {
                            int deviceId = devicePair.first;
                            for (auto& taskPair : devicePair.second)
                            {
                                int taskId = taskPair.first;
                                int bound = taskPair.second.first;
                                
                                // Stop any ongoing inference for this task
                                _scheduler->StopTaskInference(pid, deviceId, taskId);
                                
                                // Clean up device binding
                                if (_devInfo[pid][deviceId].count(bound))
                                {
                                    _devInfo[pid][deviceId][bound]--;
                                    if (_devInfo[pid][deviceId][bound] <= 0)
                                    {
                                        _devInfo[pid][deviceId].erase(bound);
                                    }
                                }
                            }
                        }
                        
                        // Clean up device info
                        auto devInfoIt = _devInfo.find(pid);
                        if (devInfoIt != _devInfo.end())
                        {
                            for (auto it = devInfoIt->second.begin(); it != devInfoIt->second.end();)
                            {
                                if (it->second.empty()) { it = devInfoIt->second.erase(it); }
                                else { ++it; }
                            }
                            if (devInfoIt->second.empty()) { _devInfo.erase(devInfoIt); }
                        }
                        
                        _taskInfo.erase(pidTaskIt);
                        LOG_DXRT_S << "Process " << pid << ": Cleaned up task and device metadata" << endl;
                    }
                }
                
                // 3. Deallocate memory with enhanced safety (separate lock to avoid deadlocks)
                dxrt::MemoryService::DeallocateAllDevice(pid);
                LOG_DXRT_S << "Process " << pid << ": Deallocated all device memory" << endl;
                
                // 4. Clean up scheduler state
                _scheduler->cleanDiedProcess(pid);
                it = _pid_set.erase(it);
                
                // 5. Clean up device state with enhanced error handling (run separately async)
                {
                    std::future<long> result = std::async(std::launch::async, &DxrtService::ClearDevice, service, pid);
                    long errCode = result.get();
                    _scheduler->StartScheduler(pid);
                    _scheduler->ClearProcLoad(pid);
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
                
                LOG_DXRT_S << "Process " << pid << ": Cleanup completed" << endl;
            } else if (errno == EPERM) {
                ++it;
            } else {
                perror("kill");
                ++it;
            }
        }
    }
#elif _WIN32
    for (auto it = _pid_set.begin(); it != _pid_set.end(); )
    {
        pid_t pid = *it;

        if (IsProcessRunning(pid)) {
            ++it;
        }
        else {
            _scheduler->StopScheduler(pid);
            dequeueAllClientMessageQueue(pid);
            dxrt::MemoryService::DeallocateAllDevice(pid);
            _scheduler->cleanDiedProcess(pid);
            it = _pid_set.erase(it);
            {
                long errCode = service->ClearDevice(pid);
                _scheduler->StartScheduler(pid);
                _scheduler->ClearProcLoad(pid);
                if (errCode != 0)
                {
                    if (errCode == 1)
                        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_TERMINATION, errCode, -1);
                    if (errCode == 2)
                        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, errCode, -1);
                    else
                        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_UNKNOWN_ERR, errCode, -1);
                    // DXRT_ASSERT(false, "failed device termination");
                }
            }
        }
    }
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
        handle_process_die(this);
        
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
    cout << scheduler_option_str << endl;

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
