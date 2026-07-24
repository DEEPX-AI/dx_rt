/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dxrt/common.h"
#include "dxrt/handler_que_template.h"
#include "../dynamic_ipc/protocol/ipc_packet_handler_registry.hpp"
#include "../dynamic_ipc/transport/ipc_server_endpoint.hpp"
#include "../dynamic_ipc/shm/memfd_service.h"
#include "../dynamic_ipc/shm/shm.h"
#include "memory_service.hpp"
#include "process_task_info_store.hpp"
#include "scheduler_service.h"
#include "service_device.h"
#include "service_error.h"

namespace dxrt {

class SharedMemoryWritingThread;

enum class DxrtServiceV2IpcPollDecision
{
    ContinuePolling,
    ExitPolling,
    StopAllServers
};

inline DxrtServiceV2IpcPollDecision EvaluateDxrtServiceV2IpcPollResult(
    int pollRc,
    bool serverStartedAfterPoll)
{
    if (pollRc >= 0)
    {
        return DxrtServiceV2IpcPollDecision::ContinuePolling;
    }

    if (!serverStartedAfterPoll)
    {
        return DxrtServiceV2IpcPollDecision::ExitPolling;
    }

    return DxrtServiceV2IpcPollDecision::StopAllServers;
}


enum class DXRTScheduleV2
{
    FIFO,
    RoundRobin,
    SJF
};

class DXRT_INTERNAL_API DxrtServiceV2 : public ISchedulerListener
{
 public:
    explicit DxrtServiceV2(DXRTScheduleV2 schedulerOption = DXRTScheduleV2::FIFO);
    explicit DxrtServiceV2(
        std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices,
        DXRTScheduleV2 schedulerOption = DXRTScheduleV2::FIFO);
    ~DxrtServiceV2();

    void onClientConnected(pid_t pid);
    void onClientDisconnected(pid_t pid);

    SharedMemoryInfo HandleAllocate(pid_t pid, int deviceId, int taskId, size_t size, bool forModel);
    int HandleDeallocate(pid_t pid, int deviceId, uint64_t address);
    int HandleInferenceRequest(pid_t pid, int deviceId, const dxrt_request_acc_t &request);
    int HandleTaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize, const TaskStaticConfig &config);
    int HandleTaskDeInit(pid_t pid, int deviceId, int taskId);
    int HandleProcessDeInit(pid_t pid, int deviceId);
    int HandleDMARead(pid_t pid, int seqId, int deviceId, int64_t blockId, uint64_t blockOffset, uint64_t size);
    int HandleDMAWrite(pid_t pid, int seqId, int deviceId, int64_t blockId, uint64_t blockOffset, uint64_t size);
    int HandleViewMemory(int deviceId, bool viewUsedMemory, uint64_t *bytes) const;
    int HandleViewAvailableDevice(uint64_t *availableMask) const;
    int HandleGetUsage(int deviceId, int channel, double *usage) const;
    int HandleGetDeviceTelemetry(int deviceId, IPCDeviceTelemetryPayload *telemetry) const;
    int HandleDMAReadWithFaultInjection(pid_t pid, int deviceId, int64_t blockId, uint64_t blockOffset, uint64_t size);

    int StartIpcServer(const std::string &endpoint, int backlog);
    int StartIpcServers(const std::vector<std::string> &endpoints, int backlog);
    int RunIpcServer(int timeoutMs = 1000);
    void StopIpcServer();

    void onCompleteInference(const dxrt::dxrt_response_t &response, int deviceId);
    void BroadcastThrottlingEventToClient(dxrt::dx_pcie_dev_ntfy_throt_t throtInfo, int deviceId);
    void ErrorBroadCastToClient(
        dxrt::dxrt_server_err_t err,
        uint32_t errCode,
        int deviceId,
        const dx_pcie_dev_err_t *errorInfo = nullptr);

    bool IsTaskValid(pid_t pid, int deviceId, int taskId);
    void Dispose();

    // ISchedulerListener
    void onInferenceComplete(const dxrt::dxrt_response_t& response, int deviceId) override;
    void onSchedulerError(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId) override;
    bool validateTask(pid_t pid, int deviceId, int taskId) override;
    void onTaskDrained(pid_t pid, int taskId) override;

 private:
    struct IpcServerContext;
    struct IpcServerWorkItem;

    // Recovery adapter wired into each ServiceDevice's DeviceDispatcher.
    // SERVICE ON path: PauseForRecovery broadcasts error to all clients and
    // waits for them to die (ACK); ResumeAfterRecovery clears the recovery flag.
    class ServiceRecoveryAdapter : public DeviceDispatcher::IRecoveryAdapter {
    public:
        explicit ServiceRecoveryAdapter(DxrtServiceV2* service) : _service(service) {}
        bool PauseForRecovery(uint32_t errCode, int deviceId) override;
        void ResumeAfterRecovery(int deviceId) override;
        [[noreturn]] void OnRecoveryFailed(int deviceId) override;
    private:
        DxrtServiceV2* _service;
    };

    void CacheMemoryServices();
    dxrt::MemoryService *GetMemoryService(int deviceId) const;
    void WaitForAllClientsDead(int timeoutMs);
    void RecoveryBroadcastAndWait(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId);
    int sendToPidAcrossServers(pid_t pid, const uint8_t *data, size_t size, int fd = -1);
    void setupIpcPacketRegistry(IPCPacketHandlerRegistry *registry, IPCServerEndpoint *server);
    void startIpcPollThread(IpcServerContext *context);
    void runIpcPollLoop(IpcServerContext *context);
    void handleIpcPollFailure(int pollRc);
    int handleIpcServerWorkItem(const IpcServerWorkItem &workItem, int threadId);
    void requestAllIpcServerStops();
    void TrackSharedMemoryHandle(int deviceId, pid_t pid, const SharedMemoryInfo &info);
    bool TryResolveSharedMemoryHandle(int deviceId, pid_t pid, uint64_t address, SharedMemoryInfo *info);
    void ReleaseSharedMemoryHandle(const SharedMemoryInfo &info);
    void ReleaseAllSharedMemoryHandlesForProcess(int deviceId, pid_t pid);

    bool TaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize, const TaskStaticConfig &config);
    void TaskDeInit(int deviceId, int taskId, int pid);

    struct SharedMemHandle {
        int fd{-1};
        void *ptr{nullptr};
        uint64_t size{0};
        uint64_t physAddrOffset{0};
    };

    bool TryGetSharedMemoryHandle(int deviceId, pid_t pid, uint64_t address, SharedMemHandle *handle);

    // DMA recovery helpers
    void IncrementPendingDMA();
    void DecrementPendingDMA();
    bool WaitForDMACompletion(int timeoutMs);

    std::vector<std::shared_ptr<dxrt::ServiceDevice>> _devices;
    std::shared_ptr<SchedulerService> _scheduler;

    // Lock acquisition order — must always be respected to prevent deadlock:
    //   1. _pidSetMutex          guards _pidSet (pid presence tracking)
    //   2. _deviceMutex          guards _devices + BoundManager state
    //   3. _pendingTaskFreeMutex  guards deferred cleanup map (_pendingTaskFree)
    //   4. _sharedMemoryLock     guards _sharedMemoryHandles (3-level map)
    //
    // Per-domain locks acquired independently (never nested with each other
    // or with the above four locks):
    //   SchedulerService::_lock   — internal scheduler request queue
    //   MemoryService::_lock      — per-device memory allocation records
    //
    // Rule: do NOT hold any of these locks while calling IPC, external
    // callbacks, or other layer methods. Release the lock first, capture
    // any needed data by value, then call outward.
    std::mutex _deviceMutex;
    std::mutex _pidSetMutex;
    std::mutex _sharedMemoryLock;

    std::set<pid_t> _pidSet;
    ProcessTaskInfoStore _taskInfoStore;

    // Tracks tasks whose memory deallocation is deferred until NPU completes
    // the in-flight request (CANCELLED state).  Key = {pid, taskId}, value = deviceId.
    // Protected by _pendingTaskFreeMutex; accessed from both IPC handler thread
    // (HandleProcessDeInit) and OutputReceiverThread (_onTaskDrained callback).
    std::mutex _pendingTaskFreeMutex;
    std::map<std::pair<pid_t, int>, int> _pendingTaskFree;
    std::map<int, std::map<pid_t, std::map<uint64_t, SharedMemHandle>>> _sharedMemoryHandles;

    std::map<int, dxrt::MemoryService *> _memoryServices;
    std::unique_ptr<SharedMemoryWritingThread> _sharedMemoryWritingThread;
    std::vector<std::unique_ptr<IpcServerContext>> _ipcServers;
    std::unique_ptr<HandlerQueueThread<IpcServerWorkItem>> _ipcHandlerQueue;
    dxrt::shm::MemFDService _memfdService;

    std::atomic<bool> _ipcServerStarted{false};
    std::atomic<int> _ipcServerRunRc{0};

    // DMA completion tracking
    std::mutex _dmaMutex;
    std::condition_variable _dmaCompletionCv;
    std::atomic<int> _pendingDmaOperations{0};

    std::atomic<bool> _recoveryInProgress{false};
};
}  // namespace dxrt
