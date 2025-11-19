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
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "service_error.h"

#define DX_RT_SERVICE_SCHED_THRE (6)

using std::make_pair;
using std::endl;

SchedulerService::SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
:_devices(devices_)
{
    _loads =  std::vector<std::atomic<int>>(_devices.size());
}

SchedulerService::~SchedulerService()
{

}

void SchedulerService::StopScheduler(int procId)
{
    std::unique_lock<std::mutex> lk(_lock);
    _map[procId].clear();
    _map.erase(procId);
    cleanTaskInferenceTime(procId);
}


/*
int SchedulerService::GetProcLoad(int procId)
{
    std::unique_lock<std::mutex> lk(_lock);
    auto it = _loadsProc.find(procId);
    if (it != _loadsProc.end()) {
        return it->second.load();
    } else {
        return 0;
    }
}
*/

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

    _map[proc_id][req_id] = packet_data;
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
}

void SchedulerService::FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data)
{
    dxrt::dxrt_response_t response_to_send = response_data;
    int req_id = response_data.req_id;
    int proc_id = response_data.proc_id;

    {
        std::unique_lock<std::mutex> lk(_lock);

        // Remove from running requests
        RemoveRunningRequest(proc_id, deviceId, req_id);

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
            return;
        }

        // If check_die_thread was called first, _loadsProc would have been initialized to 0, so ignore.
        // This is only meaningful when FinishJobs is called before check_die_thread (when >0).

        if(_loadsProc.count(proc_id) && _loadsProc[proc_id] > 0){
            _loadsProc[proc_id]--;
        } else{
            LOG_DXRT_DBG << "_loadsProc[" << proc_id << "] is zero or not found, cannot decrement." << endl;
        }


        int task_id = it->second[req_id].task_id;

        updateTaskInferenceTime(proc_id, task_id, response_data.inf_time);
        it->second.erase(req_id);

        schedule(deviceId);

        lk.unlock();
        _callBack(response_to_send, deviceId);  // send result to client
        LOG_DXRT_S_DBG << "At FinishJobs end - After _callBack end's successful"<<endl;

    }
}

void SchedulerService::SetCallback(std::function<void(const dxrt::dxrt_response_t&, int)> f)
{
    _callBack = f;
}

void SchedulerService::SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> f)
{
    _errCallBack = f;
}

void SchedulerService::SetTaskValidator(std::function<bool(pid_t, int, int)> validator)
{
    _taskValidator = validator;
}

void SchedulerService::StopTaskInference(pid_t pid, int deviceId, int taskId)
{
    std::unique_lock<std::mutex> lk(_lock);
    LOG_DXRT_S_DBG << "Stopping inference for PID " << pid << ", Device " << deviceId << ", Task " << taskId << endl;

    // Remove all pending inference requests for this specific task
    auto procIt = _map.find(pid);
    if (procIt != _map.end()) {
        auto& requests = procIt->second;
        std::vector<int> requestsToRemove;


        for (const auto& reqPair : requests) {
            int reqId = reqPair.first;
            const auto& reqData = reqPair.second;

             // Remove only if request is not currently running
            if (reqData.task_id == static_cast<uint32_t>(taskId) && !IsRequestRunning(pid, deviceId, reqId)) {
                requestsToRemove.push_back(reqId);
                LOG_DXRT_S_DBG << "Stopping inference request " << reqId
                               << " for Task " << taskId << ", PID " << pid
                               << " on device " << deviceId << endl;
            }
            else {
                LOG_DXRT_S_ERR("Task id mismatch in StopTaskInference");
            }
        }

        // Remove non-running requests
        for (int reqId : requestsToRemove) {
            requests.erase(reqId);
            if(_loadsProc.count(pid) && _loadsProc[pid] > 0){
                _loadsProc[pid]--;
                LOG_DXRT_S_DBG << "Decrease loadsProc in StopTaskInference - Process: "<< pid<<"LoadsProc: "<<_loadsProc[pid]<<"RequestId: "<<reqId<<endl;
            }

            else if (_loadsProc.count(pid) && _loadsProc[pid] == 0){
                LOG_DXRT_S_DBG << "Cannot Decrease loadsProc in StopTaskInference - LoadsProc is already zero."<<   endl;
            }
            else {
                LOG_DXRT_S_DBG << "[StopTaskInference] _loadsProc[" << pid << "] cannot not found, cannot decrement." << endl;
            }

            if(_loads[deviceId] > 0){
                _loads[deviceId]--;
                LOG_DXRT_S_DBG << "Load Decrease in StopTaskInference - Process: "<< pid<<"LoadProc: " <<_loadsProc[pid];
            }

            else {
                LOG_DXRT_S_DBG << "[StopTaskInference] _loads[" << deviceId << "] is zero or not found, cannot decrement." << endl;
            }
        }

        LOG_DXRT_S_DBG << "Stopped " << requestsToRemove.size() << " inference requests for Task "
                   << taskId << ", PID " << pid << " on device " << deviceId << endl;
    }
}

void SchedulerService::StopAllInferenceForProcess(pid_t pid, int deviceId)
{
    std::unique_lock<std::mutex> lk(_lock);

    LOG_DXRT_S_DBG << "Stopping all inference for PID " << pid << ", Device " << deviceId << endl;

    // Count requests before removal
    size_t beforeCount = 0;
    for (auto& procPair : _map) {
        beforeCount += procPair.second.size();
    }

    // Remove all pending requests for this process
    for (auto it = _map.begin(); it != _map.end(); ) {
        if (it->first == pid) {
            for (auto reqIt = it->second.begin(); reqIt != it->second.end(); ) {
                const auto& reqData = reqIt->second;
                if (reqData.task_id == 0) {  // Assuming task_id 0 means it's a process-level request
                    reqIt = it->second.erase(reqIt);

                    if(_loadsProc.count(pid) &&  _loadsProc[pid] > 0){
                        _loadsProc[pid]--;
                    } else {
                        LOG_DXRT_S_DBG << "[StopAllInferenceForProcess] _loadsProc[" << pid << "] is zero or not found, cannot decrement." << endl;
                    }

                    if(_loads[deviceId] > 0){
                        _loads[deviceId]--;
                    } else {
                        LOG_DXRT_S_DBG << "[StopAllInferenceForProcess] _loads[" << deviceId << "] is zero or not found, cannot decrement." << endl;
                    }

                } else {
                    reqIt++;
                }
            }
            if (it->second.empty()) {
                it = _map.erase(it);
            } else {
                it++;
            }
        } else {
            it++;
        }
    }

    size_t afterCount = 0;
    for (auto& procPair : _map) {
        afterCount += procPair.second.size();
    }
    size_t removedCount = beforeCount - afterCount;

    LOG_DXRT_S_DBG << "Removed " << removedCount << " pending inference requests for process " << pid << endl;
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

    dxrt::dxrt_request_acc_t new_req = _map[procId][reqId];

    // Task validity verification
    // This could cause DeadLock if not handled carefully
    if (_taskValidator && !_taskValidator(procId, deviceId, new_req.task_id)) {
        LOG_DXRT_S_ERR("Task " + std::to_string(new_req.task_id) +
                       " is not valid for process " + std::to_string(procId) +
                       " on device " + std::to_string(deviceId) +
                       " (request " + std::to_string(reqId) + ")");

        // Send error response to client for invalid task
        dxrt::dxrt_response_t error_resp{};
        error_resp.req_id = reqId;
        error_resp.proc_id = procId;
        error_resp.status = -1;  // Error status
        _callBack(error_resp, deviceId);

        // Invalid task requests are removed from the queue and load counter is decreased
        {
            std::unique_lock<std::mutex> lk(_lock);
            if(_loadsProc.count(procId) &&  _loadsProc[procId] > 0){
                // std::cout<<"Process :"<< procId<<"Load Decrease in Invalid Task"<<std::endl;
                _loadsProc[procId]--;

            }   else {
                LOG_DXRT_S_DBG << "[doInference] _loadsProc[" << procId << "] is zero or not found, cannot decrement." << endl;
            }

            if(_loads[deviceId] > 0){
                _loads[deviceId]--;
            } else {
                LOG_DXRT_S_DBG << "[doInference] _loads[" << deviceId << "] is zero or not found, cannot decrement." << endl;
            }

            auto it = _map.find(procId);
            if (it != _map.end()) {
                it->second.erase(reqId);
            }
        }
        return;
    }

    // Check if device is blocked before sending inference request
    if (_devices[deviceId]->isBlocked()) {
        LOG_DXRT_S_ERR("Device " + std::to_string(deviceId) + " is blocked, cannot process inference request");

        // Send error response to client
        dxrt::dxrt_response_t error_resp{};
        error_resp.req_id = reqId;
        error_resp.proc_id = procId;
        error_resp.status = -2;  // Device blocked error
        _callBack(error_resp, deviceId);

        // Remove from queue and decrease load
        {
            std::unique_lock<std::mutex> lk(_lock);
            if(_loadsProc.count(procId) && _loadsProc[procId] > 0){
                _loadsProc[procId]--;
            }   else {
                LOG_DXRT_S_DBG << "[doInference] _loadsProc[" << procId << "] is zero or not found, cannot decrement." << endl;
            }

            if(_loads[deviceId] > 0){
                _loads[deviceId]--;
            } else {
                LOG_DXRT_S_DBG << "[doInference] _loads[" << deviceId << "] is zero or not found, cannot decrement." << endl;
            }

            auto it = _map.find(procId);
            if (it != _map.end()) {
                it->second.erase(reqId);
            }
        }
        return;
    }

    {
         AddRunningRequest(procId, deviceId, reqId);
        _loads[deviceId]++;

        //Scheduling debug log
        LOG_DXRT_S_DBG << "Do Inference - InferenceRequest start" << deviceId << " - PROCESS_ID : " << procId << " - REQ_ID : " << reqId << " - Device LOAD : " << _loads[deviceId].load() << std::endl;
        // do inference

        // if(reqId%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || reqId%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        // {
        //    std::cout<<"[DoINF      ]              DEVICE : "<<deviceId<<" - PROCESS_ID : "<<procId<<" - REQ_ID : "<<reqId<<" - LOAD : "<<_loads[deviceId]<<endl;//AGING LOG
        //}
        int retval = _devices[deviceId]->InferenceRequest(&new_req);
        LOG_DXRT_S_DBG << "Do Inference - InferenceRequest end"<<deviceId<<" - PROCESS_ID : "<<procId<<" -Bound: " << new_req.bound << " - REQ_ID : "<<reqId<<" - Device LOAD : "<<_loads[deviceId].load()<<std::endl;//AGING LOG

        if ((retval == -EBUSY) || (retval == -EAGAIN))
        {
            _loads[deviceId]--;
            RemoveRunningRequest(procId, deviceId, reqId);
            LOG_DXRT_S << "AGAIN retval" << endl;
            pushRequest(deviceId, procId, reqId, new_req.task_id);
            return;
        }

        // No empty queue in list(-2) case
        if (retval != 0)
        {
            LOG_DXRT_S << "Report error message to client:" << retval << endl;
            _errCallBack(dxrt::dxrt_server_err_t::S_ERR_SCHEDULE_REQ, retval, deviceId);
        }
        DXRT_ASSERT(retval == 0, "IOCTL FAILED err: "+ std::to_string(retval));
    }
}

void SchedulerService::SendError(int deviceId, dxrt::dxrt_server_err_t err, uint32_t errCode)
{
    LOG_DXRT_S << "Report error message to client:" << errCode << endl;
    _errCallBack(err, errCode, deviceId);
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
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);
    return _runningRequests[key].size();
}

bool SchedulerService::IsRequestRunning(pid_t pid, int deviceId, int reqId)
{
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);
    return _runningRequests[key].count(reqId) > 0;
}

void SchedulerService::AddRunningRequest(pid_t pid, int deviceId, int reqId)
{
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);
    _runningRequests[key].insert(reqId);

    LOG_DXRT_S_DBG << "Added running request: PID " << pid
                   << ", Device " << deviceId
                   << ", Request " << reqId
                   << " (total: " << _runningRequests[key].size() << ")" << endl;
}

void SchedulerService::RemoveRunningRequest(pid_t pid, int deviceId, int reqId)
{
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);

    auto& requestSet = _runningRequests[key];
    auto it = requestSet.find(reqId);
    if (it != requestSet.end()) {
        requestSet.erase(it);
        LOG_DXRT_S_DBG << "Removed running request: PID " << pid
                       << ", Device " << deviceId
                       << ", Request " << reqId
                       << " (remaining: " << requestSet.size() << ")" << endl;
    }
}

void SchedulerService::ClearRunningRequests(pid_t pid, int deviceId)
{
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);

    auto it = _runningRequests.find(key);
    if (it != _runningRequests.end()) {
        LOG_DXRT_S_DBG << "Force clearing " << it->second.size()
                   << " running requests for PID " << pid
                   << ", Device " << deviceId << endl;

        _runningRequests.erase(it);
    }
}

std::vector<int> SchedulerService::GetRunningRequestIds(pid_t pid, int deviceId)
{
    std::lock_guard<std::mutex> lock(_runningRequestsMutex);
    auto key = std::make_pair(pid, deviceId);

    std::vector<int> result;
    auto it = _runningRequests.find(key);
    if (it != _runningRequests.end()) {
        const std::set<int>& requestSet = it->second;
        result.reserve(requestSet.size());

        for (int reqId : requestSet) {
            result.push_back(reqId);
        }
    }

    return result;
}

FIFOSchedulerService::FIFOSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: SchedulerService(devices_), _device_queues(_devices.size())
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
        // double new_time = static_cast<double>(task_time_map[key]) * 0.5 + static_cast<double>(time) * 0.5;
        // task_time_map[key] = static_cast<uint32_t>(new_time);
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
    // LOG_DXRT << "SJF proc_id " << e.procId << " req_id "<< e.requestId << ", time: " << time << endl;

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

