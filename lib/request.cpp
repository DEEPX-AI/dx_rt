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
#include <mutex>

using namespace std;

namespace dxrt
{

int Request::_nextId = REQUEST_ID_INIT_VALUE;
std::mutex Request::_idLock;
// unordered_map<int, Request> Request::_requestMap;
// unordered_map<int, RequestPtr> Request::_requestMap;
// Request _requestMapArr[REQUEST_ID_MAX_VALUE+1];
vector<RequestPtr> Request::_requestMap;
std::mutex Request::_mapLock;

Request::Request(void)
{
    LOG_DXRT_DBG << getData()->requestId << endl;
}
Request::Request(Task *task_, Tensors &inputs_, Tensors &outputs_)
: _task(task_)
{
    unique_lock<mutex> lk(_idLock);
    _data.requestId = _nextId++;
    _data.inputs = inputs_;
    _data.outputs = outputs_;
    if(_nextId>REQUEST_ID_MAX_VALUE) _nextId = REQUEST_ID_INIT_VALUE;
    lk.unlock();
    _timePoint = make_shared<TimePoint>();
}
Request::~Request()
{
    // LOG_DXRT_DBG << id() << endl;
    // LOG_DXRT << id() << endl;
}

void Request::Init()
{
    LOG_DXRT_DBG << endl;
    if(_requestMap.empty())
    {
        for(int i=0;i<REQUEST_ID_MAX_VALUE;i++)
        {
            unique_lock<mutex> lk(_mapLock);
            Tensors emptyTensors{};
            RequestPtr request = make_shared<Request>(nullptr, emptyTensors, emptyTensors);
            _requestMap.emplace_back(request);
        }
        _nextId = REQUEST_ID_INIT_VALUE;
    }
}

RequestPtr Request::Create(Task *task_, Tensors inputs_, Tensors outputs_, void *userArg)
{
    RequestPtr req = Request::Pick();
    req->_task = task_;
    req->_data.taskData = task_->getData();

    req->inputs() = move(inputs_);
    req->outputs() = move(outputs_);
    req->_userArg = userArg;
    req->latency_valid() = true;
    req->latency() = 0;
    req->inference_time() = 0;
    req->complete_cnt() = 0;
    req->_requestorName == "";

    const char* env = getenv("DXRT_DEBUG_DATA");
    if (env)
    {
        try {
            req->_debugEnv = std::stoi(env);
            Device::_sNpuValidateOpt = true;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Environment variable DXRT_DEBUG_DATA is invalid: " << e.what() << endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Environment variable DXRT_DEBUG_DATA is out of range: " << e.what() << endl;
        }
    }

    return req;
}
RequestPtr Request::Create(Task *task_, void *input, void *output, void *userArg)
{
    RequestPtr req = Request::Pick();
    req->_task = task_;
    req->_data.taskData = task_->getData();

    if(input==nullptr)
        req->inputs() = {};
    else
        req->inputs() = task_->inputs(input); // TODO: check to move to device?
    if(output==nullptr)
        req->outputs() = {};
    else    
        req->outputs() = task_->outputs(output);  // TODO: move to device?
    req->_userArg = userArg;
    req->latency_valid() = true;
    req->latency() = 0;
    req->inference_time() = 0;
    req->complete_cnt() = 0;
    req->_requestorName == "";

    const char* env = getenv("DXRT_DEBUG_DATA");
    if (env)
    {
        try {
            req->_debugEnv = std::stoi(env);
            Device::_sNpuValidateOpt = true;

        } catch (const std::invalid_argument& e) {
            std::cerr << "Environment variable DXRT_DEBUG_DATA is invalid: " << e.what() << endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Environment variable DXRT_DEBUG_DATA is out of range: " << e.what() << endl;
        }
    }

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
        unique_lock<mutex> lk(_idLock);
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
        unique_lock<mutex> lk(_mapLock);
        _requestMap.clear();
    }
    {
        unique_lock<mutex> lk(_idLock);
        _nextId = REQUEST_ID_INIT_VALUE;
    }
}
void Request::SaveTaskStats(Task *task)
{
    LOG_DXRT_DBG << endl;
    auto &profiler = Profiler::GetInstance();
    auto &stats = TaskStats::GetInstance(task->id());
    int cnt = 0;
    stats.name = task->name();
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
            profiler.AddTimePoint(task->name(), req->time_point());
        }
    }

    // LOG_VALUE(stats.latency_data.size());
    if(!stats.latency_data.empty())
    {
        cnt = 0;
        for(auto &latency:stats.latency_data)
        {
            if(latency>0)
            {
                // LOG_VALUE(latency);
                stats.latency_us += latency;
                cnt++;
            }
        }
        if(cnt>0)
            stats.latency_us /= cnt;

    }
    if(!stats.inference_time_data.empty())
    {
        cnt = 0;
        for(auto &infTime:stats.inference_time_data)
        {
            if(infTime>0)
            {
                stats.inference_time_us += infTime;
                cnt++;
            }
        }
        if(cnt>0)
            stats.inference_time_us /= cnt;

    }
}
void Request::Wait()
{
    LOG_DXRT_DBG << "request " << id() << endl;
    while( status() == Request::Status::REQ_BUSY );
}
void Request::SetStatus(Request::Status status)
{
    LOG_DXRT_DBG << id() << ", " << status << endl;
    unique_lock<mutex> lk(_statusLock);
    _status = status;
}
void Request::CheckTimePoint(int opt)
{
    LOG_DXRT_DBG << endl;
    // cout << "        tp: req" << id() << ", " << opt << endl;
    if(opt==0)
    {
        _timePoint->start = ProfilerClock::now();
    }
    else
    {
        _timePoint->end = ProfilerClock::now();
        _latency = chrono::duration_cast<chrono::microseconds>(_timePoint->end - _timePoint->start).count();
        // LOG_VALUE(_latency);
    }

}

int Request::id() const
{
    return _data.requestId;
}

TaskData* Request::taskData()
{
    return _data.taskData;
}
Task* Request::task()
{
    return _task;
}
std::string Request::requestor_name() const
{
    return _requestorName;
}
Tensors &Request::inputs()
{
    return _data.inputs;
}
Tensors &Request::outputs()
{
    return _data.outputs;
}
void * Request::input_ptr()
{
    if (_data.inputs.empty())
        return nullptr;
    return _data.inputs.front().data();
}
void * Request::output_ptr()
{
    if (_data.outputs.empty())
        return nullptr;
    return _data.outputs.front().data();
}
void * Request::user_arg() const
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
int &Request::complete_cnt()
{
    unique_lock<mutex> lk(_completeCntLock);
    return _completeCnt;
}
void Request::NotifyCompletion()
{
    unique_lock<mutex> lk(_completeCntLock);
    _completeCnt++;
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
bool &Request::validate_device()
{
    return _validateDevice;
}
int16_t &Request::model_type()
{
    return _modelType;
}
int Request::debug_env()
{
    return _debugEnv;
}
void Request::setNpuInferenceAcc(dxrt_request_acc_t npuInferenceAcc)
{
    _npuInferenceAcc=npuInferenceAcc;
}
void Request::setCallback(std::function<void(RequestPtr)> func)
{
    _callback = func;
}
void Request::onRequestComplete(RequestPtr req)
{
    _status = Request::Status::REQ_DONE;
    _callback(req);  // callback registered by InferenceJobs
}
void Request::Reset()
{
    _data.taskData = nullptr;
    _data.inputs = {};
    _data.outputs = {};

    _userArg = nullptr;

    _requestorName = "";
    _callback = nullptr;
    _status = Status::REQ_IDLE;
    
    _task = nullptr;
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
    unique_lock<mutex> lk(_lock);
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
    unique_lock<mutex> lk(_lock);
    _map[req->id()] = req;
    return 0;
}

ostream& operator<<(ostream& os, const Request& req)
{
    os << dec << "  Req. " << req.id() << " -> task " << ((req.getData()->taskData!=nullptr)?(to_string(req.getData()->taskData->id())):"null") << endl;
    for(auto &tensor : req.getData()->inputs)
    {
        os << tensor << endl;
    }
    for(auto &tensor : req.getData()->outputs)
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

RequestData* Request::getData()
{
    return &_data;
}

const RequestData* Request::getData() const
{
    return &_data;
}

} // namespace dxrt