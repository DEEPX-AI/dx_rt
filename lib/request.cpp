// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/request.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "dxrt/inference_engine.h"
#include "dxrt/inference_job.h"
#include "dxrt/profiler.h"
#include "dxrt/objects_pool.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>

using namespace std;

namespace dxrt
{

std::mutex Request::_reqLock;

Request::Request(void)
{
    _data.requestId = 0;
    LOG_DXRT_DBG << getData()->requestId << endl;
}
Request::Request(int id)
{
    _data.requestId = id;
    _data.inputs = {};
    _data.outputs = {};
    _timePoint = make_shared<TimePoint>();

    //LOG_DXRT_DBG << getData()->requestId << endl;
}

Request::Request(Task *task_, Tensors &inputs_, Tensors &outputs_)
: _task(task_)
{

    _data.inputs = inputs_;
    _data.outputs = outputs_;
    _timePoint = make_shared<TimePoint>();
}
Request::~Request()
{
    // LOG_DXRT_DBG << id() << endl;
    // LOG_DXRT << id() << endl;
}

void Request::Init()
{
    //LOG_DXRT_DBG << endl;

}

RequestPtr Request::Create(Task *task_, Tensors inputs_, Tensors outputs_, void *userArg, int jobId)
{
    RequestPtr req = Request::Pick();
    req->_task = task_;
    req->_data.taskData = task_->getData();
    req->_data.inputs.clear();
    req->_data.outputs.clear();

    req->setInputs(inputs_);
    req->setOutputs(outputs_);
    req->_userArg = userArg;
    req->latency_valid() = true;
    req->latency() = 0;
    req->inference_time() = 0;
    req->_requestorName = "";
    req->_data.jobId = jobId;
    req->_data.output_ptr = nullptr;

    return req;
}

RequestPtr Request::Create(Task *task_, void *input, void *output, void *userArg, int jobId)
{
    RequestPtr req = Request::Pick();
    req->_task = task_;
    req->_data.taskData = task_->getData();

    if(input==nullptr)
        req->setInputs({});
    else
        req->setInputs(task_->inputs(input)); // TODO: check to move to device?
    if(output==nullptr)
        req->setOutputs({});
    else    
        req->setOutputs(task_->outputs(output));  // TODO: move to device?
    req->_userArg = userArg;
    req->latency_valid() = true;
    req->latency() = 0;
    req->inference_time() = 0;
    req->_requestorName = "";
    req->_data.jobId = jobId;
    req->_data.output_ptr = nullptr;

    return req;
}
RequestPtr Request::GetById(int id)
{
    return ObjectsPool::GetInstance().GetRequestById(id);
}
RequestPtr Request::Pick()
{
    return ObjectsPool::GetInstance().PickRequest();
}
void Request::ShowAll()
{
    LOG_DXRT_DBG << ObjectsPool::REQUEST_MAX_COUNT << endl;
    for (int i = 0; i < ObjectsPool::REQUEST_MAX_COUNT; i++)
    {
        RequestPtr request = GetById(i);
        cout << dec << "(" << request.use_count() << ") " << *request << endl;
    }
}
void Request::Clear()
{
    LOG_DXRT_DBG << endl;
}
void Request::Wait()
{
    LOG_DXRT_DBG << "request " << id() << endl;
    while( status() == Request::Status::REQ_BUSY );
}
void Request::SetStatus(Request::Status status)
{
    LOG_DXRT_DBG << id() << ", " << status << endl;
    _status.store(status);
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
int Request::job_id() const
{
    return _data.jobId;
}
void Request::set_processed_unit(std::string processedPU, int processedDevId, int processedId)
{
    _data._processedPU = processedPU;
    _data._processedDevId = processedDevId;
    _data._processedId = processedId;
}
std::string Request::processed_pu() const
{
    return _data._processedPU;
}
int Request::processed_id() const
{
    return _data._processedId;
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
Tensors Request::inputs()
{
    unique_lock<mutex> lk(_reqLock);
    return _data.inputs;
}
Tensors Request::outputs()
{
    unique_lock<mutex> lk(_reqLock);
    return _data.outputs;
}
void * Request::input_ptr()
{
    unique_lock<mutex> lk(_reqLock);
    if (_data.inputs.empty())
        return nullptr;
    return _data.inputs.front().data();
}
void * Request::output_ptr()
{
    unique_lock<mutex> lk(_reqLock);
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
TimePointPtr Request::time_point()
{
    return _timePoint;
}
Request::Status Request::status()
{
    return _status.load();
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
void Request::setNpuInferenceAcc(dxrt_request_acc_t npuInferenceAcc)
{
    _npuInferenceAcc=npuInferenceAcc;
}
void Request::setInferenceJob(InferenceJob* job)
{
    _job = job;
}
void Request::onRequestComplete(RequestPtr req)
{
    SetStatus(Request::Status::REQ_DONE);
#ifdef USE_PROFILER
    _task->IncrementCompleteCount();
#endif
    if (_job != nullptr)
        _job->onRequestComplete(req);
}
void Request::Reset()
{
    _data.taskData = nullptr;
    setInputs({});
    setOutputs({});

    _userArg = nullptr;

    _requestorName = "";
    _job = nullptr;
    SetStatus(Status::REQ_IDLE);
    
    _task = nullptr;
    _use_flag = false;
}

void Request::setInputs(Tensors input)
{
    unique_lock<mutex> lk(_reqLock);
    _data.inputs.clear();
    _data.inputs = input;
}
void Request::setOutputs(Tensors output)
{
    unique_lock<mutex> lk(_reqLock);
    _data.outputs.clear();
    _data.outputs = output;
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