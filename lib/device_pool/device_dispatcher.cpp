/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "device_dispatcher.h"

#include <cerrno>
#include <cstring>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "dxrt/common.h"
#include "dxrt/util.h"

using std::endl;
using std::string;
using std::vector;
using std::cout;
using std::to_string;

namespace dxrt {

namespace {

const char *RecoverySubcodeToString(uint32_t subcode)
{
    switch (subcode)
    {
        case DX_RECOVERY_STARTED:       return "started";
        case DX_RECOVERY_DONE:          return "done";
        case DX_RECOVERY_FW_HANG:       return "fw_hang";
        case DX_RECOVERY_PERM_FAIL:     return "perm_fail";
        default:                        return "unknown";
    }
}

const char *RecoveryReasonToString(uint32_t reason)
{
    switch (reason)
    {
        case DX_RECOVERY_REASON_NONE:         return "none";
        case DX_RECOVERY_REASON_LINK_FLAP:    return "link_flap";
        case DX_RECOVERY_REASON_FW_TIMEOUT:   return "fw_timeout";
        case DX_RECOVERY_REASON_CPU_RESET:    return "cpu_reset";
        default:                              return "unknown";
    }
}

bool IsDriverOwnedRecoveryReason(uint32_t reason)
{
    return reason == DX_RECOVERY_REASON_LINK_FLAP
        || reason == DX_RECOVERY_REASON_CPU_RESET;
}

}  // namespace

DeviceDispatcher::DeviceDispatcher(std::shared_ptr<DeviceCore> core,
                                   std::array<UsageTimer, kResponseChannels>& timer,
                                   Profiler& profiler)
    : _core(std::move(core)),
      _deviceId(_core->id()),
      _timer(timer),
      _profiler(profiler)
{
}

DeviceDispatcher::~DeviceDispatcher()
{
    RequestStop();
    Join();
}

int DeviceDispatcher::Process(dxrt_cmd_t cmd, void* data, uint32_t size, uint32_t sub_cmd) const
{
    if (cmd == dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY)
        LOG_DXRT_S << _deviceId << ": Send recovery command" << endl;
    return _core->Process(cmd, data, size, sub_cmd);
}

void DeviceDispatcher::StartThreads()
{
    for (int ch = 0; ch < kResponseChannels; ++ch)
    {
        _responseThreads[ch] = std::thread(&DeviceDispatcher::ResponseLoop, this, ch);
    }
    _eventThread = std::thread(&DeviceDispatcher::EventLoop, this);
}

void DeviceDispatcher::Join()
{
    for (auto& t : _responseThreads)
    {
        if (t.joinable())
            t.join();
    }
    if (_eventThread.joinable())
        _eventThread.join();
}

int DeviceDispatcher::ResponseLoop(int ids)
{
    LOG_DXRT_S_DBG << "@@@ Thread Start : ResponseLoop(DXRT_CMD_NPU_RUN_RESP)" << std::endl;
    string threadName = "DeviceDispatcher::ResponseLoop()";
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
#endif

    int loopCnt = 0;
    int ret = 0;
    while (true)  // NOSONAR
    {
        if (_stop.load())
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt_response_t response{};
        response.req_id = ids;

#ifdef USE_PROFILER
        auto wait_start = ProfilerClock::now();
        response.wait_start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            wait_start.time_since_epoch()).count();
#endif

        LOG_DXRT_S_DBG << "DeviceDispatcher::ResponseLoop waiting NPU_RUN_RESP: thread_ch=" << ids
                            << ", wait_ch=" << ids
                            << ", response_req_id_seed=" << response.req_id
                            << ", response_dma_ch_seed=" << response.dma_ch << std::endl;


        ret = Process(cmd, &response);

        if (ret == -ENODATA)
        {
            LOG_DXRT_S << "DeviceDispatcher::ResponseLoop NPU_RUN_RESP no data: thread_ch=" << ids
                           << ", wait_ch=" << ids
                           << ", ret=" << ret << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            loopCnt++;
            continue;
        }

        if (ret != 0)
        {
            LOG_DXRT_S_ERR("DeviceDispatcher::ResponseLoop NPU_RUN_RESP failed: thread_ch="
                + std::to_string(ids) + ", wait_ch=" + std::to_string(ids)
                + ", ret=" + std::to_string(ret));
            loopCnt++;
            continue;
        }

        LOG_DXRT_S_DBG << "DeviceDispatcher::ResponseLoop NPU_RUN_RESP returned: thread_ch=" << ids
                       << ", wait_ch=" << ids
                       << ", req_id=" << response.req_id
                       << ", proc_id=" << response.proc_id
                       << ", dma_ch=" << response.dma_ch
                       << ", status=" << response.status << std::endl;

#ifdef USE_PROFILER
        auto wait_end = ProfilerClock::now();
        response.wait_end_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
            wait_end.time_since_epoch()).count();
        auto wait_duration = std::chrono::duration_cast<std::chrono::microseconds>(wait_end - wait_start);
        response.wait_timestamp = static_cast<uint64_t>(wait_duration.count());

        auto wait_tp = std::make_shared<TimePoint>();
        wait_tp->start = wait_start;
        wait_tp->end   = wait_end;
        std::string profile_name =
            "Service Process Wait[Thread_" + std::to_string(ids) + "][Device_" + std::to_string(_deviceId) + "]";
        // TODO: they has no job_id, so SERVICE_PROCESS_WAIT cannot migrate to new profiler yet.
        //_profiler.AddTimePoint(Profiler::EventType::ServiceProcessWait, 0, ids, wait_tp);
#endif

        if (ret == 0 && !_stop.load())
        {
            if (response.status != 0)
            {
                uint32_t errCode = static_cast<uint32_t>(response.status);  // NOSONAR
                LOG_VALUE(response.status);

                // Recoverable error ranges handled by EventLoop via DXRT_CMD_RECOVERY:
                //   100-103: DMA timeout + soft reset failure
                //   300:     FW timeout
                //   400-403: DMA HW abort (Abort MSI)
                bool isRecoverable = (errCode >= 100 && errCode < 200)
                                  || (errCode == 300)
                                  || (errCode >= 400 && errCode < 500);

                if (isRecoverable)
                {
                    LOG_DXRT_S_ERR("[ResponseLoop " + std::to_string(ids) + "] Recoverable error (code="
                        + std::to_string(errCode) + ") on device " + std::to_string(_deviceId)
                        + "). Deferring recovery to EventLoop.");
                    _isBlocked.store(true);
                    break;
                }

                // Non-recoverable error — fatal path (preserved behavior)
                string _dumpFile = "dxrt.dump.bin." + std::to_string(_deviceId);
                cout << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << endl;
                cout << "    Device " << _deviceId << " dump to file " << _dumpFile << endl;
                vector<uint32_t> dump(1000, 0);
                Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
                [[maybe_unused]] int dump_finish = 0;
                for (size_t i = 0; i < dump.size(); i += 2)
                {
                    if (dump[i] == 0xFFFFFFFF)
                    {
                        dump_finish = static_cast<int>(i);
                        break;
                    }
                }
                /* note: although the dump size is 1000 uint32_t, the actual valid data may be less (terminated by 0xFFFFFFFF),
                   so we dump the entire buffer but also provide a text file with formatting for easier analysis. */
                DataDumpBin(_dumpFile, dump.data(), static_cast<unsigned int>(dump.size()));
                DataDumpTxt(_dumpFile + ".txt", dump.data(), 1,
                            static_cast<unsigned int>(dump.size()) / 2, 2, true);
                _stop.store(true);
                _isBlocked.store(true);
                if (_onError)
                    _onError(dxrt_server_err_t::S_ERR_DEVICE_RESPONSE_FAULT,
                             response.status, _deviceId, nullptr);
                DXRT_ASSERT(false, "Device error detected, terminating device thread. "
                    "valid dump size: " + std::to_string(dump_finish));
            }
            else
            {
                pid_t pid = response.proc_id;

                if (response.dma_ch >= 0 && response.dma_ch < static_cast<int32_t>(_timer.size()))
                {
                    _timer[response.dma_ch].add(static_cast<double>(response.inf_time));
                }
                else
                {
                    LOG_DXRT_S_ERR("DeviceDispatcher::ResponseLoop invalid response dma_ch: req_id="
                        + std::to_string(response.req_id) + ", dma_ch=" + std::to_string(response.dma_ch));
                    continue;
                }


                LOG_DXRT_S_DBG << "process " << pid << " request " << response.req_id
                               << " response.dma_ch " << response.dma_ch << endl;
                if (pid == 0)
                {
                    continue;  // in windows, this is not error
                }
                else if (pid < 0)
                {
                    LOG_DXRT_S_ERR("Invalid process ID received: " + std::to_string(pid));
                    continue;
                }

                if (_onResponse)
                {
                    _onResponse(response);
                }
            }
        }

        loopCnt++;
    }
    LOG_DXRT_S << "@@@ Thread End : ResponseLoop(DXRT_CMD_NPU_RUN_RESP)" << ids << ", loopCount:" << loopCnt
                   << std::endl;
    return 0;
}

int DeviceDispatcher::EventLoop()  // NOSONAR
{
    LOG_DXRT_S_DBG << "@@@ Thread Start : EventLoop" << std::endl;
    string threadName = "DeviceDispatcher::EventLoop()";
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
#endif
    int loopCnt = 0;
    int ret = 0;
    bool driverOwnedRecoveryActive = false;
    uint32_t driverOwnedRecoveryReason = DX_RECOVERY_REASON_NONE;
    while (true)
    {
        if (_stop.load())
        {
            LOG_DXRT_S_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dx_pcie_dev_event_t eventInfo{};

        ret = Process(cmd, &eventInfo);

        if (ret != 0)
        {
            LOG_DXRT_S_ERR("[EventLoop] Device " << _deviceId << ": Process(DXRT_CMD_EVENT_V2) failed with ret=" << ret << ", loopCnt=" << loopCnt);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        auto eventType = static_cast<dxrt::dxrt_event_t>(eventInfo.event_type);
        bool showEventTypeLog = (eventType != dxrt::dxrt_event_t::DXRT_EVENT_NONE);
#if _WIN32
        if (static_cast<int>(eventType) == 5)
            showEventTypeLog = false; // in windows, event_type=5 is a regular timeout event, ignore it.
#endif
        if (showEventTypeLog)
        {
            LOG_DXRT_S << "[EventLoop] Device " << _deviceId << ": Process returned event_type=" << static_cast<int>(eventType) << " (loopCnt=" << loopCnt << ")" << endl;
        }
        if (eventType == dxrt::dxrt_event_t::DXRT_EVENT_RECOVERY)
        {
            const auto& recovery = eventInfo.dx_rt_recv;
            LOG_DXRT_S << "Recovery event: subcode=" << RecoverySubcodeToString(recovery.action)
                << "(" << recovery.action << ") reason=" << RecoveryReasonToString(recovery.reason)
                << "(" << recovery.reason << ") count=" << recovery.recovery_count
                << " fail_count=" << recovery.recovery_fail_count
                << " dev_state=" << recovery.dev_state
                << " device=" << _deviceId << endl;

            // Terminal recovery failure. The driver attempted recovery but
            // could NOT restore the device: DX_RECOVERY_FW_HANG (transport is
            // up but FW mailbox/PING/IDENTIFY keeps failing) or
            // DX_RECOVERY_PERM_FAIL (retry threshold exceeded). Per the
            // driver/framework integration contract these are fatal and the
            // framework must stop treating the device as usable, surface the
            // fault to clients, and not keep serving on a dead device.
            // Previously these actions were handled identically to
            // DX_RECOVERY_DONE (only the in-progress flag was cleared and the
            // loop continued), which left dxrtd "active" on a wedged device —
            // e.g. after a fatal AER (malformed TLP) hangs the FW. Broadcast
            // the fatal fault and terminate so systemd restarts; if the FW is
            // genuinely hung the restart's bounded IDENTIFY retry fails fast
            // and the service stays down, which is the correct "blocked"
            // signal for operator intervention.
            if (recovery.action == DX_RECOVERY_FW_HANG
                || recovery.action == DX_RECOVERY_PERM_FAIL)
            {
                LOG_DXRT_S_ERR(std::string("[EventLoop] Terminal recovery failure: action=")
                    + RecoverySubcodeToString(recovery.action)
                    + " reason=" + RecoveryReasonToString(recovery.reason)
                    + " fail_count=" + std::to_string(recovery.recovery_fail_count)
                    + " on device " + std::to_string(_deviceId)
                    + ". Device is unusable; surfacing fatal fault and terminating dxrtd.");
                if (_onError)
                    _onError(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT,
                             recovery.reason, _deviceId, nullptr);
                std::quick_exit(EXIT_FAILURE);
            }

            if (IsDriverOwnedRecoveryReason(recovery.reason))
            {
                driverOwnedRecoveryReason = recovery.reason;
                switch (recovery.action)
                {
                    case DX_RECOVERY_STARTED:
                        driverOwnedRecoveryActive = true;
                        break;
                    case DX_RECOVERY_DONE:
                        driverOwnedRecoveryActive = false;
                        break;
                    default:
                        break;
                }

                if (recovery.action == DX_RECOVERY_STARTED)
                {
                    LOG_DXRT_S << "Driver-owned recovery started (reason="
                        << RecoveryReasonToString(recovery.reason)
                        << "). Broadcasting to clients and restarting dxrtd." << endl;
                    if (_onError)
                        _onError(dxrt_server_err_t::S_ERR_NEED_DEV_RECOVERY,
                                 recovery.reason, _deviceId, nullptr);
                    LOG_DXRT_S << "Driver-owned recovery notification handled. "
                        << "Terminating dxrtd for systemd restart." << endl;

                    /* Terminate immediately to allow systemd to restart the service.
                       Do not wait for the recovery to complete, as the driver will
                       be waiting for this process to exit. */
                    std::quick_exit(EXIT_FAILURE);
                }
            }

            loopCnt++;
            continue;
        }

        bool is_error_event =
            (eventType == dxrt::dxrt_event_t::DXRT_EVENT_ERROR);
        bool is_not_none_error =
            (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code) != dxrt::dxrt_error_t::ERR_NONE);
        if (is_error_event && is_not_none_error)
        {
            uint32_t err_code = eventInfo.dx_rt_err.err_code;
            LOG_DXRT_S_ERR(eventInfo.dx_rt_err);

            cout << "************************************************************************" << endl;
            cout << " * Error occurred! Please follow the steps below to recover the device." << endl;
            cout << " * Refer to the user guide if additional help is needed." << endl;
            cout << endl;
            cout << " Step 1: Reset the device using dxrt-cli" << endl;
            cout << "         > dxrt-cli -r 0" << endl;
            cout << " Step 2: Retry the inference using run_model" << endl;
            cout << "         > run_model -m [model.dxnn]" << endl;
            cout << " ** If the error persists, please contact DeepX support for assistance." << endl;
            cout << "************************************************************************" << endl;

            // Classify error by code range
            //   100-103: DMA timeout + soft reset failure
            //   200-201: LPDDR ECC error
            //   300:     FW timeout
            //   400-403: DMA HW abort (Abort MSI)
            bool isRecoverable = (err_code >= 100 && err_code < 200)
                              || (err_code == 300)
                              || (err_code >= 400 && err_code < 500);

            if (isRecoverable)
            {
                // Any recoverable error (DMA abort 400-499 or FW timeout 300) during
                // an active driver-owned kernel recovery (cpu_reset / link_flap):
                // issuing DXRT_CMD_RECOVERY from user space would conflict with the
                // kernel's ongoing recovery sequence. Broadcast the fault and terminate
                // so systemd restarts the service with a clean state. In this
                // transitional window, even other recoverable fault codes should
                // avoid user-space recovery ioctl to prevent dual-recovery races.
                if (driverOwnedRecoveryActive)
                {
                    LOG_DXRT_S_ERR("[EventLoop] Recoverable error (code="
                        + std::to_string(err_code) + ") arrived during driver-owned recovery reason="
                        + RecoveryReasonToString(driverOwnedRecoveryReason)
                        + ". Broadcasting to clients and terminating.");
                    LogDmaChannelStatus(&eventInfo.dx_rt_err, err_code);

                    if (_onError)
                        _onError(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, err_code, _deviceId,
                                 &eventInfo.dx_rt_err);

                    std::quick_exit(EXIT_FAILURE);
                }

                LOG_DXRT_S_ERR("[EventLoop] Recoverable error (code="
                    + std::to_string(err_code) + ") on device " + std::to_string(_deviceId)
                    + "). Performing recovery.");
                LogDmaChannelStatus(&eventInfo.dx_rt_err, err_code);
                TriggerRecovery(err_code);
                LOG_DXRT_S << "Recovery completed (EventLoop) for device " << _deviceId
                    << ". Terminating dxrtd for systemd restart." << endl;

                // Ensure all in-process runtime state is rebuilt from a fresh start
                // (task/model setup, scheduler queues, and client-side re-init flow).
                std::quick_exit(EXIT_FAILURE);
            }
            else
            {
                if (_onError)
                    _onError(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, err_code, _deviceId,
                             &eventInfo.dx_rt_err);

                Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
                std::abort();
            }
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)
                 == dxrt::dxrt_event_t::DXRT_EVENT_NOTIFY_THROT)
        {
            dx_pcie_dev_ntfy_throt_t throtInfo = eventInfo.dx_rt_ntfy_throt;
            LOG_DXRT_S_DBG << "Received throttling event: code=" << throtInfo.ntfy_code
                << ", freq_before=" << throtInfo.throt_freq[0]
                << ", freq_after="  << throtInfo.throt_freq[1]
                << ", volt_before=" << throtInfo.throt_voltage[0]
                << ", volt_after="  << throtInfo.throt_voltage[1]
                << ", temperature=" << throtInfo.throt_temper
                << std::endl;
            if (_onThrottle)
                _onThrottle(throtInfo, _deviceId);
        }

        loopCnt++;
    }
    LOG_DXRT_S_DBG << "@@@ Thread End : EventLoop, loopCount:" << loopCnt << std::endl;
    return 0;
}

void DeviceDispatcher::LogDmaChannelStatus(const dx_pcie_dev_err_t* err, uint32_t err_code) const
{
    if (err_code >= 400 && err_code < 500)
    {
        int abort_ch = static_cast<int>(err_code - 400);
        LOG_DXRT_S << "[DMA ABORT] Channel " << abort_ch << endl;
    }
    else if (err_code >= 100 && err_code < 200)
    {
        int fail_ch = static_cast<int>(err_code - 100);
        LOG_DXRT_S << "[DMA FAIL] Channel " << fail_ch << endl;
    }
    LOG_DXRT_S << "  err_status=0x" << std::hex << std::setfill('0') << std::setw(8) << err->dma_err << std::dec << endl;
    LOG_DXRT_S << "  WR ch status: ["
        << err->dma_wr_ch_sts[0] << ", "
        << err->dma_wr_ch_sts[1] << ", "
        << err->dma_wr_ch_sts[2] << ", "
        << err->dma_wr_ch_sts[3] << "]" << endl;
    LOG_DXRT_S << "  RD ch status: ["
        << err->dma_rd_ch_sts[0] << ", "
        << err->dma_rd_ch_sts[1] << ", "
        << err->dma_rd_ch_sts[2] << ", "
        << err->dma_rd_ch_sts[3] << "]" << endl;
    LOG_DXRT_S << "  PCIe BDF: " << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(err->bus) << ":"
        << std::setw(2) << static_cast<int>(err->dev) << "."
        << static_cast<int>(err->func) << std::dec << endl;
}

void DeviceDispatcher::OnTimerTick()
{
    for (auto& t : _timer)
    {
        t.onTick();
    }
}

bool DeviceDispatcher::FillDeviceSpec(dxrt_device_info_t* spec, dxrt_dev_info_t* devInfo)
{
    if (spec == nullptr || devInfo == nullptr)
    {
        return false;
    }

    auto cachedSpec = _core->info();
    auto cachedDevInfo = _core->devInfo();

    // Prefer already-cached identify data when available.
    if (cachedSpec.mem_size > 0)
    {
        *spec = cachedSpec;
        *devInfo = cachedDevInfo;
        return true;
    }

    // Fallback: try reading device identify info directly.
    _core->Identify(_deviceId);

    const auto refreshedSpec = _core->info();
    if (refreshedSpec.mem_size == 0)
    {
        return false;
    }

    *spec = refreshedSpec;
    *devInfo = _core->devInfo();
    return true;
}

void DeviceDispatcher::SetMemoryStatsProvider(MemoryStatsProvider provider)
{
    std::lock_guard<std::mutex> lock(_memoryStatsMutex);
    _memoryStatsProvider = std::move(provider);
}

bool DeviceDispatcher::GetMemoryStats(uint64_t* total, uint64_t* used, uint64_t* free) const
{
    if (total == nullptr || used == nullptr || free == nullptr)
    {
        return false;
    }

    MemoryStatsProvider provider;
    {
        std::lock_guard<std::mutex> lock(_memoryStatsMutex);
        provider = _memoryStatsProvider;
    }

    if (!provider)
    {
        const auto info = _core->info();
        *total = info.mem_size;
        *used = 0;
        *free = info.mem_size;
        return true;
    }

    return provider(total, used, free);
}

double DeviceDispatcher::GetUsage(int channel) const
{
    if (channel < 0 || channel >= kResponseChannels)
    {
        LOG_DXRT_S_ERR("Invalid channel for GetUsage: " + std::to_string(channel));
        return 0.0;
    }
    return _timer[channel].getUsage();
}

void DeviceDispatcher::TriggerRecovery(uint32_t errCode)
{
    LOG_DXRT_S << "TriggerRecovery: device=" << _deviceId << ", errCode=" << errCode << endl;

    // step 1+2: Pause DMA requests via recovery adapter and wait for ACK.
    LOG_DXRT_S_DBG << "Step 1+2: PauseForRecovery - waiting for DMA completion..." << endl;
    if (_recoveryAdapter)
    {
        bool pauseOk = _recoveryAdapter->PauseForRecovery(errCode, _deviceId);
        if (!pauseOk)
        {
            LOG_DXRT_S_ERR("PauseForRecovery timeout/failure for device " + std::to_string(_deviceId));
            std::abort();
        }
    }
    else if (_onRecovery)
    {
        // Fallback path for legacy integration (no adapter wired).
        LOG_DXRT_S_DBG << "Step 1+2: Using legacy recovery callback" << endl;
        _onRecovery(dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, errCode, _deviceId);
    }

    // step 3: call RECOVERY ioctl to trigger device recovery in kernel
    LOG_DXRT_S_DBG << "Step 3: Calling DXRT_CMD_RECOVERY ioctl..." << endl;
    int recovery_ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
    if (recovery_ret < 0)
    {
        LOG_DXRT_S_ERR("DXRT_CMD_RECOVERY failed for device " + std::to_string(_deviceId)
            + " ret=" + std::to_string(recovery_ret) + ". Aborting.");
        if (_recoveryAdapter)
        {
            _recoveryAdapter->OnRecoveryFailed(_deviceId);
        }
        std::abort();
    }
    LOG_DXRT_S_DBG << "Step 3: DXRT_CMD_RECOVERY completed with ret=" << recovery_ret << endl;

    // EventLoop for one dispatcher is single-threaded; recovery flow Process()
    // calls here are serialized for this device.
    dxrt_device_info_t info{};
    int info_ret = Process(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, &info, sizeof(info));
    if (info_ret != 0)
    {
        LOG_DXRT_S_ERR("DXRT_CMD_IDENTIFY_DEVICE failed ret=" + std::to_string(info_ret)
            + " after recovery for device " + std::to_string(_deviceId) + ". Device unusable; aborting.");
        if (_recoveryAdapter)
        {
            _recoveryAdapter->OnRecoveryFailed(_deviceId);
        }
        std::abort();
    }
    LOG_DXRT_S_DBG << "DXRT_CMD_IDENTIFY_DEVICE succeeded - device is READY" << endl;

    // step 4: notify upper layer that recovery completed, resume normal operation
    if (_recoveryAdapter)
    {
        _recoveryAdapter->ResumeAfterRecovery(_deviceId);
    }
    LOG_DXRT_S_DBG << "TriggerRecovery complete: device=" << _deviceId << endl;
}


}  // namespace dxrt
