/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <array>
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_core.h"
#include "dxrt/profiler.h"
#include "dxrt/usage_timer.h"
#include "service_error.h"
#include "dxrt/device_struct.h"

namespace dxrt {

// DeviceDispatcher
//   Owns the NPU response polling threads (one per DMA channel, fixed at 3)
//   and the event polling thread (one per device).  Both groups talk to the
//   same DriverAdapter (DeviceCore) via ioctl, so they are bundled here.
//
//   Usage:
//     auto d = std::make_unique<DeviceDispatcher>(driverAdapter, deviceId,
//                                                 timer, profiler);
//     d->SetResponseCallback(...);
//     d->SetErrorCallback(...);
//     d->SetRecoveryCallback(...);
//     d->SetThrottleCallback(...);
//     d->SetRecoveryAdapter(...);  // optional; wire before StartThreads()
//     d->StartThreads();          // launches 3 + 1 threads
//     ...
//     d->RequestStop(); d->Join();
class DeviceDispatcher
{
 public:
    static constexpr int kResponseChannels = 3;

    // Interface used by TriggerRecovery to coordinate with the upper layer.
    // The three methods map to the phases in the DMA abort recovery document:
    //   PauseForRecovery   — Step 1+2: set recovery flag, drain in-flight DMA.
    //   ResumeAfterRecovery — Step 4: reset state, resume normal operation.
    //   OnRecoveryFailed   — Fatal path when DXRT_CMD_RECOVERY ioctl fails.
    class IRecoveryAdapter
    {
     public:
        virtual ~IRecoveryAdapter() = default;
        virtual bool PauseForRecovery(uint32_t errCode, int deviceId) = 0;
        virtual void ResumeAfterRecovery(int deviceId) = 0;
        virtual void OnRecoveryFailed(int deviceId) = 0;
    };

    using ResponseCallback = std::function<void(const dxrt_response_t&)>;
    using ErrorCallback    = std::function<void(dxrt_server_err_t, uint32_t, int,
                                                const dx_pcie_dev_err_t*)>;
    using RecoveryCallback = std::function<void(dxrt_server_err_t, uint32_t, int)>;
    using ThrottleCallback = std::function<void(dx_pcie_dev_ntfy_throt_t, int)>;
    using MemoryStatsProvider = std::function<bool(uint64_t* total, uint64_t* used, uint64_t* free)>;

    DeviceDispatcher(std::shared_ptr<DeviceCore> core,
                     std::array<UsageTimer, kResponseChannels>& timer,
                     Profiler& profiler);
    ~DeviceDispatcher();

    DeviceDispatcher(const DeviceDispatcher&)            = delete;
    DeviceDispatcher& operator=(const DeviceDispatcher&) = delete;
    DeviceDispatcher(DeviceDispatcher&&)                 = delete;
    DeviceDispatcher& operator=(DeviceDispatcher&&)      = delete;

    // Callback registration.  All callbacks must be set before StartThreads();
    // they are read without locking from the worker threads.
    void SetResponseCallback(ResponseCallback cb) { _onResponse = std::move(cb); }
    void SetErrorCallback(ErrorCallback cb)       { _onError    = std::move(cb); }
    void SetRecoveryCallback(RecoveryCallback cb) { _onRecovery = std::move(cb); }
    void SetThrottleCallback(ThrottleCallback cb) { _onThrottle = std::move(cb); }

    // Optional recovery adapter.  Must be set before StartThreads().
    // When set, TriggerRecovery delegates step 1/2 and step 4 signals to
    // this adapter instead of (only) calling the legacy RecoveryCallback.
    void SetRecoveryAdapter(std::shared_ptr<IRecoveryAdapter> adapter)
    {
        _recoveryAdapter = std::move(adapter);
    }

    // Launches kResponseChannels response threads + 1 event thread.
    void StartThreads();

    // Signal all worker threads to stop and join them.  Idempotent.
    // (Kept minimal for now — refinement deferred to caller.)
    void RequestStop() { _stop.store(true); }
    void Join();

    bool IsBlocked() const { return _isBlocked.load(); }
    int GetDeviceId() const { return _deviceId; }

    void OnTimerTick();
    double GetUsage(int channel) const;
    dxrt_device_status_t GetStatus() const { return _core->Status(); }
    bool FillDeviceSpec(dxrt_device_info_t* spec, dxrt_dev_info_t* devInfo);
    void SetMemoryStatsProvider(MemoryStatsProvider provider);
    bool GetMemoryStats(uint64_t* total, uint64_t* used, uint64_t* free) const;

private:
    int  Process(dxrt_cmd_t cmd, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) const;
    int  ResponseLoop(int channel);
    int  EventLoop();
    void LogDmaChannelStatus(const dx_pcie_dev_err_t* err, uint32_t err_code) const;
    void TriggerRecovery(uint32_t errCode);

    std::shared_ptr<DeviceCore>                   _core;
    int                                           _deviceId;
    std::array<UsageTimer, kResponseChannels>&    _timer;
    Profiler&                                     _profiler;

    std::array<std::thread, kResponseChannels>    _responseThreads;
    std::thread                                   _eventThread;

    std::atomic<bool>                             _stop{false};
    std::atomic<bool>                             _isBlocked{false};

    ResponseCallback _onResponse;
    ErrorCallback    _onError;
    RecoveryCallback _onRecovery;
    ThrottleCallback _onThrottle;

    std::shared_ptr<IRecoveryAdapter>             _recoveryAdapter;

    mutable std::mutex _memoryStatsMutex;
    MemoryStatsProvider _memoryStatsProvider;
};

}  // namespace dxrt
