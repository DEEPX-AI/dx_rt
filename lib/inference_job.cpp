// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_job.h"
#include "dxrt/inference_engine.h"
#include "dxrt/exception/exception.h"
#include "dxrt/objects_pool.h"

#include <future>
#include <memory>
#include <unordered_map>


namespace dxrt
{

bool debug_all_output = false;

std::mutex InferenceJob::_sTensorsMapLock;
std::mutex InferenceJob::_sRequestsLock;
std::mutex InferenceJob::_sInferenceJobsLock;

std::atomic<int> InferenceJob::_sNextInferenceJobId = {0};


// static functions
void InferenceJob::InitInferenceJob()
{
}

std::shared_ptr<InferenceJob> InferenceJob::GetById(int id)
{
    ObjectsPool* instance = ObjectsPool::GetInstance();
    return instance->GetInferenceJobById(id);
}

std::shared_ptr<InferenceJob> InferenceJob::Pick()
{
    int id;
    {
        std::unique_lock<std::mutex> lk(_sInferenceJobsLock);
        while (true)
        {
            id = _sNextInferenceJobId++;
            if (_sNextInferenceJobId >= INFERENCE_ID_MAX_VALUE)
                _sNextInferenceJobId = 0;
            break;
        }
    }
    ObjectsPool* instance = ObjectsPool::GetInstance();
    InferenceJobPtr jobRef = instance->GetInferenceJobById(id);
    jobRef->Clear();
    return jobRef;
}
// static functions


void InferenceJob::onRequestComplete(RequestPtr req)
{
    bool allRequestComplete = false;
    Task* thisTask = req->task();
    {
        std::unique_lock<std::mutex> lk(_sTensorsMapLock);
        for (Tensor& output : req->outputs()) {
            auto name = output.name();

            _tensors.insert(make_pair(name, output));
        }
        _doneCount++;
        if (_doneCount == _outputCount)
            allRequestComplete = true;
        _latency += req->latency();
        _infTime += req->inference_time();
    }

    if (thisTask->is_tail() == false)
    {
        for (auto& nextTask : thisTask->nexts())
        {
            auto required_tensors = nextTask->inputs();
            bool allPrepared = true;
            Tensors nexts;

            for (auto& required : required_tensors)
            {
                {
                    std::unique_lock<std::mutex> lk(_sTensorsMapLock);
                    auto it = _tensors.find(required.name());
                    if (it == _tensors.end())
                    {
                        //cout<<"["<<thisTask->name()<<"] task require "<<required.name() << " tensor, it is not found yet"<<endl;
                        allPrepared = false;
                        break;
                    }
                    else
                    {
                        nexts.push_back(it->second);
                    }
                }
            }
            if (allPrepared)
            {
                //DXRT_ASSERT(nextTask.get() != nullptr, "nexttask null");
                if ( nextTask.get() == nullptr )
                    throw InvalidOperationException(EXCEPTION_MESSAGE("next task is null"));

                //auto nextReq = Request::Create(nextTask.get(), std::move(nexts), {}, _userArg);
                auto nextReq = Request::Create(nextTask.get(), nexts, {}, _userArg, _jobId);
                nextReq->setCallback(onRequestCompleteFunction());  // on each request complete, do next request or complete whole inference
                if (nextTask->nexts().empty())
                {
                    nextReq->getData()->output_ptr = _outputPtr;
                }
                nextReq->SetStatus(Request::Status::REQ_BUSY);
                nextReq->requestor_name() = req->task()->name();
                {
                    std::unique_lock<std::mutex> lk(_sRequestsLock);
                    _requests.push_back(nextReq);
                }
                TASK_FLOW_START("["+to_string(_jobId)+"]"+nextTask->name()+"");
                nextTask->InferenceRequest(nextReq);
            }
        }
    }
    else
    {
        if (allRequestComplete)
        {
           onAllRequestComplete();
        }
    }
    TASK_FLOW_FINISH("["+to_string(_jobId)+"]"+thisTask->name());
}

InferenceJob::InferenceJob(int id) noexcept
{
    _jobId = id;
}

void InferenceJob::onAllRequestComplete()
{
    TASK_FLOW("["+to_string(_jobId)+"] ALL COMPLETE ~ ")
    _inferenceEnginePtr->PushLatency(latency());
    _inferenceEnginePtr->PushInferenceTime(inference_time());

    if(_storeResult)
    {
        setReturnOutputs();
    }

    if (_infEngCallback !=nullptr)
    {
        LOG_DXRT_DBG << "task callback" << endl;
        if (debug_all_output)
        {
            async(launch::async, [this](void){
                TensorPtrs ret;
                for (auto it : _tensors)
                {
                    ret.emplace_back(
                        make_shared<Tensor>(it.second));
                }
                _infEngCallback(ret, _userArg, _jobId);  // callback registered by inference_engine
                freeAllOutputBuffer();
                freeAllInputBuffer();
            });
        }
        else
        {
            DXRT_ASSERT(_doneCount == _outputCount , "outputcnt mismatch");
            future<void> result = async(launch::async, [this](void){
                TensorPtrs ret;
                for (auto &name : _outputs)
                {
                    ret.emplace_back(
                        make_shared<Tensor>(_tensors.find(name)->second));
                }
                _infEngCallback(ret, _userArg, _jobId);  // callback registered by inference_engine
                freeAllOutputBuffer();
                freeAllInputBuffer();

            });
        }
    }
    else
    {
        freeAllOutputBuffer();
        freeAllInputBuffer();
    }

    for (auto it : _requests)
    {
        RequestPtr req = it.lock();
        if (req)
        {
            req->Reset();
        }
    }
    _requests.clear();
    _status = Request::Status::REQ_DONE;
}

void InferenceJob::SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_, std::vector<string> lastOutputOrder)
{
    _headTask = head_;
    _doneCount = 0;
    _latency = 0;
    _infTime = 0;

    //_tasks.clear();
    _outputs.clear();
    _outputs = lastOutputOrder; 

    _outputCount = tasks_.size();
    for (std::shared_ptr<Task>& it :  tasks_)
    {
        //int taskId = it->id();
        if (_outputs.empty() && it->nexts().empty())
        {
            LOG_DXRT<<"Warning : Output order is not set"<<endl;
            for (auto output : it->outputs())
            {
                _outputs.push_back(output.name());
            }
        }
        //_tasks.emplace(taskId, std::move(it));
        //_tasks.emplace(taskId, it);
    }
}

std::function<void(RequestPtr)> InferenceJob::onRequestCompleteFunction()
{
    return [this](RequestPtr req) {
        onRequestComplete(req);
    };
}

int InferenceJob::startJob(void *inputPtr, void *userArg, void *outputPtr)
{
    TaskPtr task = _headTask.lock();
    if(!task)
    {
        return -1;
    }
    RequestPtr req = Request::Create(task.get(), inputPtr, outputPtr, userArg, _jobId);
    _status = Request::Status::REQ_BUSY;
    _userArg = userArg;
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->setCallback(onRequestCompleteFunction());  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);
    _outputPtr = outputPtr;
    if(_headTask.lock()->nexts().empty())
    {
        req->getData()->output_ptr = _outputPtr;
    }
    else
        req->getData()->output_ptr = nullptr;
    req->task()->InferenceRequest(req);
    return _jobId;
}


void InferenceJob::setReturnOutputs()
{
    TensorPtrs ret;

    int output_size_increment = 0;
    for (auto &name : _outputs)
    {
        auto output = _tensors.find(name)->second;
        int output_size = output.elem_size();

        for(int size: output.shape())
        {
            output_size *= size;
        }
        shared_ptr<vector<uint8_t> > memory = make_shared<vector<uint8_t> >(output_size);
        memcpy(memory->data(), output.data(),output_size);
        shared_ptr<Tensor> ptr =  std::shared_ptr<Tensor>(new Tensor(_tensors.find(name)->second,memory->data()),
            [memory](Tensor* p){
                delete p;
                memory->clear();
            });

        ret.emplace_back(
            make_shared<Tensor>(_tensors.find(name)->second));
        output_size_increment += output_size;
    }
    _returnOutputs = ret;
}

TensorPtrs InferenceJob::getOutput()
{
    return std::move(_returnOutputs);
}

void InferenceJob::SetStoreResult(bool storeResult)
{ 
    _storeResult = storeResult;
}

void InferenceJob::setInferenceEngineInterface(InferenceTimer* ptr)
{
    _inferenceEnginePtr = ptr;
}

void InferenceJob::setCallBack(std::function<int(TensorPtrs &outputs, void *userArg, int jobId)> func)
{
    _infEngCallback = func;
}

void InferenceJob::Clear()
{
    //_requests.clear();
    _tensors.clear();
    //_tasks.clear();
    //_head.reset();
    //_headTask.reset();
    _status = Request::Status::REQ_IDLE;
    _outputCount = {0};
    _doneCount = {0};
    //_outputs.clear();
    _userArg = nullptr;
    _latency = 0;
    _infTime = 0;
    _inferenceEnginePtr = nullptr;
    _infEngCallback = nullptr;
    _outputPtr = nullptr;
}

InferenceJob::~InferenceJob()
{
    Clear();
}

void InferenceJob::freeAllOutputBuffer()
{
    std::unique_lock<std::mutex> lk(_sInferenceJobsLock);
    for (auto& req_weak_ptr :  _requests)
    {
        RequestPtr req = req_weak_ptr.lock();
        if (req)
        {
            if((_outputPtr == nullptr) || (req->task()->is_tail()==false))
            {
                req->task()->FreeOutputBuffer(req->output_ptr());
            }
        }
    }
}
void InferenceJob::freeAllInputBuffer()
{
    std::unique_lock<std::mutex> lk(_sInferenceJobsLock);
    for (auto& req_weak_ptr :  _requests)
    {
        RequestPtr req = req_weak_ptr.lock();
        if (req && req->task()->processor()==Processor::CPU)
        {
            req->task()->FreeInputBuffer(req->output_ptr());
        }
    }
}
}  // namespace dxrt
