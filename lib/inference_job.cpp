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
#include "dxrt/cpu_handle.h"
#include "dxrt/request_response_class.h"

#include <future>
#include <memory>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <fstream>

using std::endl;
using std::to_string;


namespace dxrt
{

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
    LOG_DXRT_DBG << "onRequestComplete(job=" << _jobId << ", task=" << req->task()->name() << ")" << std::endl;

    bool allRequestComplete = false;
    Task* thisTask = req->task();

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] onRequestComplete: Task '" + thisTask->name() +
            "' completed. Processor: " + (thisTask->processor() == Processor::NPU ? "NPU" : "CPU") +
            ", is_tail: " + (thisTask->is_tail() ? "true" : "false"));

    {
        std::unique_lock<std::mutex> lk(_lock);
        // Update tensor map
        for (const Tensor& output : req->outputs()) {
            auto name = output.name();
            auto itTensor = _tensors.find(name);
            if (itTensor != _tensors.end()) itTensor->second = output; else _tensors.insert(make_pair(name, output));
        }
        // Mark task done
        auto itStatus = _taskStatusMap.find(thisTask->name());
        if (itStatus == _taskStatusMap.end()) {
            throw InvalidOperationException(EXCEPTION_MESSAGE("The task name was not found in this job."));
        }
        itStatus->second = Status::TASK_DONE;
        TASK_FLOW_FINISH("[" + to_string(_jobId) + "]" + thisTask->name());

        // Check if all requests complete - use original logic with task count
        _doneCount++;
        allRequestComplete = (_doneCount.load() == _outputCount.load());

        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' done. Progress: " + std::to_string(_doneCount.load()) + "/" + std::to_string(_outputCount.load()));

        _latency += req->latency();
        if (req->task()->processor() == Processor::NPU)
            _infTime += req->inference_time();
    }

    if (!thisTask->nexts().empty())
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' has " + std::to_string(thisTask->nexts().size()) + " successor(s). Processing...");

        for (auto & nextTaskPtr : thisTask->nexts())
        {
            if (checkAndSetTaskReady(nextTaskPtr))
            {
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + nextTaskPtr->name() + "' is ready. Starting...");
                processReadyTask(nextTaskPtr);
            }
            else
            {
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + nextTaskPtr->name() + "' is not ready yet");
            }
        }
        // Note: allRequestComplete status doesn't change when scheduling successors
        // because completion is based on original task count, not dynamic request count
    }

    if (allRequestComplete) {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] All tasks completed! Calling onAllRequestComplete()");
        onAllRequestComplete();
    }
}

InferenceJob::InferenceJob(int id) noexcept
{
    _jobId = id;
}

void InferenceJob::onAllRequestComplete()
{
    LOG_DXRT_DBG << "onAllRequestComplete(job=" << _jobId << ")" << std::endl;

#ifdef USE_PROFILER
    _inferenceEnginePtr->getTimer()->UpdateLatencyStatistics(latency());
    _inferenceEnginePtr->getTimer()->UpdateInferenceTimeStatistics(inference_time());
    _inferenceEnginePtr->getTimer()->PushLatency(latency());
    _inferenceEnginePtr->getTimer()->PushInferenceTime(inference_time());
#endif

    // Dynamic output processing is now handled immediately in onRequestComplete()
    // No special processing needed here as _tensors already contains correct dynamic tensors

    if (_storeResult)
    {
        // Build _returnOutputs which contains only final model outputs (ordered)
        setReturnOutputs();
    }

    // Execute callback with final model outputs only (filtered from _tensors)
    try
    {
        LOG_DXRT_DBG << "task callback" << endl;
        TensorPtrs callbackOutputs;
        if (_storeResult) {
            callbackOutputs = _returnOutputs;
        } else {
            std::unique_lock<std::mutex> lk(_lock);
            for (const auto &name : _outputs) {
                auto it = _tensors.find(name);
                if (it != _tensors.end()) {
                    callbackOutputs.emplace_back(std::make_shared<Tensor>(it->second));
                } else {
                    LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Missing expected output tensor during callback: " + name);
                }
            }
        }

        if (DEBUG_DATA > 0)
        {
            DataDumpBin("output.bin", callbackOutputs);
        }
        _inferenceEnginePtr->onInferenceComplete(callbackOutputs, _userArg, _jobId);
        if (_infEngCallback != nullptr)
        {
            _infEngCallback(callbackOutputs, _userArg, _jobId);
        }
    } catch (dxrt::Exception& e) {
        e.printTrace();
        LOG_DXRT << "callback error " << endl;
    } catch (std::exception& e) {
        LOG_DXRT << e.what() << " std callback error " << endl;
    } catch (...) {
        LOG_DXRT << "callback error unknown " << endl;
    }

    // Release buffers and update job status regardless of callback presence or failures.
    ReleaseAllOutputBuffer();
    setStatus(Request::Status::REQ_DONE);

    TASK_FLOW("["+to_string(_jobId)+"] ALL COMPLETE");

}

void InferenceJob::SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_,
                                  std::vector<std::string> lastOutputOrder,
                                  const std::vector<std::string>& modelInputNames)
{
    Clear();
    _headTask = head_;
    _doneCount.store(0);
    _latency = 0;
    _infTime = 0;

    _tasks = tasks_;  // Store tasks for multi-input support
    _outputs.clear();
    _outputs = lastOutputOrder;
    _modelInputNames = modelInputNames;

    _taskStatusMap.clear();

    _outputCount.store(tasks_.size());
    for (std::shared_ptr<Task>& it :  tasks_)
    {
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }
}

void InferenceJob::SetInferenceJobMultiHead(std::vector<std::shared_ptr<Task>>& tasks_,
                                           const std::vector<std::shared_ptr<Task>>& inputTasks_,
                                           std::vector<std::string> lastOutputOrder,
                                           const std::vector<std::string>& modelInputNames)
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
    _modelInputNames = modelInputNames;

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

    setStatus(Request::Status::REQ_BUSY);
    _userArg = userArg;
    _outputPtr = outputPtr;

    // For multi-head models where model inputs are shared, add model input tensors to _tensors
    // Use the model input names provided by InferenceEngine (from graph_info)
    // Note: startJob() is for single-input models. If multiple model inputs exist,
    // startMultiInputJob() should be used instead. Here we handle the special case
    // where a single model input is shared across multiple tasks.
    {
        std::unique_lock<std::mutex> lk(_lock);

        // Sanity check: startJob() should only be called for single-input models
        if (_modelInputNames.size() > 1) {
            LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] WARNING: startJob() called with " +
                         std::to_string(_modelInputNames.size()) +
                         " model inputs. Should use startMultiInputJob() instead!");
        }

        // Find all model input tensors that are used by tasks other than the primary head
        // For each model input, check if any non-primary-head task uses it
        for (const auto& modelInputName : _modelInputNames) {
            // Find tensor metadata and check if it's shared
            bool isSharedInput = false;
            std::shared_ptr<Tensor> modelInputTensorPtr;  // Use shared_ptr to safely store tensor

            for (const auto& t : _tasks) {
                const Tensors& taskInputs = t->inputs();
                for (const auto& input : taskInputs) {
                    if (input.name() == modelInputName) {
                        // Save the first occurrence of this tensor's metadata
                        if (!modelInputTensorPtr) {
                            modelInputTensorPtr = std::make_shared<Tensor>(input);  // Copy the tensor
                        }

                        // If this task is not the primary head, this is a shared input
                        if (t != task) {
                            isSharedInput = true;
                            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Model input '" + modelInputName +
                                    "' is used by non-primary task '" + t->name() + "'");
                        }
                        break;  // Found this input in current task, move to next task
                    }
                }
                // Continue checking all tasks (don't break early) to log all shared usage
            }

            if (isSharedInput && modelInputTensorPtr) {
                // This is a shared model input, add it to _tensors with the provided data pointer
                Tensor modelInputTensor = *modelInputTensorPtr;  // Copy metadata
                modelInputTensor.data() = inputPtr;
                modelInputTensor.phy_addr() = 0;
                _tensors.insert(std::make_pair(modelInputName, modelInputTensor));
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Added shared model input tensor: " + modelInputName);
            }
        }
    }

    void* first_output = nullptr;
    if (task->is_tail())
    {
        first_output = outputPtr;
    }

    RequestPtr req = Request::Create(task.get(), inputPtr, first_output, userArg, _jobId);
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->setInferenceJob(this);  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);

    if (_outputPtr != nullptr) {
        // To avoid intermediate copies, use user-provided buffer only for pure tail tasks
        if (task->is_tail()) {
            // Map all tail-task outputs to user buffer with model-global offsets
            Tensors outputTensors = BuildUserOutputTensorsForTailTask(task, _outputPtr, _outputs, _inferenceEnginePtr, _jobId);
            req->setOutputs(outputTensors);
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + task->name() + "' is tail task, using user output buffer directly");
        } else {
            req->getData()->output_buffer_base = nullptr;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + task->name() + "' uses internal buffer (not a pure tail task)");
        }
    } else {
        req->getData()->output_buffer_base = nullptr;
    }

    // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    RequestResponse::InferenceRequest(req);

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
                    // Fallback range check (legacy) - skip for dynamic shape models
                    uint64_t outputSize = _inferenceEnginePtr->GetOutputSize();
                    if (outputSize != static_cast<uint64_t>(-1)) {
                        uint8_t* userBufferStart = static_cast<uint8_t*>(_outputPtr);
                        uint8_t* userBufferEnd = userBufferStart + outputSize;
                        uint8_t* outputPtr = static_cast<uint8_t*>(req->output_buffer_base());
                        if (outputPtr >= userBufferStart && outputPtr < userBufferEnd)
                        {
                            usesUserOutputBuffer = true;
                            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + req->task()->name() +
                                    "' uses user output buffer - skipping ReleaseOutputBuffer (range-detected)");
                        }
                    } else {
                        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Skipping range check for dynamic shape model");
                    }
                }

                // Only call ReleaseOutputBuffer if not using user output buffer
                if (!usesUserOutputBuffer && ((_outputPtr == nullptr) || (req->task()->is_tail() == false)))
                {
                    req->task()->ReleaseOutputBuffer(req->output_buffer_base());
                    // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)

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

bool InferenceJob::checkAndSetTaskReady(TaskPtr taskPtr)
{
    if (!taskPtr) return false;
    std::unique_lock<std::mutex> lk(_lock);
    auto it = _taskStatusMap.find(taskPtr->name());
    if (it == _taskStatusMap.end()) return false;
    if (it->second != Status::TASK_IDLE) {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' not IDLE (status: " + std::to_string(static_cast<int>(it->second)) + ")");
        return false;
    }

    // Check if all input tensors are available
    std::vector<std::string> missingInputs;
    for (const auto & input : taskPtr->inputs()) {
        if (_tensors.find(input.name()) == _tensors.end()) {
            missingInputs.push_back(input.name());
        }
    }

    if (!missingInputs.empty()) {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' missing inputs: " +
                [&missingInputs]() {
                    std::string result;
                    for (size_t i = 0; i < missingInputs.size(); ++i) {
                        result += missingInputs[i];
                        if (i < missingInputs.size() - 1) result += ", ";
                    }
                    return result;
                }());
        return false;
    }

    it->second = Status::TASK_READY;
    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' is now READY with " +
            std::to_string(taskPtr->inputs().size()) + " input tensors");
    return true;
}

void InferenceJob::processReadyTask(TaskPtr taskPtr)
{
    if (!taskPtr) return;
    auto it = _taskStatusMap.find(taskPtr->name());
    if (it == _taskStatusMap.end()) return;
    if (it->second != Status::TASK_READY) return;

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Processing ready task '" + taskPtr->name() + "' (" +
            (taskPtr->processor() == Processor::NPU ? "NPU" : "CPU") + ")");

    Tensors inputTensors = taskPtr->inputs();

    // Patch: populate input tensor data pointers from produced tensors map.
    // After refactor, we only tracked readiness via _tensors but did not
    // propagate the actual buffer addresses into the task's input tensors,
    // resulting in null (0) data_ptr passed to ONNX Runtime (segfault).
    // For each input tensor by name, if a produced tensor exists in _tensors,
    // copy its data pointer (and physical address) into the local tensor copy.
    {
        std::unique_lock<std::mutex> lk(_lock);
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Mapping " + std::to_string(inputTensors.size()) + " input tensors for task '" + taskPtr->name() + "'");

        for (auto & inTensor : inputTensors) {
            auto itTensor = _tensors.find(inTensor.name());
            if (itTensor != _tensors.end()) {
                inTensor.data() = itTensor->second.data();
                inTensor.phy_addr() = itTensor->second.phy_addr();
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Mapped tensor '" + inTensor.name() + "' (data: " +
                        std::to_string(reinterpret_cast<uintptr_t>(inTensor.data())) + ")");
            } else {
                // Should not happen because readiness check ensured presence
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Critical: tensor '" + inTensor.name() +
                             "' missing in _tensors during processReadyTask (should not happen)");
            }
        }
    }

    // Defensive validation: ensure every input tensor now has a non-null data pointer.
    // If any pointer is null, log a detailed error and abort scheduling this task to avoid
    // propagating a segfault into downstream CPU execution (e.g., ONNX Runtime).
    {
        bool missingPtr = false;
        for (auto & t : inputTensors) { // non-const to allow calling non-const accessor
            if (t.data() == nullptr) {
                missingPtr = true;
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] processReadyTask: Input tensor '" + t.name() + "' has null data pointer (unexpected)");
            }
        }
        if (missingPtr) {
            LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Aborting scheduling of task '" + taskPtr->name() + "' due to invalid input tensor pointers");
            // Revert status to IDLE so that if tensors become available later it can retry.
            it->second = Status::TASK_IDLE;
            return; // Do not create Request
        }
    }
    RequestPtr req = Request::Create(taskPtr.get(), inputTensors, {}, _userArg, _jobId);
    req->setInferenceJob(this);
    req->SetStatus(Request::Status::REQ_BUSY);
    req->requestor_name() = taskPtr->name();
    _requests.push_back(req);
    it->second = Status::TASK_BUSY;

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' scheduled for execution (request ID: " + std::to_string(req->id()) + ")");

    RequestResponse::InferenceRequest(req);
}

}  // namespace dxrt
