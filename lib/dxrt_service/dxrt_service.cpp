// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"


#include <csignal>
#include <atomic>
#include <thread>
#include <future>
#include <unordered_set>
#include <map>
#include "memory_service.hpp"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "cxxopts.hpp"
#include "service_device.h"
#include "scheduler_service.h"
#include "service_error.h"


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

 public:
    void Process(dxrt::IPCClientMessage& clientMessage);
    explicit DxrtService(DXRT_Schedule scheduler_option = DXRT_Schedule::FIFO);
    explicit DxrtService(std::vector<std::shared_ptr<dxrt::ServiceDevice> > devices_, DXRT_Schedule scheduler_option);
    void onCompleteInference(const dxrt::dxrt_response_t& response, int deviceId);
    void ErrorBroadCastToClient(dxrt::dxrt_server_err_t err, uint32_t errCode);

    void InitDevice(int devId, dxrt::npu_bound_op bound);
    void DeInitDevice(int devId, dxrt::npu_bound_op bound);
    long ClearDevice(int procId);
    void handle_process_die(DxrtService *service);
    void die_check_thread();
    int GetDeviceIdByProcId(int procId);
    void Dispose();

    dxrt::IPCServerWrapper _ipcServerWrapper;
    std::vector<std::shared_ptr<dxrt::ServiceDevice> > _devices;
    std::shared_ptr<SchedulerService> _scheduler;

    std::set<pid_t> _pid_set;
    /* process id / device id / bounding option */
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> _devInfo;
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
    _scheduler->SetErrorCallback([this](dxrt::dxrt_server_err_t err, uint32_t errCode) {
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

    serverMessage.msgType = clientMessage.msgType;
    LOG_DXRT_S_DBG << "client-message code=" << _s(code).c_str() << endl;
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
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                if (_devInfo[clientMessage.pid][clientMessage.deviceId].find(clientMessage.npu_acc.bound) == _devInfo[clientMessage.pid][clientMessage.deviceId].end())
                {
                    LOG_DXRT_S_ERR("Process " + std::to_string(clientMessage.pid) + " device " + std::to_string(clientMessage.deviceId)
                         + ": unregistered bound " + std::to_string(clientMessage.npu_acc.bound));
                    return;
                }
            }
            _scheduler->AddScheduler(clientMessage.npu_acc, clientMessage.deviceId);

            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_INIT: {
            int deviceId = clientMessage.deviceId;
            int bound = clientMessage.data;

            InitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                _devInfo[clientMessage.pid][deviceId].insert(bound);
            }
            return;
        }
        case dxrt::REQUEST_CODE::DEVICE_DEINIT: {
            int deviceId = clientMessage.deviceId;
            int bound = clientMessage.data;
            {
                std::lock_guard<std::mutex> lock(_deviceMutex);
                _devInfo[clientMessage.pid][deviceId].erase(bound);
            }
            DeInitDevice(deviceId, static_cast<dxrt::npu_bound_op>(bound));
            return;
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
                serverMessage.result = UINT_MAX;
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
                serverMessage.result = UINT_MAX;
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
    dxrt::IPCClientWrapper clientWrapper(dxrt::IPCDefaultType(), msgType);
    clientWrapper.ClearMessages();  // clear remained messages
    clientWrapper.Close();  // close
}

int DxrtService::GetDeviceIdByProcId(int procId)
{
    auto it = _devInfo.find(procId);
    int deviceId = -1;

    if (it != _devInfo.end()) {
        std::unordered_map<int, std::unordered_set<int>> &deviceMap = it->second;
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
    ret = _devices[devId]->BoundOption(dxrt::DX_SCHED_ADD, static_cast<dxrt::npu_bound_op>(bound));
    if (ret != 0)
    {
        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, ret);
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
        ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, ret);
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
            std::this_thread::sleep_for(chrono::microseconds(10));
        }
        std::lock_guard<std::mutex> lock(_deviceMutex);
        {
            for (const auto& dev : _devInfo[procId])
            {
                devId = dev.first;
                const unordered_set<int>& bounds = dev.second;
                for (int bound : bounds)
                {
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
                _scheduler->StopScheduler(pid);
                dequeueAllClientMessageQueue(pid);
                dxrt::MemoryService::DeallocateAllDevice(pid);
                _scheduler->cleanDiedProcess(pid);
                it = _pid_set.erase(it);
                {
                    std::future<long> result = std::async(std::launch::async, &DxrtService::ClearDevice, service, pid);
                    long errCode = result.get();
                    _scheduler->StartScheduler(pid);
                    _scheduler->ClearProcLoad(pid);
                    if (errCode != 0)
                    {
                        if (errCode == 1)
                            ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_TERMINATION, errCode);
                        if (errCode == 2)
                            ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR, errCode);
                        else
                            ErrorBroadCastToClient(dxrt::dxrt_server_err_t::S_ERR_SERVICE_UNKNOWN_ERR, errCode);
                        // DXRT_ASSERT(false, "failed device termination");
                    }
                }
            } else if (errno == EPERM) {
                ++it;
            } else {
                perror("kill");
                ++it;
            }
        }
    }
#elif _WIN32
    // not implemented
#endif
}

void DxrtService::die_check_thread()
{
    LOG_DXRT_S << "Started client process status check thread" << std::endl;

    while (true)
    {
        std::this_thread::sleep_for(chrono::seconds(1));
        handle_process_die(this);
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
        ("s, scheduler", "Scheduler Mode(FIFO, RoundRobin, SJF)", cxxopts::value<string>(scheduler_option_str));

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

#ifdef __linux__
    std::thread th(&DxrtService::die_check_thread, &service);

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
