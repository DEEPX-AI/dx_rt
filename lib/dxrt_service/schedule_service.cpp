// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "scheduler_service.h"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include "service_error.h"

#define DX_RT_SERVICE_SCHED_THRE (10)

SchedulerService::SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_)
:_devices(devices_)
{
    _loads =  std::vector<std::atomic<int>>(_devices.size());
    _device_queues = std::vector<std::queue<std::pair<int, int> > >(_devices.size());

}

SchedulerService::~SchedulerService()
{

}

void SchedulerService::StopScheduler(int procId)
{
    std::unique_lock<mutex> lk(_lock);
    _map[procId].clear();
    _map.erase(procId);
    _stop = true;
}

void SchedulerService::StartScheduler()
{
    std::unique_lock<mutex> lk(_lock);
    _stop = false;
}

void SchedulerService::AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId)
{
    std::unique_lock<mutex> lk(_lock);

    int proc_id = packet_data.proc_id;
    int req_id = packet_data.req_id;
    _map[proc_id][req_id] = packet_data;
    //_queue.push(std::make_pair(proc_id, req_id));
    _device_queues[deviceId].push(std::make_pair(proc_id, req_id));

    if (_loads[deviceId] < DX_RT_SERVICE_SCHED_THRE)
        schedule(deviceId);
}

void SchedulerService::schedule(int deviceId)
{
    if (_device_queues[deviceId].empty())
    {
        return;
    }
    auto proc_req_id = _device_queues[deviceId].front();
    int proc_id = proc_req_id.first;
    int req_id = proc_req_id.second;
    _device_queues[deviceId].pop();
    if (_map.find(proc_id) == _map.end())
    {
        return;
    }
    if (!_stop)
    {
        _loads[deviceId]++;
        // do inference
        dxrt::dxrt_request_acc_t new_req = _map[proc_id][req_id];
        int retval = _devices[deviceId]->InferenceRequest(&new_req);
        if (retval == -EBUSY)
        {
            _loads[deviceId]--;
            _device_queues[deviceId].push(std::make_pair(proc_id, req_id));
            return;
        }
        if (retval != 0)
        {
            LOG_DXRT_S << "Report error message to client:" << retval << endl;
            _errCallBack(dxrt::dxrt_server_err_t::S_ERR_SCHEDULE_REQ, retval);
        }
        DXRT_ASSERT(retval == 0, "IOCTL FAILED err: "+ to_string(retval));
    }
}
void SchedulerService::FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data)
{
    std::unique_lock<mutex> lk(_lock);

    _loads[deviceId]--;
    // get response_data
    dxrt::dxrt_response_t response_to_send = response_data;
    int req_id = response_data.req_id;
    int proc_id = response_data.proc_id;


    LOG_DXRT_S_DBG << deviceId << "," <<proc_id << " 's req " << req_id <<", load: " << _loads[deviceId] << endl;

    auto it = _map.find(proc_id);
    if (it == _map.end())
        return;

    it->second.erase(req_id);

    if (!_stop)
    {
        schedule(deviceId);
        _callBack(response_to_send, deviceId);  // send result to client
    }
}

void SchedulerService::SetCallback(std::function<void(const dxrt::dxrt_response_t&, int)> f)
{
    _callBack = f;
}

void SchedulerService::SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t)> f)
{
    _errCallBack = f;
}

void SchedulerService::cleanDiedProcess(int pid)
{
    _map.erase(pid);
}
