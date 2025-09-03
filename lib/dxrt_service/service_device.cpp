/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "service_device.h"


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef __linux__
    #include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/ioctl.h>
#endif
#include <sys/types.h>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <string>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <utility>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver_adapter/driver_adapter.h"

#include "dxrt/profiler.h"
#ifdef __linux__
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#else
#include "dxrt/driver_adapter/windows_driver_adapter.h"
#endif

using std::vector;
using std::cout;
using std::endl;
using std::to_string;

namespace dxrt {



ServiceDevice::ServiceDevice(const string &file_)
: _file(file_), _profiler(Profiler::GetInstance())
{
    _name = string(_file);  // temp.
    LOG_DXRT_S_DBG << "Device created from " << _name << endl;
#ifdef __linux__
#elif _WIN32
    // _driverAdapter = make_shared<WindowsDriverAdapter>(_file);
#endif
    for (int i = 0; i < static_cast<int>(dxrt::npu_bound_op::N_BOUND_INF_MAX); i++)
    {
        _bound_count[i] = 0;
    }
    _callBack = nullptr;
}



ServiceDevice::~ServiceDevice(void)
{
    _stop.store(true);

    Terminate();
    if (_thread[0].joinable())
    {
        _thread[0].join();
        _thread[1].join();
        _thread[2].join();
    }
}

// #define ServiceDevice_DEBUG

#ifdef ServiceDevice_DEBUG
// usage
// static auto start = std::chrono::high_resolution_clock::now();
// ...
// start = durationPrint(start, "IPCPipeWindows::SendOL :");
static std::chrono::steady_clock::time_point durationPrint1(std::chrono::steady_clock::time_point start, const char* msg)
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    double total_time = duration.count();
    double avg_latency = total_time / 1;
    // if (avg_latency > 100)
        LOG_DXRT_S_DBG << msg << avg_latency << " ms" << std::endl;
    return end;
}
#endif

int ServiceDevice::Process(dxrt_cmd_t cmd, void *data, uint32_t size, uint32_t sub_cmd)
{
    int ret = 0;

    if (cmd == dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY)
        LOG_DXRT_S << _id << ": Send recovery command" << endl;
#ifdef __linux__
    ret = _driverAdapter->IOControl(cmd, data, size, sub_cmd);
    if (ret < 0)
        ret = errno*(-1);
#elif _WIN32
    ret = _driverAdapter->IOControl(cmd, data, size, sub_cmd);
#if 0
    DWORD bytesReturned;
    BOOL success = DeviceIoControl(
        (HANDLE)_devFd,
        static_cast<DWORD>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE),
        data,
        size,
        NULL,
        0,
        &bytesReturned,
        NULL);
    if (!success) {
        ret = GetLastError() * (-1);
    }
    else
    {
        ret = bytesReturned;
    }
#endif
#endif
    return ret;
}



void ServiceDevice::Identify(int id_, uint32_t subCmd )
{
    LOG_DXRT_S_DBG << "Device " << _id << " Identify" << endl;
    std::lock_guard<std::mutex> lock(_lock);
    int ret;
    _id = id_;
#ifdef __linux__
    _driverAdapter = std::make_shared<LinuxDriverAdapter>(_file.c_str());

#elif _WIN32
    _driverAdapter = std::make_shared<WindowsDriverAdapter>(_file.c_str());
    _devHandle = (HANDLE)_driverAdapter->GetFd();
    if (_devHandle == INVALID_HANDLE_VALUE) {
        cout << "Error: Can't open " << _file << endl;
        return;
    }
#endif
    _info = dxrt_device_info_t();
    _info.type = 0;
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, reinterpret_cast<void*>(&_info), 0, subCmd);
    if (ret != 0)
    {
        LOG_DXRT << "failed to identify device " << id_ << endl;
        _isBlocked = true;
        return;
    }

    LOG_DXRT_S_DBG << _name << ": device info : type " << _info.type
        << std::hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size
        << std::dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    DXRT_ASSERT(_info.mem_size > 0, "invalid device memory size");
    _type = static_cast<DeviceType>(_info.type);
    _variant = _info.variant;
#ifdef __linux__
    void *_mem = _driverAdapter->MemoryMap(0, _info.mem_size, 0);
    if (reinterpret_cast<int64_t>(_mem) == -1)
    {
        _mem = nullptr;
    }
#elif _WIN32
    void* _mem = nullptr;   // unused in windows
#endif

    for (uint32_t num = 0; num < _info.num_dma_ch; num++)
        _thread[num] = std::thread(&ServiceDevice::WaitThread, this, num);

}
void ServiceDevice::Terminate()
{
    LOG_DXRT_S_DBG << "Device " << _id << " terminate" << endl;

    for (uint32_t i = 0; i < _info.num_dma_ch; i++)
    {
        dxrt_response_t data;
        data.req_id = i;
        int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, &data);
        std::ignore = ret;
    }
}

int ServiceDevice::InferenceRequest(dxrt_request_acc_t* req)
{
    std::lock_guard<std::mutex> lock(_lock);
    // _timer.start();
    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ, req);
}

int ServiceDevice::WaitThread(int ids)
{
    LOG_DXRT_S_DBG << "@@@ Thread Start : WaitThread(DXRT_CMD_NPU_RUN_RESP)" << std::endl;
    string threadName = "ServiceDevice::WaitThread()";
    dxrt_cmd_t cmd =  // static_cast<dxrt_cmd_t>(static_cast<int>(dxrt::dxrt_cmd_t::DXRT_CMD_READ_OUTPUT_DMA_CH0)+id);
        dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
    int loopCnt = 0;
    int ret = 0;
    while (true)
    {
        // LOG_DXRT_DBG << threadName << " : wait" << endl;
        if (_stop.load())
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t response;
        memset(static_cast<void*>(&response), 0, sizeof(dxrt_response_t));
        response.req_id = ids;
        
#ifdef USE_PROFILER
        // Record wait start time using ProfilerClock
        auto wait_start = ProfilerClock::now();
        response.wait_start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(wait_start.time_since_epoch()).count();
#endif
        
        ret = Process(cmd, &response);
        
#ifdef USE_PROFILER
        // Record wait end time and calculate duration
        auto wait_end = ProfilerClock::now();
        response.wait_end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(wait_end.time_since_epoch()).count();
        
        // Calculate wait duration in microseconds
        auto wait_duration = std::chrono::duration_cast<std::chrono::microseconds>(wait_end - wait_start);
        response.wait_timestamp = static_cast<uint64_t>(wait_duration.count());
        
        // Record in profiler using TimePoint
        auto wait_tp = std::make_shared<TimePoint>();
        wait_tp->start = wait_start;
        wait_tp->end = wait_end;
        std::string profile_name = "Service Process Wait[Thread_" + std::to_string(ids) + "][Device_" + std::to_string(_id) + "]";
        _profiler.AddTimePoint(profile_name, wait_tp);
#endif
        // cout << response << endl; // for debug.

        if (ret == 0 && !_stop.load())
        {
            // if(response.req_id%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || response.req_id%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            // {
            //     cout<<"[     WAIT_T] THREAD : "<<ids<<" - DEVICE : "<<_id<<" - PROCESS_ID : "<<response.proc_id<<" - REQ_ID : "<<response.req_id<<endl;//AGING LOG
            //     //cout<<"[WAIT_THREAD ] REQ_ID "<< response << endl; // for debug.
            // }
            if (response.status != 0)
            {
                LOG_VALUE(response.status);
                string _dumpFile = "dxrt.dump.bin." + std::to_string(id());
                cout << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                cout << "    Device " << id() << " dump to file " << _dumpFile << endl;
                vector<uint32_t> dump(1000, 0);
                Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                for (size_t i = 0; i < dump.size(); i+=2)
                {
                    if (dump[i] == 0xFFFFFFFF) break;
                    // cout << hex << dump[i] << " : " << dump[i+1] << endl;
                }
                DataDumpBin(_dumpFile, dump.data(), dump.size());
                DataDumpTxt(_dumpFile+".txt", dump.data(), 1, dump.size()/2, 2, true);
                _stop.store(true);
                _isBlocked = true;
                _errCallBack(dxrt_server_err_t::S_ERR_DEVICE_RESPONSE_FAULT, response.status, id() );
            }
            else
            {
                pid_t pid = response.proc_id;

                // _timer.end();
                _timer[response.dma_ch].add(static_cast<double>(response.inf_time));

#ifdef __linux__
                // cout << pid << " process " << response.req_id << " request " << endl;    // for debug
                LOG_DXRT_S_DBG << pid << " process " << response.req_id << " request " << endl;
                // if (pid > 0)
                _callBack(response);  // send it to service scheduler
#elif _WIN32
                if (pid > 0) {  // in windows, valid process id
                    // cout << pid << " process " << response.req_id << " request " << endl;    // for debug
                    LOG_DXRT_S_DBG << pid << " process " << response.req_id << " request " << endl;
                    // if (pid > 0)
                    _callBack(response);  // send it to service scheduler
                }
#endif  // _WIN32
            }
        }
        else
        {
            // LOG_DXRT_S_ERR("DXRT_CMD_NPU_RUN_RESP ret !=0");	// for debug
            // if(response.req_id%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || response.req_id%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            // {
            //     cout<<"[     WAIT_T] - err_"<<ret<<" - THREAD : "<<ids<<" - DEVICE : "<<_id<<" - PROCESS_ID : "<<response.proc_id<<" - REQ_ID : "<<response.req_id<<endl;//AGING LOG
            //     //cout<<"[ERROR][WAIT_THREAD ] REQ_ID "<< response << endl; // for debug.
            // }
        }
        loopCnt++;
    }
    LOG_DXRT_S_DBG << "@@@ Thread End : WaitThread(DXRT_CMD_NPU_RUN_RESP), loopCount:" << loopCnt << std::endl;
    return 0;
}


std::ostream& operator<< (dxrt_sche_sub_cmd_t subCmd, std::ostream& os)
{
    switch(subCmd)
    {
        case dxrt_sche_sub_cmd_t::DX_SCHED_ADD:
            os << "DX_SCHED_ADD";
            break;
        case dxrt_sche_sub_cmd_t::DX_SCHED_DELETE:
            os << "DX_SCHED_DELETE";
            break;
        default:
            os << "dxrt_sche_sub_cmd_t errvalue" << static_cast<int>(subCmd);
            break;
    }
    return os;
}


int ServiceDevice::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp)
{
    LOG_DXRT_S_DBG << "Device " << id() << " " << subCmd << " bound " << boundOp << endl;

    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, reinterpret_cast<void*>(&boundOp),
        sizeof(dxrt_sche_sub_cmd_t), subCmd);
}

int ServiceDevice::AddBound(npu_bound_op boundOp)
{
    UniqueLock lk(_boundLock);

    if (_bound_count[static_cast<int>(boundOp)] > 0)
    {
        _bound_count[static_cast<int>(boundOp)]++;
        return 0;
    }
    int ret = BoundOption(dxrt_sche_sub_cmd_t::DX_SCHED_ADD, boundOp);
    if (ret == 0)
    {
        _bound_count[static_cast<int>(boundOp)]++;
    }
    return ret;
}
int ServiceDevice::DeleteBound(npu_bound_op boundOp)
{
    UniqueLock lk(_boundLock);
    if (_bound_count[static_cast<int>(boundOp)] > 1)
    {
        _bound_count[static_cast<int>(boundOp)]--;
        return 0;
    }
    int ret = BoundOption(dxrt_sche_sub_cmd_t::DX_SCHED_DELETE, boundOp);
    if (ret == 0)
    {
        _bound_count[static_cast<int>(boundOp)]--;
    }
    return ret;
}

int ServiceDevice::GetBoundCount(npu_bound_op boundOp)
{
    SharedLock lk(_boundLock);
    return _bound_count[static_cast<int>(boundOp)];
}

int ServiceDevice::GetBoundTypeCountInternal()
{
    int ret = 0;
    for (int i = 0; i < static_cast<int>(npu_bound_op::N_BOUND_INF_MAX); i++)
    {
        if (_bound_count[i] > 0)
        {
            ret++;
        }
    }
    return ret;
}
int ServiceDevice::GetBoundTypeCount()
{
    SharedLock lk(_boundLock);
    return GetBoundTypeCountInternal();
}
bool ServiceDevice::CanAcceptBound(npu_bound_op boundOp)
{
    SharedLock lk(_boundLock);
    if (_bound_count[static_cast<int>(boundOp)] > 0)
    {
        return true;
    }
    return GetBoundTypeCountInternal() < 3;
}


void ServiceDevice::SetCallback(std::function<void(const dxrt_response_t&)> f)
{
    _callBack = f;
}
void ServiceDevice::SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> f)
{
    _errCallBack = f;
}


// #define DEVICE_FILE "dxrt_dsp"
static vector<shared_ptr<ServiceDevice>> serviceDevices;



vector<shared_ptr<ServiceDevice>> ServiceDevice::CheckServiceDevices(uint32_t subCmd)
{
    LOG_DXRT_DBG << endl;
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = forceNumDevStr?std::stoi(forceNumDevStr):0;
    int forceDevId = forceDevIdStr?std::stoi(forceDevIdStr):-1;

    if (serviceDevices.empty())
    {
        // cout << "DXRT " DXRT_VERSION << endl;
        serviceDevices.clear();
        int cnt = 0;
        while (1)
        {
#ifdef __linux__
            string devFile("/dev/" + string(DEVICE_FILE) + std::to_string(cnt));
#elif _WIN32
            string devFile("\\\\.\\" + string(DEVICE_FILE) + std::to_string(cnt));
#endif
            if (fileExists(devFile))
            {
                if (forceNumDev > 0 && cnt >= forceNumDev) break;
                if (forceDevId != -1 && cnt != forceDevId)
                {
                    cnt++;
                    continue;
                }

                LOG_DBG("Found " + devFile);
                std::shared_ptr<ServiceDevice> device = std::make_shared<ServiceDevice>(devFile);
                device->Identify(cnt, subCmd);
                serviceDevices.emplace_back(device);
            }
            else
            {
                break;
            }
            cnt++;
        }
        DXRT_ASSERT(cnt > 0, "Device not found.");
    }

    return serviceDevices;
    // vector<shared_ptr<Device>> ret = vector<shared_ptr<Device>>(devices);
    // return ret;
}

double ServiceDevice::getUsage(int core_id)
{
    return _timer[core_id].getUsage();
}

void ServiceDevice::usageTimerTick()
{
    for (int i = 0; i < 3; i++)
    {
        _timer[i].onTick();
    }
}


void ServiceDevice::DoCustomCommand(void *data, uint32_t subCmd, uint32_t size)
{
    std::ignore = size;

    dxrt_custom_sub_cmt_t sCmd = static_cast<dxrt_custom_sub_cmt_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }
    return;  // TODO: temporary disable weight checksum
    switch (sCmd)
    {
        case DX_ADD_WEIGHT_INFO:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    sizeof(dxrt_custom_weight_info_t),
                    sCmd);
            break;
        }
        case DX_DEL_WEIGHT_INFO:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    sizeof(dxrt_custom_weight_info_t),
                    sCmd);
            break;
        }

        default:
            LOG_DXRT_ERR("Unknown sub command in service: " << sCmd);
            break;
    }
}


}  // namespace dxrt

