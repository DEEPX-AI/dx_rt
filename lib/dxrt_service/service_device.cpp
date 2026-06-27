/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "service_device.h"

#include "dxrt/exception/exception.h"

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
#include <chrono>
#include <condition_variable>
#include <utility>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/filesys_support.h"
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

namespace {

}  // namespace



ServiceDevice::ServiceDevice(const string &file_)
: _file(file_), _profiler(Profiler::GetInstance()), // NOSONAR:S3230
  _readHandlerQueue("ServiceDevice_ReadHandlerQueue", 4,[this](const DMAItem& item, int ch){ this->HandleDMARead(item, ch); return 0; }),
  _writeHandlerQueue("ServiceDevice_WriteHandlerQueue", 2,[this](const DMAItem& item, int ch){ this->HandleDMAWrite(item, ch); return 0; })
{
    _name = string(_file);  // temp.
    LOG_DXRT_S_DBG << "Device created from " << _name << endl;
#ifdef __linux__
#elif _WIN32
    // _driverAdapter = make_shared<WindowsDriverAdapter>(_file);
#endif
    _callBack = nullptr;
}



ServiceDevice::~ServiceDevice(void)
{
    _stop.store(true);

    Terminate();
    if (_dispatcher)
    {
        _dispatcher->RequestStop();
        _dispatcher->Join();
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
    if (cmd == dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY)
        LOG_DXRT_S << _id << ": Send recovery command" << endl;
    return _core->Process(cmd, data, size, sub_cmd);
}

dxrt_device_status_t ServiceDevice::status()
{
    std::lock_guard<std::mutex> lock(_lock);
    if (_core == nullptr)
    {
        _status = dxrt_device_status_t{};
        return _status;
    }

    _status = _core->Status();
    return _status;
}



void ServiceDevice::Identify(int id_, uint32_t subCmd )
{
    LOG_DXRT_S_DBG << "Device " << _id << " Identify" << endl;
    std::lock_guard<std::mutex> lock(_lock);
    _id = id_;
#ifdef __linux__
    _core = std::make_shared<DeviceCore>(_id, std::make_unique<LinuxDriverAdapter>(_file.c_str()));
#elif _WIN32
    auto adapter = std::make_unique<WindowsDriverAdapter>(_file.c_str());
    _devHandle = (HANDLE)adapter->GetFd();
    if (_devHandle == INVALID_HANDLE_VALUE) {
        cout << "Error: Can't open " << _file << endl;
        return;
    }
    _core = std::make_shared<DeviceCore>(_id, std::move(adapter));
#endif

    _core->Identify(_id, subCmd);
    _info = _core->info();
    if (_info.mem_size == 0)
    {
        LOG_DXRT << "failed to identify device " << id_ << endl;
        _isBlocked = true;
        return;
    }
    // Successful identify: clear any blocked flag latched by a prior
    // (transient) attempt so callers/retries observe the healthy state.
    _isBlocked = false;
    LOG_DXRT_S_DBG << _name << ": device info : type " << _info.type
        << std::hex << ", variant " << _info.variant
        << ", mem_addr " << _info.mem_addr
        << ", mem_size " << _info.mem_size
        << std::dec << ", num_dma_ch " << _info.num_dma_ch << endl;
    _type = static_cast<DeviceType>(_info.type);
    _variant = _info.variant;
    _devInfo = dxrt_dev_info_t{};

#ifdef __linux__
    auto interface_value = _info.interface;
#elif _WIN32
    auto interface_value = _info.interface_value;
#endif

    if ((interface_value == DEVICE_INTERFACE_ASIC) && (_info.type == DEVICE_TYPE_ACCELERATOR))
    {
        int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
                      static_cast<void*>(&_devInfo.rt_drv_ver.driver_version),
                      0,
                      dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_RT_INFO);
        if (ret != 0)
        {
            LOG_DXRT_S_ERR("Failed to get RT driver info for shared memory publishing");
        }
        else if (_devInfo.rt_drv_ver.driver_version > 1701)
        {
            int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
                          static_cast<void*>(&_devInfo.rt_drv_ver),
                          0,
                          dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_RT_INFO_V2);
            if (ret != 0)
            {
                LOG_DXRT_S_ERR("Failed to get RT driver info suffix for shared memory publishing");
            }
        }

        ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
                      static_cast<void*>(&_devInfo.pcie),
                      0,
                      dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_PCIE_INFO);
        if (ret != 0)
        {
            LOG_DXRT_S_ERR("Failed to get PCIe driver info for shared memory publishing");
            _devInfo.pcie = deepx_pcie_info{};
        }
    }
#ifdef __linux__
    _core->CreateMemoryMap();
#endif

    _dispatcher = std::make_unique<DeviceDispatcher>(_core, _timer, _profiler);
    _dispatcher->SetResponseCallback(_callBack);
    _dispatcher->SetErrorCallback(_errCallBack);
    _dispatcher->SetRecoveryCallback(_recoveryCallBack);
    _dispatcher->SetThrottleCallback(_throttleCallBack);
    _dispatcher->StartThreads();
    _readHandlerQueue.Start();
    _writeHandlerQueue.Start();
    _boundManager = std::make_unique<BoundManager>(_core);
}

void ServiceDevice::LoadPPCPUFirmware(uint64_t offset)
{
    _core->InitPPCPU(offset);
}

void ServiceDevice::Terminate() const
{
    LOG_DXRT_S_DBG << "Device " << _id << " terminate" << endl;
    if (_core == nullptr)
    {
        LOG_DXRT_S_DBG << "Device " << _id << " core is null, skipping Terminate." << endl;
        return;
    }
    _core->Close();
}

int ServiceDevice::InferenceRequest(dxrt_request_acc_t* req)
{
    std::lock_guard<std::mutex> lock(_lock);
    // _timer.start();
    if (req != nullptr)
    {
        if (_info.num_dma_ch > 0 && (req->dma_ch < 0 || req->dma_ch >= static_cast<int32_t>(_info.num_dma_ch)))
        {
            const auto numChannels = static_cast<int32_t>(_info.num_dma_ch);
            const int32_t normalizedChannel = ((req->dma_ch % numChannels) + numChannels) % numChannels;
            LOG_DXRT_S_ERR("ServiceDevice::InferenceRequest normalized invalid dma_ch: pid="
                + std::to_string(req->proc_id) + ", req_id=" + std::to_string(req->req_id)
                + ", dma_ch=" + std::to_string(req->dma_ch)
                + ", num_dma_ch=" + std::to_string(_info.num_dma_ch)
                + ", normalized=" + std::to_string(normalizedChannel));
            req->dma_ch = normalizedChannel;
        }

    }

    LOG_DXRT_S_DBG << "ServiceDevice::InferenceRequest submit NPU_RUN_REQ"
                   << (req != nullptr ? ": pid=" + std::to_string(req->proc_id)
                       + ", req_id=" + std::to_string(req->req_id)
                       + ", dma_ch=" + std::to_string(req->dma_ch)
                       + ", queue=" + std::to_string(req->queue) : ": req=null")
                   << std::endl;

    const int ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ, req);
    LOG_DXRT_S_DBG << "ServiceDevice::InferenceRequest NPU_RUN_REQ returned ret=" << ret
                   << (req != nullptr ? ": pid=" + std::to_string(req->proc_id)
                       + ", req_id=" + std::to_string(req->req_id)
                       + ", dma_ch=" + std::to_string(req->dma_ch)
                       + ", queue=" + std::to_string(req->queue) : ": req=null")
                   << std::endl;
    return ret;
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

int ServiceDevice::AddBound(npu_bound_op boundOp)     { return _boundManager->AddBound(boundOp); }
int ServiceDevice::DeleteBound(npu_bound_op boundOp)  { return _boundManager->DeleteBound(boundOp); }
int ServiceDevice::GetBoundCount(npu_bound_op boundOp){ return _boundManager->GetBoundCount(boundOp); }
int ServiceDevice::GetBoundTypeCount()                { return _boundManager->GetBoundTypeCount(); }
bool ServiceDevice::CanAcceptBound(npu_bound_op boundOp) { return _boundManager->CanAcceptBound(boundOp); }


void ServiceDevice::SetCallback(const std::function<void(const dxrt_response_t&)>& f)
{
    _callBack = f;
    if (_dispatcher)
    {
        _dispatcher->SetResponseCallback(f);
    }
}
void ServiceDevice::SetErrorCallback(const std::function<void(dxrt::dxrt_server_err_t, uint32_t, int, const dx_pcie_dev_err_t *)>& f)
{
    _errCallBack = f;
    if (_dispatcher)
    {
        _dispatcher->SetErrorCallback(f);
    }
}
void ServiceDevice::SetRecoveryCallback(const std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)>& f)
{
    _recoveryCallBack = f;
    if (_dispatcher)
    {
        _dispatcher->SetRecoveryCallback(f);
    }
}
void ServiceDevice::SetThrottleCallback(const std::function<void(dx_pcie_dev_ntfy_throt_t, int)>& f)
{
    _throttleCallBack = f;
    if (_dispatcher)
    {
        _dispatcher->SetThrottleCallback(f);
    }
}

void ServiceDevice::SetRecoveryAdapter(const std::shared_ptr<RecoveryAdapter>& adapter)
{
    if (_dispatcher)
    {
        _dispatcher->SetRecoveryAdapter(adapter);
    }
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

                // Bounded IDENTIFY retry. After a transient link event
                // (AER recovery, link_flap, cpu_reset) the driver tears the
                // link down, re-establishes it, then schedules an async
                // IDENTIFY probe; the FW republishes its mailbox only a short
                // time later (typically a few hundred ms). If dxrtd starts or
                // is restarted by systemd inside that window, a single-shot
                // IDENTIFY ioctl races the FW mailbox and fails, which would
                // otherwise throw out of the constructor and make systemd
                // bounce the service. Retry briefly so we ride through the
                // re-init window. A genuinely hung FW (mailbox never
                // republishes) still fails fast once the bounded window
                // elapses, preserving the original error semantics.
                constexpr int kIdentifyMaxAttempts = 15;
                constexpr auto kIdentifyRetryDelay = std::chrono::milliseconds(100);
                int identifyAttempt = 0;
                while (true)
                {
                    try
                    {
                        device->Identify(cnt, subCmd);
                    }
                    catch (const dxrt::DeviceIOException&)
                    {
                        if (++identifyAttempt >= kIdentifyMaxAttempts)
                        {
                            throw;
                        }
                        LOG_DXRT << "IDENTIFY not ready for " << devFile
                            << " (attempt " << identifyAttempt << "/"
                            << kIdentifyMaxAttempts
                            << "); waiting for FW mailbox to settle." << endl;
                        std::this_thread::sleep_for(kIdentifyRetryDelay);
                        continue;
                    }
                    // Soft failure: ioctl succeeded but returned empty info
                    // (mem_size==0) -> FW mailbox not fully up yet. Retry
                    // within the same bounded window before accepting the
                    // blocked device.
                    if (device->isBlocked())
                    {
                        ++identifyAttempt;
                        if (identifyAttempt < kIdentifyMaxAttempts)
                        {
                            LOG_DXRT << "IDENTIFY soft-failed (mem_size==0) for " << devFile
                                << " (attempt " << identifyAttempt << "/"
                                << kIdentifyMaxAttempts << "); retrying." << endl;
                            std::this_thread::sleep_for(kIdentifyRetryDelay);
                            continue;
                        }
                    }
                    break;
                }
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
    return _core->DoCustomCommand(data, subCmd, size);
}

void ServiceDevice::DMAReadAsync(const dxrt_meminfo_t& memInfo, pid_t pid, int seqId)
{
    ServiceDevice::DMAItem item;
    item.memInfo = memInfo;
    item.pid = pid;
    item.seqId = seqId;
    _readHandlerQueue.PushWork(item);
}
void ServiceDevice::DMAWriteAsync(const dxrt_meminfo_t& memInfo, pid_t pid, int seqId)
{
    ServiceDevice::DMAItem item;
    item.memInfo = memInfo;
    item.pid = pid;
    item.seqId = seqId;
    _writeHandlerQueue.PushWork(item);
}

void ServiceDevice::SetDMACompletionCallback(const std::function<void(bool, pid_t, int, int, int)> &callback)
{
    _dmaCompletionCallback = callback;
}

void ServiceDevice::HandleDMARead(const DMAItem& item, int ch)
{
    pid_t pid = item.pid;
    int seqId = item.seqId;
    dxrt_meminfo_t memInfo = item.memInfo;

    LOG_DXRT_S_DBG << "HandleDMARead: pid=" << pid << ", seqId=" << seqId <<
        ", memInfo={addr=" << std::hex << memInfo.data << std::dec << ", size=" << memInfo.size << "}" << std::endl;
    const int ret = _core->Read(memInfo, ch);
    if (_dmaCompletionCallback)
    {
        _dmaCompletionCallback(true, pid, _core->id(), seqId, ret);
    }
}

void ServiceDevice::HandleDMAWrite(const DMAItem& item, int ch)
{
    pid_t pid = item.pid;
    int seqId = item.seqId;
    dxrt_meminfo_t memInfo = item.memInfo;

    LOG_DXRT_S_DBG << "HandleDMAWrite: pid=" << pid << ", seqId=" << seqId <<
        ", memInfo={addr=" << std::hex << memInfo.data << std::dec << ", size=" << memInfo.size << "}" << std::endl;
    const int ret = _core->Write(memInfo, ch);
    if (_dmaCompletionCallback)
    {
        _dmaCompletionCallback(false, pid, _core->id(), seqId, ret);
    }
}


}  // namespace dxrt

