/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include <algorithm>
#include <limits>
#include <memory>
#include <iostream>
#include "scheduler_service.h"
#include "service_error.h"

#define DX_RT_SERVICE_SCHED_THRE (6)

using std::make_pair;
using std::endl;

SchedulerService::SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
:_devices(devices_)
{
    _loads =  std::vector<std::atomic<int>>(_devices.size());
}

SchedulerService::~SchedulerService() = default;

void SchedulerService::StopScheduler(int procId)
{
    std::unique_lock<std::mutex> lk(_lock);
    _map[procId].clear();
    _map.erase(procId);
    cleanTaskInferenceTime(procId);
}



void SchedulerService::ClearAllLoad()
{
    std::unique_lock<std::mutex> lk(_lock);
    _loadsProc.clear();
    std::for_each(_loads.begin(), _loads.end(), [](std::atomic<int>& value) {
        value.store(0);
    });
}

void SchedulerService::ClearProcLoad(int procId)
{
    std::unique_lock<std::mutex> lk(_lock);
    _loadsProc.erase(procId);
}

void SchedulerService::AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId)
{
    std::unique_lock<std::mutex> lk(_lock);

    int proc_id = packet_data.proc_id;
    int req_id = packet_data.req_id;

    _map[proc_id][req_id] = RequestEntry{packet_data};
    _loadsProc[proc_id]++;

    LOG_DXRT_S_DBG << "[AddScheduler] PID: " << proc_id
               << ", Device: " << deviceId
               << ", Task: " << packet_data.task_id
               << ", Bound from request: " << packet_data.bound << endl;

    pushRequest(deviceId, proc_id, req_id, packet_data.task_id);

    LOG_DXRT_S_DBG << "Load Increase in Add Scheduler - Process: " <<  proc_id
      << " Load Proc: " << _loadsProc[proc_id] << " Request Id: " << req_id << endl;
    if (_loads[deviceId].load() < DX_RT_SERVICE_SCHED_THRE)
    {
        schedule(deviceId);
    }
    else
    {
        LOG_DXRT_S_DBG << "AddScheduler: maximum load reached for device "
          << deviceId << " - Process: "<< proc_id << " Request Id: " << req_id
          << "(current load: " << _loads[deviceId].load() << ", max load:" << DX_RT_SERVICE_SCHED_THRE << ")" <<endl;
    }

    // Fire error callbacks that doInference deferred while _lock was held.
    auto deferred = std::move(_pendingErrorCallbacks);
    lk.unlock();
    for (auto& cb : deferred)
    {
        if (_listener) _listener->onInferenceComplete(cb.first, cb.second);
    }
}

void SchedulerService::FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data)
{
    dxrt::dxrt_response_t response_to_send = response_data;
    int req_id = response_data.req_id;
    int proc_id = response_data.proc_id;

    {
        std::unique_lock<std::mutex> lk(_lock);

        // get response_data
        LOG_DXRT_S_DBG<< deviceId << "," <<proc_id << " 's req " << req_id <<
            ", load: " << _loads[deviceId].load() << ", loadsProc" << _loadsProc[proc_id].load() << "DMA Channel: "<< response_data.dma_ch << endl;

        // Device's Loads are always decremented in FinishJobs to maintain consistency -> even for already terminated processes.
        if(_loads[deviceId] > 0){
            _loads[deviceId]--;
        } else {
            LOG_DXRT_DBG << "_loads[" << deviceId << "] is zero, cannot decrement." << endl;
        }

        auto it = _map.find(proc_id);
        if (it == _map.end()) {
            LOG_DXRT_S_DBG << "Cannot Find processId in _map";
            schedule(deviceId);
            return;
        }

        // If check_die_thread was called first, _loadsProc would have been initialized to 0, so ignore.
        // This is only meaningful when FinishJobs is called before check_die_thread (when >0).

        if(_loadsProc.count(proc_id) && _loadsProc[proc_id] > 0){
            _loadsProc[proc_id]--;
        } else{
            LOG_DXRT_DBG << "_loadsProc[" << proc_id << "] is zero or not found, cannot decrement." << endl;
        }


        const auto reqIt = it->second.find(req_id);
        if (reqIt == it->second.end()) {
            LOG_DXRT_S_ERR ( "FinishJobs: missing req_id in pending map. pid=" << proc_id
                           << ", req_id=" << req_id
                           << ", deviceId=" << deviceId << endl);
            schedule(deviceId);
            return;
        }

        // If the process disconnected while this request was in-flight, the entry
        // was marked CANCELLED. Silently discard the result.
        const bool wasCancelled = (reqIt->second.state == RequestEntry::State::CANCELLED);

        int task_id = reqIt->second.data.task_id;
        updateTaskInferenceTime(proc_id, task_id, response_data.inf_time);
        it->second.erase(req_id);

        // Check whether (proc_id, task_id) is fully drained — no entries of any
        // state remain for that task.  Must be evaluated while _lock is still held.
        bool taskDrained = false;
        if (_listener)
        {
            const auto checkIt = _map.find(proc_id);
            taskDrained = (checkIt == _map.end()) ||
                std::none_of(checkIt->second.begin(), checkIt->second.end(),
                    [task_id](const auto& kv) {
                        return kv.second.data.task_id == static_cast<uint32_t>(task_id);
                    });
        }

        schedule(deviceId);

        // Drain error callbacks that doInference deferred while _lock was held.
        auto deferred = std::move(_pendingErrorCallbacks);

        lk.unlock();

        for (auto& cb : deferred)
        {
            if (_listener) _listener->onInferenceComplete(cb.first, cb.second);
        }

        // Notify drain listener (outside lock; service side checks _pendingTaskFree).
        if (taskDrained && _listener)
        {
            _listener->onTaskDrained(static_cast<pid_t>(proc_id), task_id);
        }

        if (!wasCancelled)
        {
            if (_listener) _listener->onInferenceComplete(response_to_send, deviceId);  // send result to client
            LOG_DXRT_S_DBG << "At FinishJobs end - After _callBack end's successful"<<endl;
        }

    }
}

void SchedulerService::SetListener(ISchedulerListener* listener)
{
    _listener = listener;
}

void SchedulerService::StopTaskInference(pid_t pid, int deviceId, int taskId)
{
    std::unique_lock<std::mutex> lk(_lock);
    LOG_DXRT_S_DBG << "Stopping inference for PID " << pid << ", Device " << deviceId << ", Task " << taskId << endl;

    auto procIt = _map.find(pid);
    if (procIt == _map.end()) { return; }

    auto& requests = procIt->second;
    std::vector<int> toErase;

    for (auto& kv : requests)
    {
        auto& reqId = kv.first;
        auto& entry = kv.second;
        if (entry.data.task_id != static_cast<uint32_t>(taskId)) { continue; }

        if (entry.state == RequestEntry::State::RUNNING)
        {
            // Already sent to NPU; mark CANCELLED so FinishJobs suppresses the callback.
            LOG_DXRT_S_DBG << "[StopTaskInference] CANCEL running req " << reqId
                           << " (pid=" << pid << ", task=" << taskId << ")" << endl;
            entry.state = RequestEntry::State::CANCELLED;
            // _loads[deviceId] will be decremented by FinishJobs when NPU responds.
            // _loadsProc[pid] will be decremented by FinishJobs as well.
        }
        else  // PENDING or already CANCELLED
        {
            toErase.push_back(reqId);
            if (_loadsProc.count(pid) && _loadsProc[pid] > 0)
            {
                _loadsProc[pid]--;
            }
            // Do NOT touch _loads[deviceId]: PENDING requests were never counted there.
        }
    }

    for (int reqId : toErase)
    {
        requests.erase(reqId);
    }

    LOG_DXRT_S_DBG << "StopTaskInference done: erased " << toErase.size()
                   << " pending for pid=" << pid << " task=" << taskId << endl;
}

void SchedulerService::StopAllInferenceForProcess(pid_t pid, int deviceId)
{
    std::unique_lock<std::mutex> lk(_lock);

    LOG_DXRT_S_DBG << "Stopping all inference for PID " << pid << ", Device " << deviceId << endl;

    auto it = _map.find(pid);
    if (it == _map.end()) { return; }

    auto& requests = it->second;
    std::vector<int> toErase;
    size_t cancelledCount = 0;

    for (auto& kv : requests)
    {
        auto& entry = kv.second;
        if (entry.state == RequestEntry::State::RUNNING)
        {
            // In-flight on the NPU: mark CANCELLED so FinishJobs suppresses callback.
            // _loads[deviceId] and _loadsProc[pid] will be decremented by FinishJobs.
            entry.state = RequestEntry::State::CANCELLED;
            ++cancelledCount;
        }
        else  // PENDING (or already CANCELLED from a prior StopTaskInference call)
        {
            toErase.push_back(kv.first);
            if (_loadsProc.count(pid) && _loadsProc[pid] > 0)
            {
                _loadsProc[pid]--;
            }
            // PENDING requests were never counted in _loads[deviceId]; don't decrement.
        }
    }

    for (int reqId : toErase)
    {
        requests.erase(reqId);
    }

    // If all remaining entries are CANCELLED (in-flight), keep the pid entry so
    // FinishJobs can still look up task_id for updateTaskInferenceTime.
    if (requests.empty())
    {
        _map.erase(it);
    }

    LOG_DXRT_S_DBG << "StopAllInferenceForProcess done: erased " << toErase.size()
                   << " pending, cancelled " << cancelledCount
                   << " running for pid=" << pid << endl;
}

void SchedulerService::cleanDiedProcess(int pid)
{
    std::unique_lock<std::mutex> lk(_lock);
    _map.erase(pid);
}

void SchedulerService::doInference(int deviceId, int procId, int reqId)
{
    if (_map.find(procId) == _map.end())
    {
        LOG_DXRT_S_DBG << "NOTFOUND "<< deviceId << " " << procId << " " << reqId << endl;
        schedule(deviceId);
        return;
    }

    auto procIt = _map.find(procId);
    auto reqIt = procIt->second.find(reqId);
    if (reqIt == procIt->second.end())
    {
        LOG_DXRT_S_ERR( "doInference: missing req_id in pending map. pid=" << procId
                       << ", req_id=" << reqId
                       << ", deviceId=" << deviceId << endl);
        schedule(deviceId);
        return;
    }

    dxrt::dxrt_request_acc_t new_req = reqIt->second.data;

    // Task validity verification — already holding _lock; no inner lock needed.
    if (_listener && !_listener->validateTask(procId, deviceId, new_req.task_id))
    {
        LOG_DXRT_S_ERR("Task " + std::to_string(new_req.task_id) +
                       " is not valid for process " + std::to_string(procId) +
                       " on device " + std::to_string(deviceId) +
                       " (request " + std::to_string(reqId) + ")");

        // Clean up under _lock; defer callback to after the lock is released.
        if (_loadsProc.count(procId) && _loadsProc[procId] > 0) { _loadsProc[procId]--; }
        procIt->second.erase(reqIt);

        dxrt::dxrt_response_t error_resp{};
        error_resp.req_id  = reqId;
        error_resp.proc_id = procId;
        error_resp.status  = -1;
        _pendingErrorCallbacks.emplace_back(error_resp, deviceId);

        schedule(deviceId);
        return;
    }

    // Check if device is blocked before sending inference request.
    if (_devices[deviceId]->isBlocked())
    {
        LOG_DXRT_S_ERR("Device " + std::to_string(deviceId) + " is blocked, cannot process inference request");

        if (_loadsProc.count(procId) && _loadsProc[procId] > 0) { _loadsProc[procId]--; }
        procIt->second.erase(reqIt);

        dxrt::dxrt_response_t error_resp{};
        error_resp.req_id  = reqId;
        error_resp.proc_id = procId;
        error_resp.status  = -2;
        _pendingErrorCallbacks.emplace_back(error_resp, deviceId);

        schedule(deviceId);
        return;
    }

    {
        // Transition PENDING → RUNNING.
        reqIt->second.state    = RequestEntry::State::RUNNING;
        reqIt->second.deviceId = deviceId;
        _loads[deviceId]++;

        LOG_DXRT_S_DBG << "Do Inference - InferenceRequest start" << deviceId << " - PROCESS_ID : " << procId << " - REQ_ID : " << reqId << " - Device LOAD : " << _loads[deviceId].load() << std::endl;

        int retval = _devices[deviceId]->InferenceRequest(&new_req);
        LOG_DXRT_S_DBG << "Do Inference - InferenceRequest end" << deviceId
                   << " - PROCESS_ID : " << procId
                   << " -Bound: " << new_req.bound
                   << " - REQ_ID : " << reqId
                   << " - dma_ch : " << new_req.dma_ch
                   << " - queue : " << new_req.queue
                   << " - retval : " << retval
                   << " - Device LOAD : " << _loads[deviceId].load() << std::endl;

        if ((retval == -EBUSY) || (retval == -EAGAIN))
        {
            _loads[deviceId]--;
            // Revert to PENDING so StopTaskInference / StopAllInference can handle it.
            reqIt->second.state    = RequestEntry::State::PENDING;
            reqIt->second.deviceId = -1;
            LOG_DXRT_S << "AGAIN retval" << endl;
            pushRequest(deviceId, procId, reqId, new_req.task_id);
            return;
        }

        // No empty queue in list(-2) case
        if (retval != 0)
        {
            LOG_DXRT_S << "Report error message to client:" << retval << endl;
            if (_listener) _listener->onSchedulerError(dxrt::dxrt_server_err_t::S_ERR_SCHEDULE_REQ, retval, deviceId);
        }
        DXRT_ASSERT(retval == 0, "IOCTL FAILED err: "+ std::to_string(retval));
    }
}

void SchedulerService::SendError(int deviceId, dxrt::dxrt_server_err_t err, uint32_t errCode) const
{
    LOG_DXRT_S << "Report error message to client:" << errCode << endl;
    if (_listener) _listener->onSchedulerError(err, errCode, deviceId);
}

void SchedulerService::updateTaskInferenceTime(int procId, int taskId, uint32_t time)
{
    std::ignore = procId;
    std::ignore = taskId;
    std::ignore = time;
}
uint32_t SchedulerService::getTaskInferenceTime(int procId, int taskId)
{
    std::ignore = procId;
    std::ignore = taskId;
    return 0;
}
void SchedulerService::cleanTaskInferenceTime(int procId)
{
    std::ignore = procId;
}

int SchedulerService::GetRunningRequestCount(pid_t pid, int deviceId)
{
    std::lock_guard<std::mutex> lock(_lock);
    auto it = _map.find(static_cast<int>(pid));
    if (it == _map.end()) { return 0; }
    int count = 0;
    for (const auto& kv : it->second)
    {
        const auto& entry = kv.second;
        if (entry.state == RequestEntry::State::RUNNING && entry.deviceId == deviceId)
        {
            ++count;
        }
    }
    return count;
}

std::vector<int> SchedulerService::GetRunningTaskIds(pid_t pid)
{
    std::lock_guard<std::mutex> lock(_lock);
    auto it = _map.find(static_cast<int>(pid));
    if (it == _map.end()) { return {}; }
    std::set<int> taskIds;
    for (const auto& kv : it->second)
    {
        const auto& entry = kv.second;
        if (entry.state == RequestEntry::State::RUNNING)
        {
            taskIds.insert(static_cast<int>(entry.data.task_id));
        }
    }
    return {taskIds.begin(), taskIds.end()};
}

bool SchedulerService::IsRequestRunning(pid_t pid, int deviceId, int reqId)
{
    // NOTE: callers inside the scheduler already hold _lock; callers outside must
    // acquire it themselves. This method is kept for backward-compat but internal
    // callers should access _map directly while the lock is held.
    auto procIt = _map.find(pid);
    if (procIt == _map.end()) { return false; }
    auto reqIt = procIt->second.find(reqId);
    if (reqIt == procIt->second.end()) { return false; }
    return reqIt->second.state == RequestEntry::State::RUNNING
        && reqIt->second.deviceId == deviceId;
}

bool SchedulerService::HasPendingRequest(pid_t pid, int reqId)
{
    std::lock_guard<std::mutex> lock(_lock);
    const auto procIt = _map.find(pid);
    if (procIt == _map.end())
    {
        return false;
    }

    return procIt->second.find(reqId) != procIt->second.end();
}

void SchedulerService::AddRunningRequest(pid_t pid, int deviceId, int reqId)
{
    // Kept for API compatibility; state is now managed directly in doInference.
    std::ignore = pid;
    std::ignore = deviceId;
    std::ignore = reqId;
}

void SchedulerService::RemoveRunningRequest(pid_t pid, int deviceId, int reqId)
{
    // Kept for API compatibility; state is now managed directly in doInference/FinishJobs.
    std::ignore = pid;
    std::ignore = deviceId;
    std::ignore = reqId;
}

void SchedulerService::ClearRunningRequests(pid_t pid, int deviceId)
{
    std::lock_guard<std::mutex> lock(_lock);
    auto it = _map.find(pid);
    if (it == _map.end()) { return; }
    for (auto& pair : it->second)
    {
        auto& entry = pair.second;
        if (entry.state == RequestEntry::State::RUNNING && entry.deviceId == deviceId)
        {
            entry.state = RequestEntry::State::CANCELLED;
        }
    }
}

std::vector<int> SchedulerService::GetRunningRequestIds(pid_t pid, int deviceId)
{
    std::lock_guard<std::mutex> lock(_lock);
    std::vector<int> result;
    auto it = _map.find(pid);
    if (it == _map.end()) { return result; }
    for (const auto& pair : it->second)
    {
        const auto& entry = pair.second;
        if (entry.state == RequestEntry::State::RUNNING && entry.deviceId == deviceId)
        {
            result.push_back(pair.first);
        }
    }
    return result;
}

FIFOSchedulerService::FIFOSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: SchedulerService(devices_), _device_queues(devices_.size())
{
}
FIFOSchedulerService::~FIFOSchedulerService() = default;
void FIFOSchedulerService::pushRequest(int deviceId, int procId, int reqId, int taskId)
{
    std::ignore = taskId;
    _device_queues[deviceId].push(std::make_pair(procId, reqId));
    LOG_DXRT_S_DBG << "[Device " << deviceId << "] Push Done. Current Queue size: " << _device_queues[deviceId].size() << std::endl;
}

void FIFOSchedulerService::schedule(int deviceId)
{
    if (_device_queues[deviceId].empty())
    {
        LOG_DXRT_S_DBG << "_device_queue is empty.So nothing to Schedule.";
        return;
    }
    else
    {
        auto proc_req_id = _device_queues[deviceId].front();
        int proc_id = proc_req_id.first;
        int req_id = proc_req_id.second;
        _device_queues[deviceId].pop();
        doInference(deviceId, proc_id, req_id);
    }
}

RoundRobinSchedulerService::RoundRobinSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: SchedulerService(devices_), _proc_maps(devices_.size()), _next_proc(devices_.size())
{
}
RoundRobinSchedulerService::~RoundRobinSchedulerService() = default;

void RoundRobinSchedulerService::schedule(int deviceId)
{
    // find next schedule process
    if (_proc_maps[deviceId].empty())
    {
        return;
    }
    int current_proc = _next_proc[deviceId];
    if (current_proc == 0)
    {
        current_proc = _proc_maps[deviceId].begin()->first;
        _next_proc[deviceId] = current_proc;
    }
    auto it = _proc_maps[deviceId].find(current_proc);
    if (it == _proc_maps[deviceId].end())
    {
        it = _proc_maps[deviceId].begin();
    }
    if (it->second.empty())
    {
        return;
    }
    int req_id = it->second.front();
    it->second.pop();
    // calc next proc
    if (it->second.empty())
    {
        it = _proc_maps[deviceId].erase(it);
    }
    else
    {
        it++;
    }
    if (it == _proc_maps[deviceId].end())
    {
        it = _proc_maps[deviceId].begin();
    }
    if (_proc_maps[deviceId].empty())
    {
        _next_proc[deviceId] = 0;
    }
    else
    {

        _next_proc[deviceId] = it->first;
    }

    int proc_id = current_proc;


    LOG_DXRT_DBG << "Rount_robin proc_id " << proc_id << " req_id "<< req_id << endl;
    doInference(deviceId, proc_id, req_id);
}
void RoundRobinSchedulerService::pushRequest(int deviceId, int procId, int reqId, int taskId)
{
    std::ignore = taskId;
    _proc_maps[deviceId][procId].push(reqId);
}

InferenceTimeCheckSchedulerService::InferenceTimeCheckSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
  : SchedulerService(devices_)
{
}
void InferenceTimeCheckSchedulerService::updateTaskInferenceTime(int procId, int taskId, uint32_t time)
{
    auto key = std::make_pair(procId, taskId);
    if (task_time_map[key] == 0)
    {
        task_time_map[key] = time;
    }
    else
    {
        return;
    }
}
uint32_t InferenceTimeCheckSchedulerService::getTaskInferenceTime(int procId, int taskId)
{
    auto key = std::make_pair(procId, taskId);
    auto it = task_time_map.find(key);
    if (it == task_time_map.end())
    {
        return 0;
    }
    else return it->second;
}
void InferenceTimeCheckSchedulerService::cleanTaskInferenceTime(int procId)
{
    for (auto it = task_time_map.begin(); it != task_time_map.end(); )
    {
        if (it->first.first == procId)
        {
            it = task_time_map.erase(it);
        }
        else
        {
            it++;
        }
    }
}


SJFSchedulerService::SJFSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: InferenceTimeCheckSchedulerService(devices_), request_map(devices_.size())
{
}

SJFSchedulerService::~SJFSchedulerService() = default;

void SJFSchedulerService::schedule(int deviceId)
{
    if (request_map[deviceId].empty())
    {
        return;
    }
    else
    {
        SJFSchedulerService::request_elem e = request_map[deviceId].top();
        request_map[deviceId].pop();
        LOG_DXRT_DBG << "SJF proc_id " << e.procId << " req_id "<< e.requestId <<", time:" << e.time<< endl;
        doInference(deviceId, e.procId, e.requestId);
    }
}

void SJFSchedulerService::pushRequest(int deviceId, int procId, int reqId, int taskId)
{
    uint32_t time = getTaskInferenceTime(procId, taskId);
    SJFSchedulerService::request_elem e;
    e.time = time;
    e.procId = procId;
    e.requestId = reqId;

    request_map[deviceId].push(e);
}

bool operator<(const SJFSchedulerService::request_elem& a, const SJFSchedulerService::request_elem& b)
{
    if (a.time == b.time)
    {
        return a.requestId > b.requestId;
    }
    return a.time > b.time;
}

