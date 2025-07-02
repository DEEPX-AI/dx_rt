// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "scheduler_service.h"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "service_error.h"
#include <algorithm>
#include <limits>

#define DX_RT_SERVICE_SCHED_THRE (5)

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
    std::unique_lock<mutex> lk(_lock);
    _map[procId].clear();
    _map.erase(procId);
    _procId.emplace_back(procId);
    cleanTaskInferenceTime(procId);
}

void SchedulerService::StartScheduler(int procId)
{
    std::unique_lock<mutex> lk(_lock);
    _procId.erase(std::remove(_procId.begin(), _procId.end(), procId), _procId.end());
}

int SchedulerService::GetProcLoad(int procId)
{
    std::unique_lock<mutex> lk(_lock);
    auto it = _loadsProc.find(procId);
    if (it != _loadsProc.end()) {
        return it->second.load();
    } else {
        return 0;
    }
}

void SchedulerService::ClearAllLoad()
{
    std::unique_lock<mutex> lk(_lock);
    _loadsProc.clear();
    std::for_each(_loads.begin(), _loads.end(), [](std::atomic<int>& value) {
        value.store(0);
    });
}

void SchedulerService::ClearProcLoad(int procId)
{
    std::unique_lock<mutex> lk(_lock);
    _loadsProc.erase(procId);
}

void SchedulerService::AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId)
{
    std::unique_lock<mutex> lk(_lock);

    int proc_id = packet_data.proc_id;
    int req_id = packet_data.req_id;
    _map[proc_id][req_id] = packet_data;
    if (std::find(_procId.begin(), _procId.end(), proc_id) == _procId.end())
        pushRequest(deviceId, proc_id, req_id, packet_data.task_id);
    _loadsProc[proc_id]++;
    if (_loads[deviceId].load() < DX_RT_SERVICE_SCHED_THRE)
    {
        schedule(deviceId);
    }
    else
    {
        //cout << "SCHEDULER FAULT "<< proc_id << " " << req_id << endl;
    }
}

void SchedulerService::FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data)
{
    dxrt::dxrt_response_t response_to_send = response_data;
    int req_id = response_data.req_id;
    int proc_id = response_data.proc_id;

    {
        std::unique_lock<mutex> lk(_lock);


        // get response_data

        _loadsProc[proc_id]--;
        _loads[deviceId]--;

        LOG_DXRT_S_DBG << deviceId << "," <<proc_id << " 's req " << req_id <<
            ", load: " << _loads[deviceId].load() << ", loadsProc" << _loadsProc[proc_id].load() << endl;

        auto it = _map.find(proc_id);
        if (it == _map.end())
            return;
        int task_id = it->second[req_id].task_id;
        updateTaskInferenceTime(proc_id, task_id, response_data.inf_time);
        it->second.erase(req_id);
        schedule(deviceId);
        if (std::find(_procId.begin(), _procId.end(), proc_id) == _procId.end())
        {
            lk.unlock();
            _callBack(response_to_send, deviceId);  // send result to client
        }
            else
        {
            cout << "SCHEDULER FAULT2 "<< proc_id << " " << req_id << endl;
        }
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

void SchedulerService::cleanDiedProcess(int pid)
{
    _map.erase(pid);
}

void SchedulerService::doInference(int deviceId, int procId, int reqId)
{
    if (_map.find(procId) == _map.end())
    {
        LOG_DXRT_S << "NOTFOUND "<< deviceId << " " << procId << " " << reqId << endl;
        return;
    }
    {
        
        _loads[deviceId]++;
        // do inference

        //if(reqId%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || reqId%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        //{
        //    cout<<"[DoINF      ]              DEVICE : "<<deviceId<<" - PROCESS_ID : "<<procId<<" - REQ_ID : "<<reqId<<" - LOAD : "<<_loads[deviceId]<<endl;//AGING LOG
        //}

        dxrt::dxrt_request_acc_t new_req = _map[procId][reqId];
        int retval = _devices[deviceId]->InferenceRequest(&new_req);
        if ((retval == -EBUSY) || (retval == -EAGAIN))
        {
            _loads[deviceId]--;
            LOG_DXRT_S << "AGAIN retval"<<endl;
            pushRequest(deviceId, procId, reqId, new_req.task_id);
            return;
        }
        if (retval != 0)
        {
            LOG_DXRT_S << "Report error message to client:" << retval << endl;
            _errCallBack(dxrt::dxrt_server_err_t::S_ERR_SCHEDULE_REQ, retval, deviceId);
        }
        DXRT_ASSERT(retval == 0, "IOCTL FAILED err: "+ to_string(retval));
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

FIFOSchedulerService::FIFOSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: SchedulerService(devices_), _device_queues(_devices.size())
{
}
FIFOSchedulerService::~FIFOSchedulerService() = default;
void FIFOSchedulerService::pushRequest(int deviceId, int procId, int reqId, int taskId)
{
    std::ignore = taskId;
    _device_queues[deviceId].push(std::make_pair(procId, reqId));
}

void FIFOSchedulerService::schedule(int deviceId)
{
    if (_device_queues[deviceId].empty())
    {
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
    //find next schedule process
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
        //double new_time = static_cast<double>(task_time_map[key]) * 0.5 + static_cast<double>(time) * 0.5;
        //task_time_map[key] = static_cast<uint32_t>(new_time);
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

using std::make_pair;
SJFSchedulerService::SJFSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
: InferenceTimeCheckSchedulerService(devices_), request_map(devices_.size())
{
}

SJFSchedulerService::~SJFSchedulerService() = default;

void SJFSchedulerService::schedule(int deviceId)
{
    if(request_map[deviceId].empty())
    {
        return;
    }
    else
    {
        SJFSchedulerService::request_elem e = request_map[deviceId].top();
        request_map[deviceId].pop();
        LOG_DXRT << "SJF proc_id " << e.procId << " req_id "<< e.requestId <<", time:"<<e.time<< endl;
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
    //LOG_DXRT << "SJF proc_id " << e.procId << " req_id "<< e.requestId << ", time: " << time << endl;

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

