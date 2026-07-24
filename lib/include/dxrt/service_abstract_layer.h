/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <array>
#include <string>
#include <unordered_map>

#include "dxrt/device_core.h"
#include "dxrt/driver.h"
#include "dxrt/exception/exception.h"
#include "dxrt/exception/server_err.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/memory.h"
#include "dxrt/service_util.h"
#include "dxrt/usage_timer.h"
#include "../../dynamic_ipc/protocol/memory_type.hpp"
#include "../../dynamic_ipc/shm/shm.h"
#include "../../device_pool/inference_context.h"

#ifdef __linux__
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace dxrt {

class MultiprocessMemory;
class SharedMemoryWriter;
struct ServiceLayerDynamicIpcClient;
struct ServiceLayerDynamicIpcClientDeleter {
    void operator()(ServiceLayerDynamicIpcClient *client) const;
};
using ServiceLayerDynamicIpcClientOwner =
    std::unique_ptr<ServiceLayerDynamicIpcClient, ServiceLayerDynamicIpcClientDeleter>;

class DXRT_API ServiceLayerInterface {
public:
    virtual void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut = nullptr) = 0;
    virtual void SignalDeviceReset(int id) = 0;
    virtual SharedMemoryInfo AllocateInfo(int deviceId, int taskId, MemoryType type, uint64_t size) = 0;
    virtual void DeAllocateInfo(int deviceId, const SharedMemoryInfo& info) = 0;
    uint64_t Allocate(int deviceId, uint64_t size) {
        SharedMemoryInfo info = AllocateInfo(deviceId, -1, MemoryType::Normal, size);
        if (info.phys_addr() == static_cast<uint64_t>(-1)) {
            throw std::runtime_error("Allocation failed for device " + std::to_string(deviceId));
        }
        return info.phys_addr();
    }

    uint64_t Allocate(int deviceId, int taskId, MemoryType type, uint64_t size) {
        SharedMemoryInfo info = AllocateInfo(deviceId, taskId, type, size);
        return info.phys_addr();
    }

    void DeAllocate(int deviceId, int64_t addr)
    {
        SharedMemoryInfo info{};
        info.block_id = static_cast<uint64_t>(addr);
        info.set_phys_addr(static_cast<uint64_t>(addr));
        info.fd = -1;
        info.ptr = nullptr;
        DeAllocateInfo(deviceId, info);
    }
    virtual void SignalEndJobs(int id) = 0;
    virtual void CheckServiceRunning() = 0;
    virtual bool isRunOnService() const = 0;
    virtual void RegisterDeviceCore(DeviceCore *core) = 0;
    virtual void RegisterInferenceResponseHandler(int deviceId, std::function<void(const dxrt_response_t &)> handler) = 0;
    virtual void RegisterErrorHandler(
        int deviceId,
        std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler) = 0;
    virtual void RegisterThrottleHandler(
        int deviceId,
        std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler) = 0;
    virtual void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config) = 0;
    virtual void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) = 0;
    virtual void SignalStoppedDmaToWaitRecovery(int deviceId) = 0;
    virtual void SignalStoppedDmaToWaitRecovery(int deviceId, uint32_t recoveryId)
    {
        std::ignore = recoveryId;
        SignalStoppedDmaToWaitRecovery(deviceId);
    }

    // Recovery coordination hooks called from DeviceTaskLayer::triggerRecovery().
    // SERVICE OFF: NoServiceLayer implements these via AccDeviceTaskLayer callbacks.
    // SERVICE ON:  ServiceLayer stubs (DeviceDispatcher::IRecoveryAdapter handles that path).
    virtual void PauseForRecovery(int deviceId) = 0;
    virtual void ResumeAfterRecovery(int deviceId) = 0;
    [[noreturn]] virtual void OnRecoveryFailed(int deviceId) = 0;
    virtual int DMARead(SharedMemoryView view)
    {
        std::ignore = view;
        return -1;
    }
    virtual int DMAWrite(SharedMemoryView view)
    {
        std::ignore = view;
        return -1;
    }

    int DMARead(int deviceId, uint64_t sourceAddress, uint64_t destinationAddress, uint64_t size)
    {
        return DMARead(SharedMemoryView::ofWhole(
            MakeDMAInfo(deviceId, destinationAddress, sourceAddress, size)));
    }

    int DMAWrite(int deviceId, uint64_t sourceAddress, uint64_t destinationAddress, uint64_t size)
    {
        return DMAWrite(SharedMemoryView::ofWhole(
            MakeDMAInfo(deviceId, sourceAddress, destinationAddress, size)));
    }

    int DMARead(const SharedMemoryInfo &mem)
    {
        return DMARead(SharedMemoryView::ofWhole(mem));
    }

    int DMAWrite(const SharedMemoryInfo &mem)
    {
        return DMAWrite(SharedMemoryView::ofWhole(mem));
    }

    // Convenience: build a view from (base info, offset, size) and forward.
    int DMARead(SharedMemoryInfo mem, uint64_t start, uint64_t size)
    {
        SharedMemoryView v;
        v.info   = mem;
        v.offset = start;
        v.size   = size;
        return DMARead(v);
    }
    int DMAWrite(SharedMemoryInfo mem, uint64_t start, uint64_t size)
    {
        SharedMemoryView v;
        v.info   = mem;
        v.offset = start;
        v.size   = size;
        return DMAWrite(v);
    }
    virtual int DMAReadWithFaultInjection(SharedMemoryView view)
    {
        std::ignore = view;
        DXRT_ASSERT(false, "DMAReadWithFaultInjection abstract method, "
            "fault injection is only for testing and should not be called in production");
        return -1;
    }

    virtual ~ServiceLayerInterface() = default;
    virtual void CommitMemory(const SharedMemoryInfo &info) { std::ignore = info; }
    virtual void InvalidateMemory(const SharedMemoryInfo &info) { std::ignore = info; }
private:
    static SharedMemoryInfo MakeDMAInfo(int deviceId, uint64_t hostAddress, uint64_t deviceAddress, uint64_t size)
    {
        SharedMemoryInfo info{};
        info.deviceid = deviceId;
        info.ptr = reinterpret_cast<void *>(hostAddress);
        info.set_phys_addr(deviceAddress);
        info.size = size;
        return info;
    }
};

class DXRT_API ServiceLayer : public ServiceLayerInterface {
public:
    explicit ServiceLayer();
    void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut = nullptr) override;
    void SignalDeviceReset(int id) override;
    SharedMemoryInfo AllocateInfo(int deviceId, int taskId, MemoryType type, uint64_t size) override;

    void DeAllocateInfo(int deviceId, const SharedMemoryInfo& info) override;
    void SignalEndJobs(int id) override;
    void CheckServiceRunning() override;
    bool isRunOnService() const override;
    void RegisterDeviceCore(DeviceCore *core) override;
    void RegisterInferenceResponseHandler(int deviceId, std::function<void(const dxrt_response_t &)> handler) override;
    void RegisterErrorHandler(
        int deviceId,
        std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler) override;
    void RegisterThrottleHandler(
        int deviceId,
        std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler) override;
    void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config) override;
    void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
    void SignalStoppedDmaToWaitRecovery(int deviceId) override;
    void SignalStoppedDmaToWaitRecovery(int deviceId, uint32_t recoveryId) override;
    void PauseForRecovery(int /*deviceId*/) override {}
    void ResumeAfterRecovery(int /*deviceId*/) override {}
    [[noreturn]] void OnRecoveryFailed(int /*deviceId*/) override { std::abort(); }
    void CommitMemory(const SharedMemoryInfo &info) override;
    void InvalidateMemory(const SharedMemoryInfo &info) override;
    int DMARead(SharedMemoryView view) override;
    int DMAWrite(SharedMemoryView view) override;
    int DMAReadWithFaultInjection(SharedMemoryView view) override;

    ~ServiceLayer() override = default;
private:
    void EnsureDynamicIPCConnected();  // must be called with _ipcMutex held

    // Locked implementations — called only with _ipcMutex already acquired.
    void HandleInferenceAccLocked(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut);
    SharedMemoryInfo AllocateInfoLocked(int deviceId, int taskId, MemoryType memoryType, uint64_t size);
    void DeAllocateInfoLocked(int deviceId, const SharedMemoryInfo &info);
    void SignalTaskInitLocked(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config);
    void SignalTaskDeInitLocked(int deviceId, int taskId, npu_bound_op bound);
    int DMAWriteLocked(SharedMemoryView view);
    int DMAReadLocked(SharedMemoryView view);
    int DMAReadWithFaultInjectionLocked(SharedMemoryView view);


    void DispatchInferenceResponse(int deviceId, const dxrt_response_t &response);
    void DispatchErrorNotification(
        int deviceId,
        dxrt_server_err_t err,
        int value,
        const dx_pcie_dev_err_t &errorInfo);

    void DispatchThrottleNotification(int deviceId, const dx_pcie_dev_ntfy_throt_t &throtInfo);

    ServiceLayerDynamicIpcClientOwner _dynamicIpcClient;
    bool _dynamicIpcConnected{false};
    std::mutex _ipcMutex;
    std::mutex _responseHandlerMutex;
    std::unordered_map<int, std::function<void(const dxrt_response_t &)>> _inferenceResponseHandlers;
    std::mutex _errorHandlerMutex;
    std::unordered_map<int, std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)>> _errorHandlers;
    std::mutex _throttleHandlerMutex;
    std::unordered_map<int, std::function<void(const dx_pcie_dev_ntfy_throt_t &)>> _throttleHandlers;
};

class DXRT_API NoServiceLayer : public ServiceLayerInterface {
public:
    NoServiceLayer();
    ~NoServiceLayer() override;

    void HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut = nullptr) override;
    void SignalDeviceReset(int id) override;
    SharedMemoryInfo AllocateInfo(int deviceId, int taskId, MemoryType memoryType, uint64_t size) override;
    void DeAllocateInfo(int deviceId, const SharedMemoryInfo& info) override;
    void SignalEndJobs(int id) override;
    void CheckServiceRunning() override;
    bool isRunOnService() const override;
    void RegisterDeviceCore(DeviceCore *core) override;
    void RegisterInferenceResponseHandler(int deviceId, std::function<void(const dxrt_response_t &)> handler) override;
    void RegisterErrorHandler(
        int deviceId,
        std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler) override;
    void RegisterThrottleHandler(
        int deviceId,
        std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler) override;
    void SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config) override;
    void SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound) override;
    void SignalStoppedDmaToWaitRecovery(int deviceId) override;
    void SignalStoppedDmaToWaitRecovery(int deviceId, uint32_t recoveryId) override;
    void PauseForRecovery(int deviceId) override;
    void ResumeAfterRecovery(int deviceId) override;
    [[noreturn]] void OnRecoveryFailed(int deviceId) override;
    // Register callbacks from AccDeviceTaskLayer::StartThread so that
    // PauseForRecovery/ResumeAfterRecovery can drive _dmaStopGate.
    void RegisterRecoveryCallbacks(
        int deviceId,
        std::function<void()> onPause,
        std::function<void()> onResume);
    void CommitMemory(const SharedMemoryInfo &info) override;
    void InvalidateMemory(const SharedMemoryInfo &info) override;
    int DMARead(SharedMemoryView view) override;
    int DMAWrite(SharedMemoryView view) override;
    int DMAReadWithFaultInjection(SharedMemoryView view) override;

    // NPU utilization tracking (NoService mode only)
    void addUsage(int deviceId, int coreId, double value);
    double getUsage(int deviceId, int coreId);
    void onTick(int deviceId, int coreId);
private:
    std::map<int, std::shared_ptr<Memory>> _mems;
    std::map<int, DeviceCore*> _ptr;

    // NPU utilization tracking per device (NoService mode)
    std::map<int, std::array<UsageTimer, 3>> _usageTimers;  // deviceId -> [3 DMA channels]

    // Shared memory writer for external monitoring tools
    std::unique_ptr<SharedMemoryWriter> _shmWriter;

    // Usage monitoring thread (0.5-second periodic onTick() calls)
    std::thread _usageMonitorThread;
    std::atomic<bool> _usageMonitorRunning{false};
    bool _useExternalShmWriter{false};
    void UsageMonitorThread();  // Thread function for periodic monitoring

    // SharedMemoryInfo tracking for NoService mode.
    std::atomic<uint64_t> _nextNoServiceBlockId{1};
    std::unordered_map<uint64_t, uint64_t> _blockIdToAddr;
    std::mutex _sharedMemoryLock;

    // Per-device recovery callbacks registered by AccDeviceTaskLayer::StartThread.
    std::map<int, std::function<void()>> _pauseCallbacks;
    std::map<int, std::function<void()>> _resumeCallbacks;
};

} // namespace dxrt
