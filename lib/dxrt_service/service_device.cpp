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

#include "../data/ppcpu.h"
#include "dxrt/safe_cast.h"

using std::vector;
using std::cout;
using std::endl;
using std::to_string;

namespace dxrt {



ServiceDevice::ServiceDevice(const string &file_)
: _file(file_), _profiler(Profiler::GetInstance())  // NOSONAR:S3230
{
    _name = string(_file);  // temp.
    LOG_DXRT_S_DBG << "Device created from " << _name << endl;
#ifdef __linux__
#elif _WIN32
    // _driverAdapter = make_shared<WindowsDriverAdapter>(_file);
#endif
    std::fill(_bound_count.begin(), _bound_count.end(), 0);
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

// define ServiceDevice_DEBUG for debug usage

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

int ServiceDevice::Process(dxrt_cmd_t cmd, void *data, uint32_t size, uint32_t sub_cmd) const
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
    ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, static_cast<void*>(&_info), 0, subCmd);
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
    void *_mem = _driverAdapter->MemoryMap(nullptr, _info.mem_size, 0);
    int64_t mem_ptr_int;
    memcpy(&mem_ptr_int, &_mem, sizeof(void*));

    if (mem_ptr_int == -1)
    {
        _mem = nullptr;
    }
#elif _WIN32
    void* _mem = nullptr;   // unused in windows
#endif

    for (uint32_t num = 0; num < _info.num_dma_ch; num++)
    {
        _thread[num] = std::thread(&ServiceDevice::WaitThread, this, num);
    }
    _eventThread = std::thread(&ServiceDevice::EventThread, this);
}

void ServiceDevice::LoadPPCPUFirmware(uint64_t offset)
{
    size_t size = 0;
    auto data = static_cast<void*>(PPCPUDataLoader::GetData(size));

    dxrt_req_meminfo_t memInfo;
    memInfo.base = _info.mem_addr;
    memInfo.offset = static_cast<uint32_t>(offset);
    memInfo.size = static_cast<uint32_t>(size);
    memInfo.data = SafeCast::PointerToInteger<void*>(data);
    memInfo.ch = 0;

    // Write PPCPU firmware to device memory
    int ret1 = Process(dxrt::dxrt_cmd_t::DXRT_CMD_WRITE_MEM, static_cast<void*>(&memInfo));
    if (ret1 != 0)
    {
        LOG_DXRT << "failed to load PPCPU firmware to device " << _id <<", ret:" << ret1 << std::endl;
        _isBlocked = true;
        return;
    }

    // send PPCPU firmware information
    dxrt_custom_sub_cmt_t customCmd = dxrt_custom_sub_cmt_t::DX_INIT_PPCPU;

    int ret2 = Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM, static_cast<void*>(&memInfo), sizeof(dxrt_req_meminfo_t), static_cast<uint32_t>(customCmd));
    if (ret2 != 0)
    {
        LOG_DXRT << "failed to initialize PPCPU firmware on device " << _id <<", ret:" << ret2 << std::endl;
        _isBlocked = true;
        return;
    }


    //check ppcpu data integrity

    std::vector<uint8_t> readData(size, 0);
    dxrt_req_meminfo_t checkMemInfo;
    checkMemInfo.base = _info.mem_addr;
    checkMemInfo.offset = static_cast<uint32_t>(offset);
    checkMemInfo.size = static_cast<uint32_t>(size);
    checkMemInfo.data = SafeCast::PointerToInteger<void*>(readData.data());
    checkMemInfo.ch = 0;
    int retCheck = Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, static_cast<void*>(&checkMemInfo));
    if (retCheck != 0)
    {
        LOG_DXRT << "failed to read back PPCPU firmware from device " << _id <<", ret:" << retCheck << std::endl;
        _isBlocked = true;
        return;
    }
    if (memcmp(data, readData.data(), size) != 0)
    {
        LOG_DXRT << "PPCPU firmware data mismatch on device " << _id << std::endl;
        _isBlocked = true;
        return;
    }
    LOG_DXRT_S << "PPCPU firmware loaded to device " << _id << " successfully." << std::endl;
}

void ServiceDevice::Terminate() const
{
    LOG_DXRT_S_DBG << "Device " << _id << " terminate" << endl;
    if (_driverAdapter == nullptr)
    {
        LOG_DXRT_S_DBG << "Device " << _id << " driver adapter is null, skipping Terminate." << endl;
        return;
    }
    _driverAdapter->Close();
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
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
#endif

    int loopCnt = 0;
    int ret = 0;
    while (true)   // NOSONAR
    {
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

        //Scheduling debug log
        LOG_DXRT_S_DBG << "process " << response.proc_id
            << " request " << response.req_id
            << " response.dma_ch " << response.dma_ch << endl;

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


        if (ret == 0 && !_stop.load())
        {
            if (response.status != 0)
            {
                uint32_t errCode = static_cast<uint32_t>(response.status);   // NOSONAR
                LOG_VALUE(response.status);

                // Check if this is a recoverable error that EventThread will handle
                // via DXRT_CMD_EVENT → DXRT_CMD_RECOVERY.
                //   100-103: DMA timeout + soft reset failure
                //   300:     FW timeout
                //   400-403: DMA HW abort (Abort MSI)
                bool isRecoverable = (errCode >= 100 && errCode < 200)
                                  || (errCode == 300)
                                  || (errCode >= 400 && errCode < 500);

                if (isRecoverable)
                {
                    LOG_DXRT_S_ERR("[WaitThread " + std::to_string(ids) + "] Recoverable error (code="
                        + std::to_string(errCode) + ") on device " + std::to_string(id())
                        + "). Deferring recovery to EventThread.");
                    // EventThread handles DXRT_CMD_RECOVERY via the driver event
                    // queue.  WaitThread may reach here only if a pending
                    // NPU_RUN_RESP returns with an error status.  Just stop
                    // this thread — EventThread will perform recovery and
                    // call std::_Exit.
                    _isBlocked = true;
                    break;
                }

                // Non-recoverable error — fatal path (existing behavior)
                string _dumpFile = "dxrt.dump.bin." + std::to_string(id());
                cout << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                cout << "    Device " << id() << " dump to file " << _dumpFile << endl;
                vector<uint32_t> dump(1000, 0);
                Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                for (size_t i = 0; i < dump.size(); i += 2)
                {
                    if (dump[i] == 0xFFFFFFFF)
                        break;
                }
                DataDumpBin(_dumpFile, dump.data(), static_cast<unsigned int>(dump.size()));
                DataDumpTxt(_dumpFile+".txt", dump.data(), 1, static_cast<unsigned int>(dump.size())/2, 2, true);
                _stop.store(true);
                _isBlocked = true;
                _errCallBack(dxrt_server_err_t::S_ERR_DEVICE_RESPONSE_FAULT, response.status, id() );
                DXRT_ASSERT(false, "Device error detected, terminating device thread.");
            }
            else
            {
                pid_t pid = response.proc_id;


                _timer[response.dma_ch].add(static_cast<double>(response.inf_time));

#ifdef __linux__
               LOG_DXRT_S_DBG << "process "<< pid << " request " << response.req_id << " response.dma_ch " << response.dma_ch << endl;
                if (pid <= 0)
                {
                    LOG_DXRT_S_ERR("Invalid process ID received: " + std::to_string(pid));
                    continue;
                }
                _callBack(response);  // send it to service scheduler
#elif _WIN32
                if (pid > 0) {  // in windows, valid process id

                    LOG_DXRT_S_DBG << pid << " process " << response.req_id << " request " << endl;

                    _callBack(response);  // send it to service scheduler
                }
#endif  // _WIN32
            }
        }

        loopCnt++;
    }
    LOG_DXRT_S_DBG << "@@@ Thread End : WaitThread(DXRT_CMD_NPU_RUN_RESP), loopCount:" << loopCnt << std::endl;
    return 0;
}

int ServiceDevice::EventThread()   // NOSONAR
{
    LOG_DXRT_S_DBG << "@@@ Thread Start : EventThread" << std::endl;
    string threadName = "ServiceDevice::EventThread()";
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
#endif
    int loopCnt = 0;
    int ret = 0;
    while (true)
    {
        if (_stop.load())
        {
            LOG_DXRT_S_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dx_pcie_dev_event_t eventInfo;
        memset(&eventInfo, 0, sizeof(dxrt::dx_pcie_dev_event_t));

        ret = Process(cmd, &eventInfo);

        if (ret != 0)
        {
            LOG_DXRT_S_ERR("DXRT_CMD_EVENT_V2 ret:" + std::to_string(ret));  // for debug
            std::this_thread::sleep_for(std::chrono::seconds(100));   // sleep and retry to avoid busy loop if event processing fails
        }

        bool is_error_event = (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type) == dxrt::dxrt_event_t::DXRT_EVENT_ERROR);
        bool is_not_none_error = (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code) != dxrt::dxrt_error_t::ERR_NONE);
        if (is_error_event && is_not_none_error)
        {
            uint32_t err_code = eventInfo.dx_rt_err.err_code;
            LOG_DXRT_S_ERR(eventInfo.dx_rt_err);

            // Classify error by code range
            //   100-103: DMA timeout + soft reset failure
            //   200-201: LPDDR ECC error
            //   300:     FW timeout
            //   400-403: DMA HW abort (Abort MSI)
            // Recoverable errors (100-199, 300, 400-499):
            // EventThread is the authoritative handler — the driver event
            // queue delivers err_code here.  WaitThread may have no pending
            // NPU_RUN_RESP (client _Exit'd before submitting), so it cannot
            // be relied upon to trigger recovery.
            bool isRecoverable = (err_code >= 100 && err_code < 200)
                              || (err_code == 300)
                              || (err_code >= 400 && err_code < 500);

            if (isRecoverable)
            {
                LOG_DXRT_S_ERR("[EventThread] Recoverable error (code="
                    + std::to_string(err_code) + ") on device " + std::to_string(id())
                    + "). Performing recovery.");
                LogDmaChannelStatus(&eventInfo.dx_rt_err);

                // 1. Broadcast to all clients and wait for them to terminate.
                //    _recoveryCallBack sets DxrtService::_recoveryInProgress,
                //    broadcasts ERROR_REPORT, and polls until every client PID
                //    is dead (with SIGKILL fallback after timeout).
                _recoveryCallBack(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, err_code, id());

                // 2. Single DXRT_CMD_RECOVERY ioctl
                int recovery_ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
                if (recovery_ret < 0)
                {
                    LOG_DXRT_S_ERR("DXRT_CMD_RECOVERY failed for device " + std::to_string(id())
                        + " ret=" + std::to_string(recovery_ret) + ". Aborting.");
                    std::abort();
                }

                LOG_DXRT_S << "Recovery completed (EventThread) for device " << id() << endl;

                // 3. Terminate dxrtd for systemd restart
                LOG_DXRT_S << "Terminating dxrtd for systemd restart." << endl;
                std::_Exit(EXIT_FAILURE);
            }
            else
            {
                // Non-recoverable errors — fatal abort as before
                _errCallBack(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, err_code, id());

                Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);

                std::abort();
            }
        }
        loopCnt++;
    }
    LOG_DXRT_S_DBG << "@@@ Thread End : EventThread, loopCount:" << loopCnt << std::endl;
    return 0;
}

void ServiceDevice::LogDmaChannelStatus(const dx_pcie_dev_err_t *err) const
{
    std::cout << "  WR ch status: ["
        << err->dma_wr_ch_sts[0] << ", "
        << err->dma_wr_ch_sts[1] << ", "
        << err->dma_wr_ch_sts[2] << ", "
        << err->dma_wr_ch_sts[3] << "]" << std::endl;
    std::cout << "  RD ch status: ["
        << err->dma_rd_ch_sts[0] << ", "
        << err->dma_rd_ch_sts[1] << ", "
        << err->dma_rd_ch_sts[2] << ", "
        << err->dma_rd_ch_sts[3] << "]" << std::endl;
    std::cout << "  PCIe BDF: "
        << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(err->bus) << ":"
        << std::setw(2) << static_cast<int>(err->dev) << "."
        << static_cast<int>(err->func)
        << std::dec << std::endl;
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


int ServiceDevice::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp) const
{
    LOG_DXRT_S_DBG << "Device " << id() << " " << subCmd << " bound " << boundOp << endl;

    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE, static_cast<void*>(&boundOp),
        sizeof(dxrt_sche_sub_cmd_t), subCmd);
}

int ServiceDevice::AddBound(npu_bound_op boundOp)
{
    UniqueLock lk(_boundLock);

    LOG_DXRT_S_DBG << "Device " << id() << " ADD bound " << boundOp << endl;
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

    else
    {
        LOG_DXRT_S_ERR("Failed to add bound option: " << ret);
    }
    return ret;
}
int ServiceDevice::DeleteBound(npu_bound_op boundOp)
{
    UniqueLock lk(_boundLock);
    LOG_DXRT_S_DBG << "Device " << id() << " DELETE bound " << boundOp << endl;

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

    else
    {
        LOG_DXRT_S_ERR("Failed to delete bound option: " << ret);
    }
    return ret;
}

int ServiceDevice::GetBoundCount(npu_bound_op boundOp)
{
    SharedLock lk(_boundLock);
    return _bound_count[static_cast<int>(boundOp)];
}

int ServiceDevice::GetBoundTypeCountInternal() const
{
    return static_cast<int>(std::count_if(_bound_count.begin(), _bound_count.end(),
                         [](int count) { return count > 0; }));
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
    if (GetBoundTypeCountInternal() >= 3)
    {
        std::string msg = "Current Bound: ";
        for (size_t i = 0; i < _bound_count.size(); i++)
        {
            if (_bound_count[i] > 0)
            {
                msg += std::to_string(i) + "," + std::to_string(_bound_count[i]) + " ";
            }
        }
        msg += "cannot accept new bound " + std::to_string(static_cast<int>(boundOp));
        LOG_DXRT_DBG << msg << endl;
    }
    return GetBoundTypeCountInternal() < 3;
}


void ServiceDevice::SetCallback(const std::function<void(const dxrt_response_t&)>& f)
{
    _callBack = f;
}
void ServiceDevice::SetErrorCallback(const std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)>& f)
{
    _errCallBack = f;
}
void ServiceDevice::SetRecoveryCallback(const std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)>& f)
{
    _recoveryCallBack = f;
}


static vector<shared_ptr<ServiceDevice>> serviceDevices;  // NOSONAR:S5421



vector<shared_ptr<ServiceDevice>> ServiceDevice::CheckServiceDevices(uint32_t subCmd)
{
    LOG_DXRT_DBG << endl;
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = forceNumDevStr?std::stoi(forceNumDevStr):0;
    int forceDevId = forceDevIdStr?std::stoi(forceDevIdStr):-1;

    if (serviceDevices.empty())
    {
        serviceDevices.clear();
        int cnt = 0;
        bool shouldBreak = false;
        while (shouldBreak == false)
        {
#ifdef __linux__
            std::string devFile("/dev/" + std::string(DEVICE_FILE) + std::to_string(cnt));
#elif _WIN32
            std::string devFile("\\\\.\\" + std::string(DEVICE_FILE) + std::to_string(cnt));
#endif
            if (fileExists(devFile))
            {
                if (forceNumDev > 0 && cnt >= forceNumDev)
                {
                    shouldBreak = true;
                    continue;
                }
                if (forceDevId != -1 && cnt != forceDevId)
                {
                    cnt++;
                    continue;
                }

                LOG_DBG("Found " + devFile);
                auto device = std::make_shared<ServiceDevice>(devFile);
                device->Identify(cnt, subCmd);
                serviceDevices.emplace_back(device);
            }
            else
            {
                shouldBreak = true;
                continue;
            }
            cnt++;
        }
        DXRT_ASSERT(cnt > 0, "Device not found.");
    }

    return serviceDevices;
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


void ServiceDevice::DoCustomCommand(void *data, uint32_t subCmd, uint32_t size) const
{
    std::ignore = size;

    auto sCmd = static_cast<dxrt_custom_sub_cmt_t>(subCmd);
    if (data == nullptr)
    {
        LOG_DXRT_ERR("Null data pointer received");
        return;
    }

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
        case DX_INIT_PPCPU:
        {
            Process(dxrt::dxrt_cmd_t::DXRT_CMD_CUSTOM,
                    data,
                    size,
                    sCmd);
            break;
        }

        default:
            LOG_DXRT_ERR("Unknown sub command in service: " << sCmd);
            break;
    }
}


}  // namespace dxrt

