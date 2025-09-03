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
#include <vector>
#include <map>
#include "memory_service.hpp"
#include "service_device.h"
#include "dxrt/device.h"
#include "service_error.h"

class SchedulerService
{
 public:
    explicit SchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    virtual ~SchedulerService();
    void AddScheduler(const dxrt::dxrt_request_acc_t& packet_data, int deviceId);
    void FinishJobs(int deviceId, const dxrt::dxrt_response_t& response_data);
    void SendError(int deviceId, dxrt::dxrt_server_err_t err, uint32_t errCode);


    int Load(int deviceId) const {return _loads[deviceId];}

    void SetCallback(std::function<void(const dxrt::dxrt_response_t&, int)> f);
    void SetErrorCallback(std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> f);
    void cleanDiedProcess(int pid);
    void StopScheduler(int procId);
    void StartScheduler(int procId);
    int GetProcLoad(int procId);
    void ClearAllLoad();
    void ClearProcLoad(int procId);
    
    // Task validity verification callback
    void SetTaskValidator(std::function<bool(pid_t, int, int)> validator);
    
    // Stop inference requests for a specific task
    void StopTaskInference(pid_t pid, int deviceId, int taskId);
    void StopAllInferenceForProcess(pid_t pid, int deviceId);

 protected:
    virtual void schedule(int deviceId) = 0;
    virtual void pushRequest(int deviceId, int procId, int reqId, int taskId) = 0;
    virtual void updateTaskInferenceTime(int procId, int taskId, uint32_t time);
    virtual uint32_t getTaskInferenceTime(int procId, int taskId);
    virtual void cleanTaskInferenceTime(int procId);
    void doInference(int deviceId, int procId, int reqId);
    std::vector<int> _procId;

    std::vector<std::atomic<int> > _loads;
    std::map<int,std::atomic<int>> _loadsProc;
    std::map<int,std::map<int, dxrt::dxrt_request_acc_t>> _map;
    std::mutex _lock;
    std::vector<std::shared_ptr<dxrt::ServiceDevice>> _devices;
    std::function<void(const dxrt::dxrt_response_t&, int)> _callBack;
    std::function<void(dxrt::dxrt_server_err_t, uint32_t, int)> _errCallBack;
    
    // Task validity verification callback
    std::function<bool(pid_t, int, int)> _taskValidator;
};

class FIFOSchedulerService : public SchedulerService
{
 public:
    explicit FIFOSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    ~FIFOSchedulerService() override;

 protected:
    void schedule(int deviceId) override;
    void pushRequest(int deviceId, int procId, int reqId, int taskId) override;

    std::vector<std::queue<std::pair<int, int> > > _device_queues;

};

class RoundRobinSchedulerService : public SchedulerService
{
 public:
    explicit RoundRobinSchedulerService(std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices_);
    ~RoundRobinSchedulerService() override;

 protected:
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
 protected:

    void schedule(int deviceId) override;
    void pushRequest(int deviceId, int procId, int reqId, int taskId) override;
    //void updateTaskInferenceTime(int procId, int taskId, uint32_t time) override;
    std::vector<std::priority_queue<request_elem> > request_map;
    std::multimap<int, std::pair<int, int> > key_less_map;
};