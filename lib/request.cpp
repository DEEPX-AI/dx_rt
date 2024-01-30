// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/request.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "dxrt/inference_engine.h"
#include "dxrt/profiler.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <shared_mutex>

using namespace std;

namespace dxrt
{

int Request::_nextId = REQUEST_ID_INIT_VALUE;
shared_mutex Request::_idLock;
// unordered_map<int, Request> Request::_requestMap;
// unordered_map<int, RequestPtr> Request::_requestMap;
// Request _requestMapArr[REQUEST_ID_MAX_VALUE+1];
vector<RequestPtr> Request::_requestMap;
shared_mutex Request::_mapLock;

Request::Request(void)
{
    LOG_DXRT_DBG << _id << endl;
}
Request::Request(Task *task_, Tensors &inputs_, Tensors &outputs_)
: _task(task_), _inputs(inputs_), _outputs(outputs_)
{
    unique_lock<shared_mutex> lk(_idLock);
    _id = _nextId++;
    if(_nextId>REQUEST_ID_MAX_VALUE) _nextId = REQUEST_ID_INIT_VALUE;
    lk.unlock();
    _timePoint = make_shared<TimePoint>();
}

Request::~Request()
{    
    // LOG_DXRT_DBG << _id << endl;
    // LOG_DXRT << _id << endl;
}

void Request::Init()
{
    LOG_DXRT_DBG << endl;
    if(_requestMap.empty())
    {
        for(int i=0;i<REQUEST_ID_MAX_VALUE;i++)
        {
            unique_lock<shared_mutex> lk(_mapLock);
            Tensors emptyTensors{};
            RequestPtr request = make_shared<Request>(nullptr, emptyTensors, emptyTensors);
            _requestMap.emplace_back(request);
        }
        _nextId = REQUEST_ID_INIT_VALUE;
    }
}

RequestPtr Request::Create(Task *task_, Tensors inputs_, Tensors outputs_)
{
#if 0
    RequestPtr ret = make_shared<Request>(task_, inputs_, outputs_);
    return ret;
#else
    RequestPtr req = Request::Pick();    
    req->task() = task_;
    req->inputs() = move(inputs_);
    req->outputs() = move(outputs_);
    if(req->task()->is_head())
    {
        req->head() = req;
        req->last_output_ptr() = req->outputs().front().data();
    }
    req->latency_valid() = true;
    return req;
#endif
}
RequestPtr Request::Create(Task *task_, void *input, void *output, void *userArg, void *lastOutput)
{
    RequestPtr req = Request::Pick();
    req->task() = task_;
    if(req->task()->is_head())
    {
        req->head() = req;
        req->last_output_ptr() = lastOutput;
    }
    req->input_ptr() = input;
    req->output_ptr() = output;
    if(input==nullptr)
        req->inputs() = {};
    else
        req->inputs() = task_->inputs(input); // TODO: check to move to device?
    if(output==nullptr)
        req->outputs() = {};
    else    
        req->outputs() = task_->outputs(output);  // TODO: move to device?
    req->user_arg() = userArg;
    req->latency_valid() = true;
    return req;
}
RequestPtr Request::GetById(int id)
{
    LOG_DXRT_DBG << id << endl;
    return _requestMap[ id - REQUEST_ID_INIT_VALUE ];
}
RequestPtr Request::Pick()
{
#if 0
    while(1)
    {
        for(auto &request:_requestMap)
        {
            if(request->status()!=Request::Status::REQ_BUSY)
            {
                LOG_DXRT_DBG << "pick request " << request->id() << endl;
                return request;
            }
        }
    }
#else
    int id;
    {
        unique_lock lk(_idLock);
        id = _nextId++;
        if(_nextId>REQUEST_ID_MAX_VALUE) _nextId = REQUEST_ID_INIT_VALUE;
    }
    return GetById(id);
#endif
}
void Request::ShowAll()
{
    LOG_DXRT_DBG << _requestMap.size() << endl;
    for(auto &request : _requestMap)
    {
        cout << dec << "(" << request.use_count() << ") " << *request << endl;
    }
}
void Request::Clear()
{
    LOG_DXRT_DBG << endl;
    {
        unique_lock<shared_mutex> lk(_mapLock);
        _requestMap.clear();
    }
    {
        unique_lock<shared_mutex> lk(_idLock);
        _nextId = REQUEST_ID_INIT_VALUE;
    }
}
void Request::SaveTaskStats(Task *task)
{
    LOG_DXRT_DBG << endl;
    auto &profiler = Profiler::GetInstance();
    auto &stats = TaskStats::GetInstance(task->id());
    int cnt = 0;
    stats.id = task->id();
    stats.latency_us = 0;
    stats.inference_time_us = 0;
    for(auto &req:_requestMap)
    {
        if(req->task()==task)
        {
            cnt++;
            if(req->latency_valid())
            {
                stats.latency_data.emplace_back(req->latency());
            }
            stats.inference_time_data.emplace_back(req->inference_time());
            profiler.AddTimePoint("Task"+to_string(task->id()), req->time_point());
        }
    }
    // LOG_VALUE(stats.latency_data.size());
    if(!stats.latency_data.empty())
    {
        for(auto &latency:stats.latency_data)
        {
            stats.latency_us += latency;
        }
        stats.latency_us /= stats.latency_data.size();
    }
    if(!stats.inference_time_data.empty())
    {
        for(auto &infTime:stats.inference_time_data)
        {
            stats.inference_time_us += infTime;
        }
        stats.inference_time_us /= stats.inference_time_data.size();
    }
}
void Request::Wait()
{
    LOG_DXRT_DBG << "request " << _id << endl;
    while( status() == Request::Status::REQ_BUSY );
}
void Request::SetStatus(Request::Status status)
{
    LOG_DXRT_DBG << _id << ", " << status << endl;
    unique_lock<mutex> lk(_statusLock);
    _status = status;
}
void Request::CheckTimePoint(int opt)
{
    LOG_DXRT_DBG << endl;
    if(opt==0)
    {
        _timePoint->start = ProfilerClock::now();
    }
    else
    {
        _timePoint->end = ProfilerClock::now();
        _latency = chrono::duration_cast<chrono::microseconds>(_timePoint->end - _timePoint->start).count();
    }

}

int &Request::id()
{
    return _id;
}
RequestPtr &Request::head()
{
    return _head;
}
Task* &Request::task()
{
    return _task;
}
Tensors &Request::inputs()
{
    return _inputs;
}
Tensors &Request::outputs()
{
    return _outputs;
}
void * &Request::input_ptr()
{
    return _inputPtr;
}
void * &Request::output_ptr()
{
    return _outputPtr;
}
void * &Request::last_output_ptr()
{
    return _lastOutputPtr;
}
void * &Request::user_arg()
{
    return _userArg;
}
void * &Request::dev_arg()
{
    return _devArg;
}
dxrt_request_t &Request::npu_inference()
{
    return _npuInference;
}
dxrt_request_t* &Request::npu_inference_ptr()
{
    return _npuInferencePtr;
}
dxrt_request_acc_t &Request::npu_inference_acc()
{
    return _npuInferenceAcc;
}
uint32_t &Request::inference_time()
{
    return _infTime;
}
TimePointPtr Request::time_point()
{
    return _timePoint;
}
Request::Status Request::status()
{
    unique_lock<mutex> lk(_statusLock);
    return _status;
}
int &Request::latency()
{
    return _latency;
}
bool &Request::latency_valid()
{
    return _latencyValid;
}
int16_t &Request::model_type()
{
    return _modelType;
}

RequestMap::RequestMap()
{
    LOG_DXRT_DBG << endl;
}
RequestMap::~RequestMap()
{
    LOG_DXRT_DBG << endl;
}
RequestPtr RequestMap::GetById(int id)
{
    LOG_DXRT_DBG << id << endl;
    shared_lock<shared_mutex> lk(_lock);
    auto it = _map.find(id);
    if(it != _map.end())
    {
        // LOG_DXRT_DBG << "found request " << id << endl;
        // LOG_DXRT_DBG << "found request " << id << ": " << *it->second << endl;
        // LOG_VALUE(it->second.use_count());
        return it->second;
    }
    else
    {
        LOG_DXRT_DBG << "cannot find request " << id << endl;
        return nullptr;
    }
}
int RequestMap::Add(RequestPtr req)
{
    unique_lock<shared_mutex> lk(_lock);
    _map[req->id()] = req;
}

ostream& operator<<(ostream& os, const Request& req)
{
    os << dec << "  Req. " << req._id << " -> task " << ((req._task!=nullptr)?(to_string(req._task->id())):"null") << endl;
    for(auto &tensor:req._inputs)
    {
        os << tensor << endl;
    }
    for(auto &tensor:req._outputs)
    {
        os << tensor << endl;
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const Request::Status& status)
{
    switch(status)
    {
        case Request::Status::REQ_IDLE: os << "IDLE"; break;
        case Request::Status::REQ_BUSY: os << "BUSY"; break;
        case Request::Status::REQ_DONE: os << "DONE"; break;        
    }
    return os;

}

} // namespace dxrt