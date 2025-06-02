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
#include "dxrt/util.h"

#include <future>
#include <memory>
#include <unordered_map>


namespace dxrt
{

bool debug_all_output = false;


void InferenceJob::onRequestComplete(RequestPtr req)
{
    bool allRequestComplete = false;
    Task* thisTask = req->task();

    {
        std::unique_lock<std::mutex> lk(_lock);

        // 1. _tensors udpate
        for (const Tensor& output : req->outputs()) {
            auto name = output.name();
            DXRT_ASSERT(_tensors.find(name) == _tensors.end(), "not cleared");
            _tensors.insert(make_pair(name, output));
        }
        _doneCount++;
        if (_doneCount.load() == _outputCount.load())
        {
            allRequestComplete = true;
        }
        _latency += req->latency();
        if (req->task()->processor() == Processor::NPU) 
            _infTime += req->inference_time();

        // processed task status update
        auto completedTaskIt = _taskStatusMap.find(thisTask->name());
        if (completedTaskIt != _taskStatusMap.end()) 
        {
            completedTaskIt->second = Status::TASK_DONE;
        }
        else
        {
            throw InvalidOperationException(EXCEPTION_MESSAGE("The task name was not found in this job."));
        }
        TASK_FLOW_FINISH("["+to_string(_jobId)+"]"+thisTask->name());
    }

    if (thisTask->is_tail() == false)
    {
        for (auto& nextTaskPtr : thisTask->nexts())
        {
            // 2. _taskStatusMap update (TASK_IDLE -> TASK_READY)
            if (checkAndSetTaskReady(nextTaskPtr))
            {
                // 3. ready task inference request
                processReadyTask(nextTaskPtr);
            }
        }
    }
    else
    {
        TASK_FLOW_FINISH("["+to_string(_jobId)+"]"+thisTask->name()+" (Tail Task)");
        if (allRequestComplete)
        {
            onAllRequestComplete();
        }
    }
}

bool InferenceJob::checkAndSetTaskReady(TaskPtr taskPtr)
{
    std::unique_lock<std::mutex> lk(_lock);
    auto it = _taskStatusMap.find(taskPtr->name());
    if (it == _taskStatusMap.end())
    {
        throw InvalidOperationException(EXCEPTION_MESSAGE("The task name was not found in this job."));
    }
    if (it->second == Status::TASK_IDLE)
    {
        auto required_tensors = taskPtr->inputs();
        bool allPrepared = true;
        for (const auto& required : required_tensors)
        {
            if (_tensors.find(required.name()) == _tensors.end())
            {
                allPrepared = false;
                break;
            }
        }
        if (allPrepared)
        {
            it->second = Status::TASK_READY;
            return true;
        }
    }
    return false;
}

void InferenceJob::processReadyTask(TaskPtr taskPtr)
{
    RequestPtr nextReq;
    {
        std::unique_lock<std::mutex> lk(_lock);
        auto it = _taskStatusMap.find(taskPtr->name());
        if (it == _taskStatusMap.end())
        {
            throw InvalidOperationException(EXCEPTION_MESSAGE("The task name was not found in this job."));
        }
        if (it->second == Status::TASK_READY)
        {
            Tensors nexts;
            auto required_tensors = taskPtr->inputs();
            for (const auto& required : required_tensors)
            {
                auto tensor_it = _tensors.find(required.name());
                if (tensor_it == _tensors.end()) {
                    // This should ideally not happen if checkAndSetTaskReady worked correctly
                    LOG_DXRT_ERR("Required tensor '" + required.name() + "' not found for task '" + taskPtr->name() + "'");
                    return;
                }
                nexts.push_back(tensor_it->second); // Call this function only when allPrepared is true
            }

            nextReq = Request::Create(taskPtr.get(), nexts, {}, _userArg, _jobId);
            nextReq->setInferenceJob(this);
            if ((taskPtr->nexts().empty()) && (_outputPtr != nullptr))
            {
                uint8_t* _offsetPtr = static_cast<uint8_t*>(_outputPtr) + taskPtr->getTailOffset();
                nextReq->setOutputs(taskPtr->outputs(_offsetPtr));
                nextReq->getData()->output_ptr = _offsetPtr;
            }
            nextReq->SetStatus(Request::Status::REQ_BUSY);
            nextReq->requestor_name() = taskPtr->name(); // Record which Task made the request
            _requests.push_back(nextReq);

            it->second = Status::TASK_BUSY; // Create request and change to BUSY

            TASK_FLOW_START("["+to_string(_jobId)+"]"+taskPtr->name()+"");
        } else {
            return; // Not READY, no request to process
        }
    }
    if (nextReq) {
        InferenceRequest(nextReq);
    }
}

InferenceJob::InferenceJob(int id) noexcept
{
    _jobId = id;
}

void InferenceJob::onAllRequestComplete()
{
    TASK_FLOW("["+to_string(_jobId)+"] ALL COMPLETE")
#ifdef USE_PROFILER
    _inferenceEnginePtr->getTimer()->UpdateLatencyStatistics(latency());
    _inferenceEnginePtr->getTimer()->UpdateInferenceTimeStatistics(inference_time());
    _inferenceEnginePtr->getTimer()->PushLatency(latency());
    _inferenceEnginePtr->getTimer()->PushInferenceTime(inference_time());
#endif

    if(_storeResult)
    {
        setReturnOutputs();
    }
    //for (auto it : _tensors) cout << it.first << "," << it.second << endl;
    if (_infEngCallback !=nullptr)
    {
        LOG_DXRT_DBG << "task callback" << endl;
        if (debug_all_output)
        {
            //std::thread([this](void){
                try {
                    TensorPtrs ret;
                    {
                        std::unique_lock<std::mutex> lk(_lock);
                        for (auto it : _tensors)
                        {
                            ret.emplace_back(
                                make_shared<Tensor>(it.second));
                        }
                    }
                    _infEngCallback(ret, _userArg, _jobId);  // callback registered by inference_engine

                    ReleaseAllOutputBuffer();
                    setStatus(Request::Status::REQ_DONE);
                   
                } catch (dxrt::Exception& e) {
                    e.printTrace();
                    LOG_DXRT << "callback error " << endl;
                }catch (std::exception& e) {
                    LOG_DXRT << e.what() << " std callback error " << endl;
                } catch (...) {
                    LOG_DXRT << "callback error unknown " << endl;
                }
            //}).detach();
        }
        else
        {
            DXRT_ASSERT(_doneCount.load() == _outputCount.load() , "output-count mismatch");
            //std::thread([this](void){
                try {
                    TensorPtrs ret;
                    {
                        std::unique_lock<std::mutex> lk(_lock);
                        for (auto &name : _outputs)
                        {
                            auto it = _tensors.find(name);
                            DXRT_ASSERT(it != _tensors.end(), "output name NOT FOUND" + name);
                            ret.emplace_back(
                                make_shared<Tensor>(it->second));
                        }
                    }
                    if (DEBUG_DATA > 0)
                    {
                        DataDumpBin("output.bin", ret);
                    }
                    _infEngCallback(ret, _userArg, _jobId);  // callback registered by inference_engine

                    ReleaseAllOutputBuffer();
                    setStatus(Request::Status::REQ_DONE);

                    
                } catch (dxrt::Exception& e) {
                    e.printTrace();
                    LOG_DXRT << "callback error " << endl;
                }catch (std::exception& e) {
                    LOG_DXRT << e.what() << " std callback error " << endl;
                } catch (...) {
                    LOG_DXRT << "callback error unknown " << endl;
                }
            //}).detach();
        }
    }
    else
    {
        ReleaseAllOutputBuffer();
        setStatus(Request::Status::REQ_DONE);
        
    }
}

void InferenceJob::SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_, std::vector<string> lastOutputOrder)
{
    Clear();
    _headTask = head_;
    _doneCount.store(0);
    _latency = 0;
    _infTime = 0;

    //_tasks.clear();
    _outputs.clear();
    _outputs = lastOutputOrder; 
    
    _taskStatusMap.clear();

    _outputCount.store(tasks_.size());
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
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }
}

int InferenceJob::startJob(void *inputPtr, void *userArg, void *outputPtr)
{
    TaskPtr task = _headTask.lock();
    if(!task)
    {
        return -1;
    }
    RequestPtr req = Request::Create(task.get(), inputPtr, outputPtr, userArg, _jobId);
    setStatus(Request::Status::REQ_BUSY);

    _userArg = userArg;
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->setInferenceJob(this);  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);
    _outputPtr = outputPtr;
    if((_headTask.lock()->nexts().empty()) && (_outputPtr != nullptr))
    {
        uint8_t* _offsetPtr = static_cast<uint8_t*>(_outputPtr) + task->getTailOffset();
        req->setOutputs(task->outputs(_offsetPtr));
        req->getData()->output_ptr = _offsetPtr;
    }
    else
        req->getData()->output_ptr = nullptr;
    //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //    cout<<"[PROC         ][Job_"<<_jobId<<"][Req_"<<req->id()<<"] Inference Request"<<endl;

    InferenceRequest(req);
    return _jobId;
}

// void InferenceJob::setReturnOutputs()
// {
//     TensorPtrs ret;

//     //int output_size_increment = 0;
//     for (auto &name : _outputs)
//     {
//         std::unique_lock<std::mutex> lock(_lock);
//         auto it = _tensors.find(name);
//         DXRT_ASSERT(it != _tensors.end(), "Failed to find tensor: setReturnOutputs");

//         auto output = it->second;
//         int output_size = output.elem_size();
        
//         for(int size: output.shape())
//         {
//             output_size *= size;
//         }
//         shared_ptr<vector<uint8_t> > memory = make_shared<vector<uint8_t> >(output_size);
//         memcpy(memory->data(), output.data(),output_size);
//         shared_ptr<Tensor> ptr =  std::shared_ptr<Tensor>(new Tensor(it->second,memory->data()),
//             [memory](Tensor* p){
//                 delete p;
//                 memory->clear();
//             });

//         ret.emplace_back(
//             make_shared<Tensor>(it->second));
//         //output_size_increment += output_size;
//     }
//     _returnOutputs = ret;
// }

void InferenceJob::setReturnOutputs()
{
    TensorPtrs ret_tensor_ptrs;

    //int output_size_increment = 0;
    for (const auto &name : _outputs)
    {
        std::unique_lock<std::mutex> lock(_lock);
        auto it = _tensors.find(name);
        DXRT_ASSERT(it != _tensors.end(), "Failed to find tensor: setReturnOutputs");

        auto &output_tensor = it->second;
        size_t output_size = output_tensor.elem_size();

        if (_outputPtr == nullptr)
        {
            for(int size: output_tensor.shape())
            {
                output_size *= size;
            }
            shared_ptr<vector<uint8_t> > memory = make_shared<vector<uint8_t> >(output_size);
            std::memcpy(memory->data(), output_tensor.data(),output_size);

            auto copied_tensor = std::make_shared<Tensor>(output_tensor, memory->data());
            copied_tensor.reset(new Tensor(*copied_tensor), [memory](Tensor* p) {
                    delete p;
                    memory->clear();
                });

            ret_tensor_ptrs.push_back(copied_tensor);
        }
        else
        {
            ret_tensor_ptrs.push_back(std::make_shared<Tensor>(output_tensor));
        }
    }

    //std::cout << "InferenceJob::setReturnOutputs output-tensor-ptrs-size=" << ret_tensor_ptrs.size() << std::endl;
    _returnOutputs = ret_tensor_ptrs;
}

TensorPtrs InferenceJob::getOutput()
{
    return std::move(_returnOutputs);
}

void InferenceJob::SetStoreResult(bool storeResult)
{ 
    _storeResult = storeResult;
}

void InferenceJob::setInferenceEngineInterface(InferenceEngine* ptr)
{
    _inferenceEnginePtr = ptr;
}

void InferenceJob::setCallBack(std::function<int(TensorPtrs &outputs, void *userArg, int jobId)> func)
{
    std::unique_lock<std::mutex> lk(_lock);
    _infEngCallback = func;
}

void InferenceJob::Clear()
{
    std::unique_lock<std::mutex> lk(_lock);
    //_requests.clear();
    _tensors.clear();
    //_tasks.clear();
    //_head.reset();
    //_headTask.reset();
    setStatus(Request::Status::REQ_IDLE);
    _outputCount.store(0);
    _doneCount.store(0);
    //_outputs.clear();
    _userArg = nullptr;
    _latency = 0;
    _infTime = 0;
    _inferenceEnginePtr = nullptr;
    _infEngCallback = nullptr;
    _outputPtr = nullptr;
    _storeResult = false;

    _occupiedJob.store(false);
}

InferenceJob::~InferenceJob()
{
    Clear();
}

void InferenceJob::ReleaseAllOutputBuffer()
{
    
    std::unique_lock<std::mutex> lk(_lock);
    int head_req_id = -1;
    int head_req_processed_dev_id = -1;
    for (auto& req_weak_ptr :  _requests)
    {
        RequestPtr req = req_weak_ptr.lock();
        if (req)
        {

            if (DEBUG_DATA > 0 && req->task()->processor()==Processor::CPU)
            {
                int id = req->id();
                DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
                DataDumpBin(req->task()->name() + "_output_done.bin", &id, 1);
            }
            if((_outputPtr == nullptr) || (req->task()->is_tail()==false))
            {
                req->task()->ReleaseOutputBuffer(req->output_ptr());
                //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
                //    cout<<"[        OUT_W][Job_"<<_jobId<<"][Req_"<<req->id()<<"]{xxx_"<<req->getData()->_processedDevId<<"}{xxxxxx} Buffer Released"<<endl;
            }
        }
        else
        {
            DXRT_ASSERT(false, "ReleaseAllOutputBuffer lock failed");
        }
    }
    for (auto& it : _requests)
    {
        RequestPtr req = it.lock();
        if (head_req_id == -1)
        {
            head_req_id = req->id();
            head_req_processed_dev_id = req->getData()->_processedDevId;
        }
        if (req)
        {
            req->Reset();
        }
        else
        {
            DXRT_ASSERT(false, "ReleaseAllOutputBuffer lock failed");
        }
    }
    _requests.clear();
    _use_flag.store(false);
    //if(head_req_id%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || head_req_id%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //    cout<<"[        OUT_W][Job_"<<_jobId<<"]{xxx_"<<head_req_id<<"}{xxx_"<<head_req_processed_dev_id<<"}{xxxxxx} use_flag reset"<<endl;
    (void)head_req_processed_dev_id; // avoid 'not used' warning

}

void InferenceJob::setStatus(Request::Status status)
{
    std::unique_lock<std::mutex> lock(_waitMutex);
    _status.store(status);
    _waitCV.notify_one();
}

int InferenceJob::getId()
{
    return _jobId;
}
Request::Status InferenceJob::getStatus()
{
    return _status.load();
}

void InferenceJob::Wait()
{
    std::unique_lock<std::mutex> lock(_waitMutex);
    _waitCV.wait(lock, [this]{ return _status.load() != Request::Status::REQ_BUSY; });
}

}  // namespace dxrt
