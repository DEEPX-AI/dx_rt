/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

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
#include <cstring>
#include <iostream>

using std::endl;
using std::to_string;


namespace dxrt
{

bool debug_all_output = false;

// Build user-buffer-mapped output tensors for a tail task
static Tensors BuildUserOutputTensorsForTailTask(
    const TaskPtr& taskPtr,
    void* userOutputBase,
    const std::vector<std::string>& outputsOrder,
    InferenceEngine* inferenceEngine,
    int jobId)
{
    std::ignore = jobId;
    Tensors outputTensors;
    if (userOutputBase == nullptr || inferenceEngine == nullptr) {
        return outputTensors;
    }

    for (const auto& tensor : taskPtr->outputs())
    {
        auto it = std::find(outputsOrder.begin(), outputsOrder.end(), tensor.name());
        if (it != outputsOrder.end())
        {
            // Calculate offset for each tensor in the full user output buffer
            size_t tensorOffset = inferenceEngine->GetOutputTensorOffset(tensor.name());
            uint8_t* tensorPtr = static_cast<uint8_t*>(userOutputBase) + tensorOffset;

            Tensor outputTensor = tensor;  // copy metadata
            outputTensor.data() = tensorPtr; // map to user buffer
            outputTensors.push_back(outputTensor);

            LOG_DBG("[Job_" + std::to_string(jobId) + "] Task '" + taskPtr->name() +
                    "' tensor '" + tensor.name() + "' at offset: " + std::to_string(tensorOffset));
        }
    }

    return outputTensors;
}


void InferenceJob::onRequestComplete(RequestPtr req)
{
    bool allRequestComplete = false;
    Task* thisTask = req->task();

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] onRequestComplete: Task '" + thisTask->name() +
            "' completed. Processor: " + (thisTask->processor() == Processor::NPU ? "NPU" : "CPU") +
            ", is_tail: " + (thisTask->is_tail() ? "true" : "false"));

    {
        std::unique_lock<std::mutex> lk(_lock);

        // 1. _tensors update with thread-safe user buffer handling
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Adding " + std::to_string(req->outputs().size()) +
                " output tensors from task '" + thisTask->name() + "'");

        for (const Tensor& output : req->outputs()) {
            auto name = output.name();
            auto it = _tensors.find(name);
            if (it != _tensors.end()) {
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Overwriting existing tensor: " + name +
                              " from task: " + thisTask->name());
                it->second = output;
            } else {
                _tensors.insert(make_pair(name, output));
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Added tensor: " + name +
                        " from task: " + thisTask->name());
            }
        }

        _doneCount++;
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' done. Progress: " + std::to_string(_doneCount.load()) + "/" + std::to_string(_outputCount.load()));

        if (_doneCount.load() == _outputCount.load())
        {
            allRequestComplete = true;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] All tasks completed!");
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
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' is not tail. Processing " + std::to_string(thisTask->nexts().size()) + " next tasks");

        for (auto& nextTaskPtr : thisTask->nexts())
        {
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Checking readiness of next task: " + nextTaskPtr->name());

            // 2. _taskStatusMap update (TASK_IDLE -> TASK_READY)
            if (checkAndSetTaskReady(nextTaskPtr))
            {
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + nextTaskPtr->name() + "' is ready. Starting...");
                // 3. ready task inference request
                processReadyTask(nextTaskPtr);
            }
            else
            {
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + nextTaskPtr->name() + "' is not ready yet");
            }
        }
    }
    else
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() + "' is tail task");
        TASK_FLOW_FINISH("["+to_string(_jobId)+"]"+thisTask->name()+" (Tail Task)");
        if (allRequestComplete)
        {
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] All requests complete. Calling onAllRequestComplete()");
            onAllRequestComplete();
        }
        else
        {
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Tail task complete but not all requests done yet. " +
                    "Waiting for remaining tasks...");
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

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] checkAndSetTaskReady: Task '" + taskPtr->name() +
            "' current status: " + (it->second == Status::TASK_IDLE ? "IDLE" :
                                  it->second == Status::TASK_READY ? "READY" :
                                  it->second == Status::TASK_BUSY ? "BUSY" : "DONE"));

    if (it->second == Status::TASK_IDLE)
    {
        auto required_tensors = taskPtr->inputs();
        bool allPrepared = true;

        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() +
                "' requires " + std::to_string(required_tensors.size()) + " input tensors");

        std::vector<std::string> missing_inputs;
        for (const auto& required : required_tensors)
        {
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Checking required input: " + required.name());

            if (_tensors.find(required.name()) == _tensors.end())
            {
                allPrepared = false;
                missing_inputs.push_back(required.name());
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Missing input tensor: " + required.name());
            }
            else
            {
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Found input tensor: " + required.name());
            }
        }

        if (allPrepared)
        {
            it->second = Status::TASK_READY;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() +
                    "' is now READY (all inputs available)");
            return true;
        }
        else
        {
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() +
                    "' is NOT ready. Missing inputs: ");
            for (const auto& missing : missing_inputs)
            {
                std::ignore = missing;
                LOG_DBG("  - " + missing);
            }

            // Log currently available tensors for debugging
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Currently available tensors:");
            for (const auto& pair : _tensors)
            {
                std::ignore = pair;
                LOG_DBG("  + " + pair.first);
            }
        }
    }
    else
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() +
                "' is not in IDLE state, cannot change to READY");
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
            Tensors nextInputTensors;
            auto required_tensors = taskPtr->inputs();
            for (const auto& required : required_tensors)
            {
                auto tensor_it = _tensors.find(required.name());
                if (tensor_it == _tensors.end()) {
                    // This should ideally not happen if checkAndSetTaskReady worked correctly
                    LOG_DXRT_ERR("Required tensor '" + required.name() + "' not found for task '" + taskPtr->name() + "'");
                    return;
                }
                nextInputTensors.push_back(tensor_it->second);  // Call this function only when allPrepared is true
            }

            nextReq = Request::Create(taskPtr.get(), nextInputTensors, {}, _userArg, _jobId);
            nextReq->setInferenceJob(this);

            // For multi-tail models, only allocate user buffer for final output tensors
            if (_outputPtr != nullptr)
            {
                // To avoid intermediate copies, use user-provided buffer only for pure tail tasks
                if (taskPtr->is_tail() && taskPtr->processor() == Processor::CPU)
                {
                    Tensors outputTensors = BuildUserOutputTensorsForTailTask(taskPtr, _outputPtr, _outputs, _inferenceEnginePtr, _jobId);
                    nextReq->setOutputs(outputTensors);
                    // Set the first address of continuous memory to outputs_ptr
                    if (!outputTensors.empty()) {
                        size_t firstTensorOffset = _inferenceEnginePtr->GetOutputTensorOffset(outputTensors[0].name());
                        uint8_t* firstTensorPtr = static_cast<uint8_t*>(outputTensors[0].data());
                        uint8_t* basePtr = firstTensorPtr - firstTensorOffset;
                        nextReq->getData()->output_buffer_base = basePtr;
                        nextReq->getData()->outputs_is_user_buffer = true;
                    } else {
                        nextReq->getData()->output_buffer_base = nullptr;
                        nextReq->getData()->outputs_is_user_buffer = false;
                    }
                    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' (CPU tail) using user output buffer directly");
                }
                else
                {
                    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' uses internal buffer (not a pure CPU tail task)");
                }
            }
            nextReq->SetStatus(Request::Status::REQ_BUSY);
            nextReq->DSP_SetDspEnable(0);
            nextReq->requestor_name() = taskPtr->name();  // Record which Task made the request
            _requests.push_back(nextReq);

            it->second = Status::TASK_BUSY;  // Create request and change to BUSY

            TASK_FLOW_START("["+to_string(_jobId)+"]"+taskPtr->name()+"");
        }
        else
        {
            return;  // Not READY, no request to process
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
#ifdef USE_PROFILER
    _inferenceEnginePtr->getTimer()->UpdateLatencyStatistics(latency());
    _inferenceEnginePtr->getTimer()->UpdateInferenceTimeStatistics(inference_time());
    _inferenceEnginePtr->getTimer()->PushLatency(latency());
    _inferenceEnginePtr->getTimer()->PushInferenceTime(inference_time());
#endif

    if (_storeResult)
    {
        setReturnOutputs();
    }
    // for (auto it : _tensors) cout << it.first << "," << it.second << endl;
    if (_infEngCallback !=nullptr)
    {
        LOG_DXRT_DBG << "task callback" << endl;
        if (debug_all_output)
        {
            // std::thread([this](void){
                try {
                    TensorPtrs ret;
                    {
                        std::unique_lock<std::mutex> lk(_lock);
                        for (auto it : _tensors)
                        {
                            ret.emplace_back(
                                std::make_shared<Tensor>(it.second));
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
            // }).detach();
        }
        else
        {
            DXRT_ASSERT(_doneCount.load() == _outputCount.load() , "output-count mismatch");
            // std::thread([this](void){
                try {
                    TensorPtrs ret;
                    {
                        std::unique_lock<std::mutex> lk(_lock);
                        for (auto &name : _outputs)
                        {
                            auto it = _tensors.find(name);
                            DXRT_ASSERT(it != _tensors.end(), "output name NOT FOUND" + name);
                            ret.emplace_back(
                                std::make_shared<Tensor>(it->second));
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
            // }).detach();
        }
    }
    else
    {
        ReleaseAllOutputBuffer();
        setStatus(Request::Status::REQ_DONE);

    }
    TASK_FLOW("["+to_string(_jobId)+"] ALL COMPLETE");

}

void InferenceJob::SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_, std::vector<string> lastOutputOrder)
{
    Clear();
    _headTask = head_;
    _doneCount.store(0);
    _latency = 0;
    _infTime = 0;

    _tasks = tasks_;  // Store tasks for multi-input support
    _outputs.clear();
    _outputs = lastOutputOrder;

    _taskStatusMap.clear();

    _outputCount.store(tasks_.size());
    for (std::shared_ptr<Task>& it :  tasks_)
    {
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }
}

void InferenceJob::SetInferenceJobMultiHead(std::vector<std::shared_ptr<Task>>& tasks_,
                                           const std::vector<std::shared_ptr<Task>>& inputTasks_,
                                           std::vector<string> lastOutputOrder)
{
    Clear();
    _isMultiHead = true;
    _inputTasks = inputTasks_;
    _doneCount.store(0);
    _latency = 0;
    _infTime = 0;

    _tasks = tasks_;  // Store tasks for multi-input support
    _outputs.clear();
    _outputs = lastOutputOrder;

    _taskStatusMap.clear();

    _outputCount.store(tasks_.size());
    for (std::shared_ptr<Task>& it :  tasks_)
    {
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }

    LOG_DBG("[MULTI_HEAD] Set inference job with " + std::to_string(inputTasks_.size()) + " input tasks");
}

int InferenceJob::startJob(void *inputPtr, void *userArg, void *outputPtr)
{
    TaskPtr task = _headTask.lock();
    if (task == nullptr)
    {
        return -1;
    }
    RequestPtr req = Request::Create(task.get(), inputPtr, outputPtr, userArg, _jobId);
    setStatus(Request::Status::REQ_BUSY);

    _userArg = userArg;
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->DSP_SetDspEnable(0);
    req->setInferenceJob(this);  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);
    _outputPtr = outputPtr;
    if (_outputPtr != nullptr)
    {
        // To avoid intermediate copies, use user-provided buffer only for pure tail tasks
        if (task->is_tail())
        {
            // Map all tail-task outputs to user buffer with model-global offsets
            Tensors outputTensors = BuildUserOutputTensorsForTailTask(task, _outputPtr, _outputs, _inferenceEnginePtr, _jobId);
            req->setOutputs(outputTensors);
            if (!outputTensors.empty()) {
                size_t firstTensorOffset = _inferenceEnginePtr->GetOutputTensorOffset(outputTensors[0].name());
                uint8_t* firstTensorPtr = static_cast<uint8_t*>(outputTensors[0].data());
                uint8_t* basePtr = firstTensorPtr - firstTensorOffset;
                req->getData()->output_buffer_base = basePtr;
                req->getData()->outputs_is_user_buffer = true;
            } else {
            req->getData()->output_buffer_base = nullptr;
                req->getData()->outputs_is_user_buffer = false;
            }
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + task->name() + "' is tail task, using user output buffer directly");
        }
        else
        {
            req->getData()->output_buffer_base = nullptr;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + task->name() + "' uses internal buffer (not a pure tail task)");
        }
    }
    else
        req->getData()->output_buffer_base = nullptr;
    // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //    cout<<"[PROC         ][Job_"<<_jobId<<"][Req_"<<req->id()<<"] Inference Request"<<endl;

    InferenceRequest(req);
    return _jobId;
}

int InferenceJob::startMultiInputJob(const std::map<std::string, void*>& inputTensors, void *userArg, void *outputPtr)
{
    setStatus(Request::Status::REQ_BUSY);
    _userArg = userArg;
    _outputPtr = outputPtr;

    {
        std::unique_lock<std::mutex> lk(_lock);

        // Add input tensors to _tensors for processing
        for (const auto& pair : inputTensors)
        {
            const std::string& tensorName = pair.first;
            void* tensorData = pair.second;

            // Find the corresponding task and get tensor information
            TaskPtr targetTask = nullptr;
            for (auto& task : _tasks)
            {
                auto inputs = task->inputs();
                for (const auto& input : inputs)
                {
                    if (input.name() == tensorName)
                    {
                        targetTask = task;
                        break;
                    }
                }
                if (targetTask) break;
            }

            if (targetTask)
            {
                // Get the actual tensor information from the task
                auto taskInputs = targetTask->inputs();
                for (auto& input : taskInputs)
                {
                    if (input.name() == tensorName)
                    {
                        // Create tensor with correct information but using provided data pointer
                        Tensor inputTensor(input.name(), input.shape(), input.type(), tensorData);
                        inputTensor.phy_addr() = 0;  // Physical address not available for user-provided data
                        _tensors.insert(std::make_pair(tensorName, inputTensor));

                        LOG_DBG("[MULTI_INPUT][Job_" + std::to_string(_jobId) + "] Added input tensor: " + tensorName);
                        break;
                    }
                }
            }
            else
            {
                // Fallback: create basic tensor if we can't find the task
                Tensor inputTensor(tensorName, {}, DataType::FLOAT, tensorData);
                _tensors.insert(std::make_pair(tensorName, inputTensor));

                LOG_DBG("[MULTI_INPUT][Job_" + std::to_string(_jobId) + "] Added input tensor (fallback): " + tensorName);
            }
        }
    }

    // Find and start all ready tasks (tasks that have all their inputs available)
    for (const auto& pair : _taskStatusMap)
    {
        const auto& taskName = pair.first;
        const auto& status = pair.second;

        if (status == Status::TASK_IDLE)
        {
            // Find the task pointer
            TaskPtr taskPtr = nullptr;
            for (auto& task : _tasks)
            {
                if (task->name() == taskName)
                {
                    taskPtr = task;
                    break;
                }
            }

            if (taskPtr && checkAndSetTaskReady(taskPtr))
            {
                processReadyTask(taskPtr);
            }
        }
    }

    return _jobId;
}

void InferenceJob::setReturnOutputs()
{
    TensorPtrs ret_tensor_ptrs;

    // Log the current state for debugging
    LOG_DBG("[Job_" + std::to_string(_jobId) + "] setReturnOutputs: Expected outputs count: " + std::to_string(_outputs.size()));
    LOG_DBG("[Job_" + std::to_string(_jobId) + "] setReturnOutputs: Available tensors count: " + std::to_string(_tensors.size()));

    // Log available tensors for debugging
    {
        std::unique_lock<std::mutex> lock(_lock);
        for (const auto& pair : _tensors)
        {
            std::ignore = pair;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Available tensor: " + pair.first);
        }
    }

    std::vector<std::string> missing_tensors;

    for (const auto &name : _outputs)
    {
        std::unique_lock<std::mutex> lock(_lock);
        auto it = _tensors.find(name);

        if (it == _tensors.end())
        {
            // Tensor not found - collect missing tensors for better error reporting
            missing_tensors.push_back(name);
            LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Missing expected output tensor: " + name);
            continue;
        }

        auto &output_tensor = it->second;
        size_t output_size = 0;

        if (_outputPtr == nullptr)
        {
            output_size = output_tensor.size_in_bytes();
            std::shared_ptr<std::vector<uint8_t> > memory = std::make_shared<std::vector<uint8_t> >(output_size);
            memcpy(memory->data(), output_tensor.data(), output_size);

            auto copied_tensor = std::make_shared<Tensor>(output_tensor, memory->data());
            copied_tensor.reset(new Tensor(*copied_tensor), [memory](Tensor* p) {
                    delete p;
                    memory->clear();
                });

            ret_tensor_ptrs.push_back(copied_tensor);
        }
        else
        {
            // User provided an output buffer. We need to copy the result into it.
            size_t tensorOffset = _inferenceEnginePtr->GetOutputTensorOffset(name);
            uint8_t* dest_ptr = static_cast<uint8_t*>(_outputPtr) + tensorOffset;

            void* src_ptr = output_tensor.data();
            size_t tensor_size = output_tensor.size_in_bytes();

            if (src_ptr != nullptr && dest_ptr != src_ptr)
            {
                std::memcpy(dest_ptr, src_ptr, tensor_size);
                
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Thread-safe copy: " + name + 
                        " to offset " + std::to_string(tensorOffset) + 
                        " (size: " + std::to_string(tensor_size) + " bytes)");
            }

            // Create a new Tensor object for the return list that correctly points to the user's buffer.
            auto final_tensor = std::make_shared<Tensor>(output_tensor);  // Copy metadata
            final_tensor->data() = dest_ptr;  // Update pointer to user's buffer
            ret_tensor_ptrs.push_back(final_tensor);
        }

        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Found output tensor: " + name +
                " shape: [" + std::to_string(output_tensor.shape().size()) + "] " +
                " size: " + std::to_string(output_size));
    }

    // Handle missing tensors
    if (!missing_tensors.empty())
    {
        std::string error_msg = "[Job_" + std::to_string(_jobId) + "] Failed to find output tensors: ";
        for (size_t i = 0; i < missing_tensors.size(); ++i)
        {
            error_msg += missing_tensors[i];
            if (i < missing_tensors.size() - 1) error_msg += ", ";
        }
        error_msg += ". Available tensors: ";
        {
            std::unique_lock<std::mutex> lock(_lock);
            for (const auto& pair : _tensors)
            {
                error_msg += pair.first + " ";
            }
        }

        LOG_DXRT_ERR(error_msg);

        // Instead of ASSERT which causes deadlock, throw an exception
        throw InvalidOperationException(error_msg);
    }

    _returnOutputs = ret_tensor_ptrs;
    LOG_DBG("[Job_" + std::to_string(_jobId) + "] setReturnOutputs completed successfully with " +
            std::to_string(ret_tensor_ptrs.size()) + " output tensors");
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
    // _requests.clear();
    _tensors.clear();
    _tasks.clear();  // Clear tasks for multi-input support
    // _head.reset();
    // _headTask.reset();
    setStatus(Request::Status::REQ_IDLE);
    _outputCount.store(0);
    _doneCount.store(0);
    _isDsp.store(0);
    // _outputs.clear();
    _userArg = nullptr;
    _latency = 0;
    _infTime = 0;
    _inferenceEnginePtr = nullptr;
    _infEngCallback = nullptr;
    _outputPtr = nullptr;
    _storeResult = false;

    // Clear multi-head support variables
    _inputTasks.clear();
    _isMultiHead = false;

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
            if (DEBUG_DATA > 0 && req->task()->processor() == Processor::CPU)
            {
                int id = req->id();
                DataDumpBin(req->task()->name() + "_output.bin", req->outputs());
                DataDumpBin(req->task()->name() + "_output_done.bin", &id, 1);
            }

            // requests that use BufferSet are already released in Request::releaseBuffers()
            // conditional release to avoid duplicate releases
            if (req->isBufferReleased())
            {
                LOG_DXRT_DBG << "Request " << req->id() << " already released - skipping" << std::endl;
            }
            else if (req->hasBufferSet())
            {
                LOG_DXRT_DBG << "Request " << req->id() << " has BufferSet - skipping individual buffer release" << std::endl;
            }
            else
            {
                LOG_DXRT_DBG << "Request " << req->id() << " no BufferSet - using individual buffer release" << std::endl;

                // Check if this task uses user output buffer
                bool usesUserOutputBuffer = req->getData()->outputs_is_user_buffer;
                if (!usesUserOutputBuffer && _outputPtr != nullptr && req->output_buffer_base() != nullptr)
                {
                    // Fallback range check (legacy)
                    uint8_t* userBufferStart = static_cast<uint8_t*>(_outputPtr);
                    uint8_t* userBufferEnd = userBufferStart + _inferenceEnginePtr->GetOutputSize();
                    uint8_t* outputPtr = static_cast<uint8_t*>(req->output_buffer_base());
                    if (outputPtr >= userBufferStart && outputPtr < userBufferEnd)
                    {
                        usesUserOutputBuffer = true;
                        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + req->task()->name() +
                                "' uses user output buffer - skipping ReleaseOutputBuffer (range-detected)");
                    }
                }

                // Only call ReleaseOutputBuffer if not using user output buffer
                if (!usesUserOutputBuffer && ((_outputPtr == nullptr) || (req->task()->is_tail() == false)))
                {
                    req->task()->ReleaseOutputBuffer(req->output_buffer_base());
                    // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
                    //    cout<<"[        OUT_W][Job_"<<_jobId<<"][Req_"<<req->id()<<"]{xxx_"<<req->getData()->_processedDevId<<"}{xxxxxx} Buffer Released"<<endl;
                }
                if (req->task()->processor() == Processor::NPU)
                {
                    req->task()->ReleaseEncodedInputBuffer(req->encoded_inputs_ptr());
                    req->task()->ReleaseEncodedOutputBuffer(req->encoded_outputs_ptr());
                }
                req->markBufferReleased();
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
    // if(head_req_id%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || head_req_id%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //    cout<<"[        OUT_W][Job_"<<_jobId<<"]{xxx_"<<head_req_id<<"}{xxx_"<<head_req_processed_dev_id<<"}{xxxxxx} use_flag reset"<<endl;
    (void)head_req_processed_dev_id;  // avoid 'not used' warning
    TASK_FLOW("[" + to_string(_jobId)+"] ReleaseAllOutputBuffer");
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

// DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////

void InferenceJob::DSP_OnRequestComplete(RequestPtr req)
{
    _dspOutputPtr = req->getData()->output_buffer_base;

    LOG_DXRT_DBG << "outputAddrDsp " << std::hex << (uint64_t)_dspOutputPtr << endl;

    setStatus(Request::Status::REQ_DONE);
}

//std::function<void(RequestPtr)> InferenceJob::DSP_onRequestCompleteFunction()
//{
//    return [this](RequestPtr req) {
//        DSP_OnRequestComplete(req);
//    };
//}

int InferenceJob::DSP_StartJob(dxrt_dspcvmat_t *dspCvMatInPtr, dxrt_dspcvmat_t *dspCvMatOutPtr, void *userArg)
{
    TaskPtr task = _headTask.lock();
    if (task == nullptr)
    {
        LOG_DXRT_DBG << "can't get task " << endl;
        return -1;
    }

    void *inputPtr  = reinterpret_cast<void*>(dspCvMatInPtr->data);
    void *outputPtr = reinterpret_cast<void*>(dspCvMatOutPtr->data);

    RequestPtr req = Request::Create(task.get(), inputPtr, outputPtr, userArg, _jobId);
    setStatus(Request::Status::REQ_BUSY);

    _userArg = userArg;
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->DSP_SetDspEnable(1);
    // req->setCallback(DSP_onRequestCompleteFunction());  // on each request complete, do next request or complete whole inference
    req->setInferenceJob(this);  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);

    DSP_ProcRequest(req, dspCvMatInPtr, dspCvMatOutPtr);
    return _jobId;
}

// ~DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////



}  // namespace dxrt
