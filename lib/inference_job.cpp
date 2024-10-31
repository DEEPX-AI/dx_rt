// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/tensor.h"
#include "dxrt/inference_job.h"
#include "dxrt/inference_engine.h"
#include <future>
#include <memory>
#include <unordered_map>


namespace dxrt
{

bool debug_all_output = false;


std::mutex InferenceJob::_sInferenceJobsMapLock;
std::mutex InferenceJob::_sInferenceJobsLock;
std::atomic<int> InferenceJob::_sNextInferenceJobId = {0};
std::vector<std::shared_ptr<InferenceJob> > InferenceJob::_sInferenceJobs;


// static functions
void InferenceJob::InitInferenceJob()
{
    std::unique_lock<std::mutex> lk(_sInferenceJobsMapLock);
    if (_sInferenceJobs.empty())
    {
        for (int i = 0; i < INFERENCE_ID_MAX_VALUE; i++)
        {
            _sInferenceJobs.push_back(make_shared<InferenceJob>(i));
        }
    }
}

std::shared_ptr<InferenceJob>& InferenceJob::GetById(int id)
{
    return _sInferenceJobs[id];
}

std::shared_ptr<InferenceJob>& InferenceJob::Pick()
{
    int id;
    {
        std::unique_lock<std::mutex> lk(_sInferenceJobsLock);
        id = _sNextInferenceJobId++;
        if (_sNextInferenceJobId >= INFERENCE_ID_MAX_VALUE)
            _sNextInferenceJobId = 0;

    }

    std::shared_ptr<InferenceJob> &jobRef = InferenceJob::GetById(id);
    jobRef->Clear();
    return jobRef;
}
// static functions



void InferenceJob::onRequestComplete(RequestPtr req)
{
    Task* thisTask = req->task();
    for (Tensor& output : req->outputs())
    {
        auto names = output.name();
        _tensors.insert(make_pair(names, output));
    }
    _doneCount++;
    _latency += req->latency();
    _infTime += req->inference_time();


    if (thisTask->nexts().empty() == false)
    {
        for (auto& nextTask : thisTask->nexts())
        {
            auto required_tensors = nextTask->inputs();
            bool allPrepared = true;
            Tensors nexts;

            for (auto& required : required_tensors)
            {
                auto it = _tensors.find(required.name());
                if (it == _tensors.end())
                {
                    allPrepared = false;
                    break;
                }
                else
                {
                    nexts.push_back(it->second);
                }
            }
            if (allPrepared)
            {
                DXRT_ASSERT(nextTask.get() != nullptr, "nexttask null");
                //auto nextReq = Request::Create(nextTask.get(), std::move(nexts), {}, _userArg);
                auto nextReq = Request::Create(nextTask.get(), nexts, {}, _userArg);
                nextReq->setCallback(onRequestCompleteFunction());  // on each request complete, do next request or complete whole inference
                nextReq->SetStatus(Request::Status::REQ_BUSY);
                nextReq->requestor_name() = req->task()->name();
                _requests.push_back(req);
                nextTask->InferenceRequest(nextReq);
            }
        }
    }
    else
    {
        // check all outputs complete
        if (_doneCount == _outputCount)
        {
           onAllRequestComplate();
        }
    }
}

InferenceJob::InferenceJob(int id) noexcept
{
    //std::unique_lock<std::mutex> lk(_inferenceJobsLock);
    _jobId = id;
}

void InferenceJob::onAllRequestComplate()
{
    _inferenceEnginePtr->PushLatency(latency());
    _inferenceEnginePtr->PushInferenceTime(inference_time());
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
            });
        }
    }

    _status = Request::Status::REQ_DONE;
    for (auto it : _requests)
    {
        it->Reset();
    }
}

void InferenceJob::SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_)
{
    _headTask = head_;
    _doneCount = 0;
    _latency = 0;
    _infTime = 0;


    _tasks.clear();
    _outputs.clear();

    _outputCount = tasks_.size();
    for (std::shared_ptr<Task>& it :  tasks_)
    {
        int taskId = it->id();
        if (it->nexts().empty())
        {
            for (auto output : it->outputs())
            {
                _outputs.push_back(output.name());
            }
        }
        //_tasks.emplace(taskId, std::move(it));
        _tasks.emplace(taskId, it);
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
    RequestPtr req = Request::Create(_headTask.get(), inputPtr, outputPtr, userArg);
    _status = Request::Status::REQ_BUSY;
    _userArg = userArg;
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->setCallback(onRequestCompleteFunction());  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);
    req->task()->InferenceRequest(req);
    return _jobId;
}

TensorPtrs InferenceJob::getOutput()
{
    TensorPtrs ret;

    for (auto &name : _outputs)
    {
        ret.emplace_back(
            make_shared<Tensor>(_tensors.find(name)->second));
    }
    return ret;
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
    _requests.clear();
    _tensors.clear();
    _tasks.clear();
    _head = nullptr;
    _headTask = nullptr;
    _status = Request::Status::REQ_IDLE;
    _outputCount = {0};
    _doneCount = {0};
    _outputs.clear();
    _userArg = nullptr;
    _latency = 0;
    _infTime = 0;
    _inferenceEnginePtr = nullptr;
    _infEngCallback = nullptr;
}

InferenceJob::~InferenceJob()
{
    Clear();
}

}  // namespace dxrt
