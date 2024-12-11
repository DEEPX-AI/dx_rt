// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#include "dxrt/common.h"


#include <csignal>
#include <atomic>
#include <thread>
#include <mutex>
#include "memory_service.hpp"
#include "service_device.h"
#include "dxrt/device.h"
#include "service_error.h"

class SchedulerService
{
public:
    SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    virtual ~SchedulerService();
    void AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId);
    void FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data);



    int Load(int deviceId) const {return _loads[deviceId];}

    void SetCallback(std::function<void(const dxrt::dxrt_response_t&, int)> f);
    void SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t)> f);
    void cleanDiedProcess(int pid);
    void StopScheduler(int procId);
    void StartScheduler(int procId);
    int GetProcLoad(int procId);
    void ClearAllLoad();
    void ClearProcLoad(int procId);

private:
    virtual void schedule(int deviceId);
    std::vector<int> _procId;

    std::queue<std::pair<int, int> > _queue;
    std::vector<std::queue<std::pair<int, int> > > _device_queues;
    std::vector<std::atomic<int> > _loads;
    std::map<int,std::atomic<int>> _loadsProc;
    std::map<int,std::map<int, dxrt::dxrt_request_acc_t>> _map;
    std::mutex _lock;
    std::vector<std::shared_ptr<dxrt::ServiceDevice>> _devices;
    std::function<void(const dxrt::dxrt_response_t&, int)> _callBack;
    std::function<void(dxrt::dxrt_server_err_t, uint32_t)> _errCallBack;

};