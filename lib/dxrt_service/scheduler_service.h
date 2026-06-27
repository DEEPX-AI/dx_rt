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


#include <csignal>
#include <atomic>
#include <thread>
#include <mutex>
#include <memory>
#include <utility>
#include <queue>
#include <set>
#include <vector>
#include <map>
#include "memory_service.hpp"
#include "service_device.h"
#include "dxrt/device.h"
#include "service_error.h"

class SchedulerService;

// Callback interface from SchedulerService to its owner (DxrtService / DxrtServiceV2).
// Implementations must be thread-safe: methods may be called from OutputReceiverThread.
class DXRT_INTERNAL_API ISchedulerListener
{
public:
    virtual ~ISchedulerListener() = default;
    // Called after a successful inference or a deferred error response.
    virtual void onInferenceComplete(const dxrt::dxrt_response_t& response, int deviceId) = 0;
    // Called on device-level errors (broadcasts to all clients on deviceId).
    virtual void onSchedulerError(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId) = 0;
    // Returns true if (pid, deviceId, taskId) is still valid for inference dispatch.
    virtual bool validateTask(pid_t pid, int deviceId, int taskId) = 0;
    // Fired (outside _lock) when the last in-flight entry for (pid, taskId) leaves
    // _map.  Default no-op; V2 overrides for deferred RMAP/Weight memory cleanup.
    virtual void onTaskDrained(pid_t /*pid*/, int /*taskId*/) {}
};

class SchedulerService
{
 public:
    // Request lifecycle state.
    // PENDING  : queued, not yet sent to NPU.
    // RUNNING  : sent to NPU via ioctl, awaiting response.
    // CANCELLED: process disconnected while request was RUNNING; FinishJobs
    //            will silently discard the response when NPU finishes.
    struct RequestEntry {
        dxrt::dxrt_request_acc_t data{};
        enum class State : uint8_t { PENDING, RUNNING, CANCELLED } state = State::PENDING;
        int deviceId = -1;  // set when state transitions to RUNNING
    };

    explicit SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    virtual ~SchedulerService();
    void AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId);
    void FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data);
    void SendError(int deviceId, dxrt::dxrt_server_err_t err, uint32_t errCode) const;


    int Load(int deviceId) const {return _loads[deviceId];}

    // Replaces the old SetCallback / SetErrorCallback / SetTaskValidator.
    // Must be called before any inference is dispatched.
    void SetListener(ISchedulerListener* listener);
    void cleanDiedProcess(int pid);
    void StopScheduler(int procId);
    void StartScheduler(int procId);

    void ClearAllLoad();
    void ClearProcLoad(int procId);

    // Stop inference requests for a specific task
    void StopTaskInference(pid_t pid, int deviceId, int taskId);
    void StopAllInferenceForProcess(pid_t pid, int deviceId);

    int GetRunningRequestCount(pid_t pid, int deviceId);
    // Returns task IDs that have at least one RUNNING entry for this pid.
    // Used by HandleProcessDeInit to identify tasks needing deferred memory free.
    std::vector<int> GetRunningTaskIds(pid_t pid);
    bool IsRequestRunning(pid_t pid, int deviceId, int reqId);
    bool HasPendingRequest(pid_t pid, int reqId);
    void AddRunningRequest(pid_t pid, int deviceId, int reqId);
    void RemoveRunningRequest(pid_t pid, int deviceId, int reqId);
    void ClearRunningRequests(pid_t pid, int deviceId);
    std::vector<int> GetRunningRequestIds(pid_t pid, int deviceId);

 protected:  // NOSONAR
    virtual void schedule(int deviceId) = 0;
    virtual void pushRequest(int deviceId, int procId, int reqId, int taskId) = 0;
    virtual void updateTaskInferenceTime(int procId, int taskId, uint32_t time);
    virtual uint32_t getTaskInferenceTime(int procId, int taskId);
    virtual void cleanTaskInferenceTime(int procId);
    void doInference(int deviceId, int procId, int reqId);

 private:
    std::vector<std::atomic<int> > _loads;
    std::map<int,std::atomic<int>> _loadsProc;
    // Single source of truth: [pid][reqId] -> RequestEntry (state + data).
    // RUNNING entries remain here until FinishJobs removes them, so we always
    // have the task_id available for updateTaskInferenceTime.
    std::map<int, std::map<int, RequestEntry>> _map;
    std::mutex _lock;  // guards _map, _loadsProc, _pendingErrorCallbacks
    std::vector<std::shared_ptr<dxrt::ServiceDevice>> _devices;
    // Listener for inference completion, errors, task validation, and drain events.
    // Owned by the service; must outlive SchedulerService.
    ISchedulerListener* _listener = nullptr;

    // Error responses queued while _lock is held; flushed after unlock.
    std::vector<std::pair<dxrt::dxrt_response_t, int>> _pendingErrorCallbacks;
};

class FIFOSchedulerService : public SchedulerService
{
 public:
    explicit FIFOSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    ~FIFOSchedulerService() override;

 private:
    void schedule(int deviceId) override;
    void pushRequest(int deviceId, int procId, int reqId, int taskId) override;

    std::vector<std::queue<std::pair<int, int> > > _device_queues;

};

class RoundRobinSchedulerService : public SchedulerService
{
 public:
    explicit RoundRobinSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    ~RoundRobinSchedulerService() override;

 private:
    void schedule(int deviceId) override;
    void pushRequest(int deviceId, int procId, int reqId, int taskId) override;

    std::vector<std::map<int, std::queue<int> > > _proc_maps;
    std::vector<int> _next_proc;

};

class InferenceTimeCheckSchedulerService : public SchedulerService
{
 public:
    explicit InferenceTimeCheckSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
 protected:
    void updateTaskInferenceTime(int procId, int taskId, uint32_t time) override;
    uint32_t getTaskInferenceTime(int procId, int taskId) override;
    void cleanTaskInferenceTime(int procId) override;
 private:
    std::map<std::pair<int, int>, uint32_t> task_time_map;
};

class SJFSchedulerService : public InferenceTimeCheckSchedulerService
{
 public:
    explicit SJFSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    ~SJFSchedulerService() override;
    struct request_elem
    {
       int requestId;
       int procId;
       uint32_t time;
    };
 private:

    void schedule(int deviceId) override;
    void pushRequest(int deviceId, int procId, int reqId, int taskId) override;

    std::vector<std::priority_queue<request_elem> > request_map;
    std::multimap<int, std::pair<int, int> > key_less_map;
};
