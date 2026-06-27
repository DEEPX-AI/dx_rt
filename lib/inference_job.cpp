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
#include "dxrt/safe_cast.h"

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
    const InferenceEngine* inferenceEngine,
    int jobId)
{
    std::ignore = jobId;
    Tensors outputTensors;
    if (userOutputBase == nullptr || inferenceEngine == nullptr)
    {
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

    bool all_request_complete = false;
    Task* thisTask = req->task();

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] onRequestComplete: Task '" + thisTask->name() +
            "' completed. Processor: " + (thisTask->processor() == Processor::NPU ? "NPU" : "CPU") +
            ", is_tail: " + (thisTask->is_tail() ? "true" : "false"));

    {
        std::unique_lock<std::mutex> lk(_lock);
        // Update tensor map
        for (const Tensor& output : req->outputs())
        {
            auto name = output.name();
            auto itTensor = _tensors.find(name);
            if (itTensor != _tensors.end()) itTensor->second = output; else _tensors.insert(make_pair(name, output));
        }
        // Mark task done
        auto itStatus = _taskStatusMap.find(thisTask->name());
        if (itStatus == _taskStatusMap.end())
        {
            throw InvalidOperationException(EXCEPTION_MESSAGE("The task name was not found in this job."));
        }
        itStatus->second = Status::TASK_DONE;
        TASK_FLOW_FINISH("[" + to_string(_jobId) + "]" + thisTask->name());

        // Check if all requests complete - use original logic with task count
        _doneCount++;
        all_request_complete = (_doneCount.load() == _outputCount.load());

        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' done. Progress: " + std::to_string(_doneCount.load()) + "/" + std::to_string(_outputCount.load()));

        _latency += req->latency();
        if (req->task()->processor() == Processor::NPU)
        {
            _infTime += req->inference_time();
        }
    }

    if (!thisTask->nexts().empty())
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + thisTask->name() +
                "' has " + std::to_string(thisTask->nexts().size()) + " successor(s). Processing...");

        for (const auto & nextTaskPtr : thisTask->nexts())
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
        // Note: all_request_complete status doesn't change when scheduling successors
        // because completion is based on original task count, not dynamic request count
    }

    if (all_request_complete)
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] All tasks completed! Calling onAllRequestComplete()");
        onAllRequestComplete();
    }
}

InferenceJob::InferenceJob(int id) noexcept
: _jobId(id)
{
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

    // Build _returnOutputs whenever we need to store results OR when the user
    // provided an output buffer. setReturnOutputs() performs the copy into the
    // user-provided _outputPtr, so skipping it when a callback is registered
    // would silently ignore the user's output pointer.
    const bool buildReturnOutputs = _storeResult || (_outputPtr != nullptr);

    if (buildReturnOutputs)
    {
        // Build _returnOutputs which contains only final model outputs (ordered)
        setReturnOutputs();
    }

    // Execute callback with final model outputs only (filtered from _tensors)
    try
    {
        LOG_DXRT_DBG << "task callback" << endl;
        TensorPtrs callbackOutputs;
        if (buildReturnOutputs)
        {
            callbackOutputs = _returnOutputs;
        }
        else
        {
            std::unique_lock<std::mutex> lk(_lock);
            for (const auto &name : _outputs)
            {
                auto it = _tensors.find(name);
                if (it != _tensors.end())
                {
                    callbackOutputs.emplace_back(std::make_shared<Tensor>(it->second));
                }
                else
                {
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
    } catch (...) {  // NOSONAR:S2738
        LOG_DXRT << "callback error unknown " << endl;
    }

    // Release buffers and update job status regardless of callback presence or failures.
    ReleaseAllOutputBuffer();
    setStatus(Request::Status::REQ_DONE);
    SetOccupiedJob(false);

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

    _outputCount.store(static_cast<int>(tasks_.size()));
    for (const auto& it :  tasks_)
    {
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }
}

void InferenceJob::SetInferenceJobMultiHead(std::vector<std::shared_ptr<Task>>& tasks_,
                                           const std::vector<std::shared_ptr<Task>>& inputTasks_,
                                           const std::vector<std::string>& lastOutputOrder,
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

    _outputCount.store(static_cast<int>(tasks_.size()));
    for (const auto& it :  tasks_)
    {
        _taskStatusMap.insert(make_pair(it->name(), Status::TASK_IDLE));
    }

    LOG_DBG("[MULTI_HEAD] Set inference job with " + std::to_string(inputTasks_.size()) + " input tasks");
}

int InferenceJob::startJob(void *inputPtr, void *userArg, void *outputPtr)
{
    TaskPtr head_task = _headTask.lock();
    if (head_task == nullptr)
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
        if (_modelInputNames.size() > 1)
        {
            LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] WARNING: startJob() called with " +
                         std::to_string(_modelInputNames.size()) +
                         " model inputs. Should use startMultiInputJob() instead!");
        }

        // Find all model input tensors that are used by tasks other than the primary head
        // For each model input, check if any non-primary-head task uses it
        for (const auto& model_input_name : _modelInputNames) {
            // Find tensor metadata and check if it's shared
            bool is_shared_input = false;

            // Use shared_ptr because Tensor has no default constructor
            std::shared_ptr<Tensor> shared_input_metadata;

            for (const auto& task : _tasks)
            {
                for (const auto& input_tensor : task->inputs())
                {
                    if (input_tensor.name() == model_input_name)
                    {
                        // Save the first occurrence of this tensor's metadata
                        if (!shared_input_metadata)
                        {
                            shared_input_metadata = std::make_shared<Tensor>(input_tensor);
                        }

                        // If this task is not the primary head, this is a shared input
                        if (task != head_task)
                        {
                            is_shared_input = true;
                            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Model input '" + model_input_name +
                                    "' is used by non-primary task '" + task->name() + "'");
                        }
                        break;  // Found this input in current task, move to next task
                    }
                }
                // Continue checking all tasks (don't break early) to log all shared usage
            }

            if (is_shared_input && shared_input_metadata)
            {
                // This is a shared model input, add it to _tensors with the provided data pointer
                Tensor model_input_tensor = *shared_input_metadata;  // Copy metadata
                model_input_tensor.data() = inputPtr;
                model_input_tensor.phy_addr() = 0;
                _tensors.insert(std::make_pair(model_input_name, model_input_tensor));
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Added shared model input tensor: " + model_input_name);
            }
        }
    }

    void* first_output = nullptr;
    if (head_task->is_tail())
    {
        first_output = outputPtr;
    }

    RequestPtr req = Request::Create(head_task.get(), inputPtr, first_output, userArg, _jobId);
    req->requestor_name() = "";
    req->SetStatus(Request::Status::REQ_BUSY);
    req->setInferenceJob(this);  // on each request complete, do next request or complete whole inference
    _requests.push_back(req);

    if (_outputPtr != nullptr)
    {
        // To avoid intermediate copies, use user-provided buffer only for pure tail tasks
        if (head_task->is_tail())
        {
            // Map all tail-task outputs to user buffer with model-global offsets
            Tensors outputTensors = BuildUserOutputTensorsForTailTask(head_task, _outputPtr, _outputs, _inferenceEnginePtr, _jobId);
            req->setOutputs(outputTensors);
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + head_task->name() + "' is tail task, using user output buffer directly");
        }
        else
        {
            req->getData()->output_buffer_base = nullptr;
            LOG_DBG("[Job_" + std::to_string(_jobId) + "] Head task '" + head_task->name() + "' uses internal buffer (not a pure tail task)");
        }
    }
    else
    {
        req->getData()->output_buffer_base = nullptr;
    }

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
            for (const auto& task : _tasks)
            {
                const auto& inputs = task->inputs();
                for (const auto& input : inputs)
                {
                    if (input.name() == tensorName)
                    {
                        targetTask = task;
                        break;
                    }
                }
                if (targetTask)
                {
                    break;
                }
            }

            if (targetTask)
            {
                // Get the actual tensor information from the task
                const auto& taskInputs = targetTask->inputs();
                for (const auto& input : taskInputs)
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
            for (const auto& task : _tasks)
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
            auto memory = std::make_shared<std::vector<uint8_t>>(output_size);
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
            size_t tensor_offset = _inferenceEnginePtr->GetOutputTensorOffset(name);
            uint8_t* dest_ptr = static_cast<uint8_t*>(_outputPtr) + tensor_offset;

            const void* src_ptr = output_tensor.data();
            size_t tensor_size = output_tensor.size_in_bytes();

            if (src_ptr != nullptr && dest_ptr != src_ptr)
            {
                std::memcpy(dest_ptr, src_ptr, tensor_size);

                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Thread-safe copy: " + name +
                        " to offset " + std::to_string(tensor_offset) +
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

void InferenceJob::setCallBack(std::function<int(const TensorPtrs &outputs, void *userArg, int jobId)> func)
{
    std::unique_lock<std::mutex> lk(_lock);
    _infEngCallback = func;
}

#ifdef USE_VNPU
void InferenceJob::setUserInputReleaseCallback(std::function<void(void* userArg, int jobId)> func)
{
    std::unique_lock<std::mutex> lk(_lock);
    LOG_DXRT_DBG << "[Job_" + std::to_string(_jobId) + "] Set User Input Release Callback" << std::endl;
    _userInputReleaseCallback = func;
}

void InferenceJob::TriggerUserInputRelease()
{
    // Use atomic flag to ensure callback is called only once
    bool expected = false;
    if (_userInputReleased.compare_exchange_strong(expected, true))
    {
        // First call - successfully changed false -> true
        if (_userInputReleaseCallback != nullptr)
        {
            LOG_DXRT_DBG << "[Job_" + std::to_string(_jobId) + "] Head task processing complete. Calling user input release callback" << std::endl;
            try
            {
                _userInputReleaseCallback(_userArg, _jobId);
            }
            catch (std::exception& e)
            {
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] User input release callback error: " + std::string(e.what()));
            }
            catch (...) {
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] User input release callback unknown error");
            }
        }
        else
        {
            LOG_DXRT_DBG << "[Job_" + std::to_string(_jobId) + "] User input release callback is not set (nullptr)" << std::endl;
        }
    }
    else
    {
        // Callback already triggered by another task (expected != false after compare_exchange)
        // This is normal for multi-task models where only the first completed head task should release
        LOG_DXRT_DBG << "[Job_" + std::to_string(_jobId) + "] User input already released by previous task (flag was already true)" << std::endl;
    }
}
#endif  // USE_VNPU

void InferenceJob::Clear()
{
    std::unique_lock<std::mutex> lk(_lock);

    _tensors.clear();
    _tasks.clear();  // Clear tasks for multi-input support
    // _head.reset();
    // _headTask.reset();
    setStatus(Request::Status::REQ_IDLE);
    _outputCount.store(0);
    _doneCount.store(0);

    _userArg = nullptr;
    _latency = 0;
    _infTime = 0;
    _inferenceEnginePtr = nullptr;
    _infEngCallback = nullptr;
    _outputPtr = nullptr;
    _storeResult = false;

#ifdef USE_VNPU
    _userInputReleaseCallback = nullptr;
    _userInputReleased.store(false);
#endif

    // Clear multi-head support variables
    _inputTasks.clear();
    _isMultiHead = false;

    _occupiedJob.store(false);
}

InferenceJob::~InferenceJob()
{
    Clear();
}

// Returns true if req's output_buffer_base points into the user-provided output buffer.
// Uses the explicit flag first; falls back to address-range check for legacy paths.
bool InferenceJob::isUserOutputBuffer(const RequestPtr& req) const
{
    if (req->getData()->outputs_is_user_buffer)
    {
        return true;
    }

    // Legacy fallback: detect via address range comparison.
    // Skipped for dynamic-shape models (GetOutputSize() returns -1).
    if (_outputPtr == nullptr || req->output_buffer_base() == nullptr)
    {
        return false;
    }

    const uint64_t output_size = _inferenceEnginePtr->GetOutputSize();
    if (output_size == static_cast<uint64_t>(-1))
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Skipping range check for dynamic shape model");
        return false;
    }

    const auto* user_buffer_start = static_cast<const uint8_t*>(_outputPtr);
    const auto* user_buffer_end   = user_buffer_start + output_size;
    const auto* output_buf_ptr    = static_cast<const uint8_t*>(req->output_buffer_base());

    if (output_buf_ptr >= user_buffer_start && output_buf_ptr < user_buffer_end)
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + req->task()->name() +
                "' uses user output buffer - detected by range check");
        return true;
    }

    return false;
}

void InferenceJob::releaseIndividualBuffers(const RequestPtr& req)
{
    const bool uses_user_outputBuffer = isUserOutputBuffer(req);
    const bool is_intermediate_task   = !req->task()->is_tail();
    const bool should_release_output  = !uses_user_outputBuffer && (_outputPtr == nullptr || is_intermediate_task);

    if (should_release_output)
    {
        LOG_DXRT_DBG << "Request " << req->id() << " releasing internal output buffer" << std::endl;
        req->task()->ReleaseOutputBuffer(req->output_buffer_base());
    }
    else
    {
        LOG_DXRT_DBG << "Request " << req->id()
                     << " skipping output buffer release"
                     << (uses_user_outputBuffer ? " (user-provided buffer)" : " (tail task with user outputPtr)")
                     << std::endl;
    }

    if (req->task()->processor() == Processor::NPU)
    {
        LOG_DXRT_DBG << "Request " << req->id() << " releasing NPU encoded input/output buffers" << std::endl;
        req->task()->ReleaseEncodedInputBuffer(req->encoded_inputs_ptr());
        req->task()->ReleaseEncodedOutputBuffer(req->encoded_outputs_ptr());
    }

    // CPU buffers are managed by CPUHandle and do not require explicit release here
    req->markBufferReleased();
}

void InferenceJob::ReleaseAllOutputBuffer()
{
    std::unique_lock<std::mutex> lk(_lock);

    for (const auto& req_weak_ptr :  _requests)
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

            // Skip individual buffer release in two cases:
            //   1. isBufferReleased(): already freed earlier (e.g. by early release path)
            //   2. hasBufferSet():     NOT yet freed here, but ownership is held by BufferSet.
            //                         The second loop below calls req->Reset() -> releaseBuffers(),
            //                         which invokes task->ReleaseAllBuffers(*_bufferSet) and cleans up.
            //                         Individual release must be skipped to avoid double-free.
            if (req->isBufferReleased())
            {
                LOG_DXRT_DBG << "Request " << req->id() << " buffers already released - skipping" << std::endl;
            }
            else if (req->hasBufferSet())
            {
                LOG_DXRT_DBG << "Request " << req->id() << " has BufferSet - deferring to Reset()->releaseBuffers()" << std::endl;
            }

            else
            {
                // This branch is only reachable when DXRT_USE_DEVICE_VALIDATION is enabled.
                // In normal inference, output_buffer_base is always nullptr at InferenceRequest()
                // entry, so AcquireAllBuffers() and setBufferSet() are always called, making
                // hasBufferSet() always true. The only path that sets output_buffer_base to a
                // non-null value before InferenceRequest() is CreateValidateRequest(), which
                // bypasses AcquireAllBuffers() and therefore never calls setBufferSet().
                LOG_DXRT_DBG << "Request " << req->id() << " no BufferSet - using individual buffer release" << std::endl;
                releaseIndividualBuffers(req);
            }
        }
        else
        {
            // weak_ptr::lock() failed in the buffer-release loop.
            // The Request object was already destroyed before we could release its buffers.
            // This indicates a lifecycle bug: something released the Request shared_ptr
            // while _requests still held a weak_ptr to it.
            DXRT_ASSERT(false, "Job ReleaseAllOutputBuffer: "
                "Request expired during buffer-release loop (loop 1). "
                "Request shared_ptr was destroyed before buffers were released.");
        }
    }
    for (const auto& it : _requests)
    {
        RequestPtr req = it.lock();

        if (req)
        {
            req->Reset();
        }
        else
        {
            // weak_ptr::lock() failed in the Reset loop (loop 2),
            // even though the same weak_ptr succeeded in the buffer-release loop above.
            // This means the Request was destroyed between loop 1 and loop 2,
            // despite this function holding _lock throughout. Severe lifecycle violation.
            DXRT_ASSERT(false, "Job ReleaseAllOutputBuffer: "
                "Request expired during Reset loop (loop 2). "
                "Request was alive in loop 1 but destroyed before Reset() could be called.");
        }
    }
    _requests.clear();
    _use_flag.store(false);

    TASK_FLOW("[" + to_string(_jobId)+"] ReleaseAllOutputBuffer");
}

void InferenceJob::setStatus(Request::Status status)
{
    std::unique_lock<std::mutex> lock(_waitMutex);
    _status.store(status);
    _waitCV.notify_one();
}

int InferenceJob::getId() const
{
    return _jobId;
}
Request::Status InferenceJob::getStatus() const
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
    if (!taskPtr)
    {
        return false;
    }
    std::unique_lock<std::mutex> lk(_lock);
    auto it = _taskStatusMap.find(taskPtr->name());
    if (it == _taskStatusMap.end())
    {
        return false;
    }

    if (it->second != Status::TASK_IDLE)
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' not IDLE (status: " + std::to_string(static_cast<int>(it->second)) + ")");
        return false;
    }

    // Check if all input tensors are available
    std::vector<std::string> missing_inputs;
    for (const auto & input : taskPtr->inputs())
    {
        if (_tensors.find(input.name()) == _tensors.end())
        {
            missing_inputs.push_back(input.name());
        }
    }

    if (!missing_inputs.empty())
    {
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + taskPtr->name() + "' missing inputs: " +
                [&missing_inputs]() {
                    std::string result;
                    for (size_t i = 0; i < missing_inputs.size(); ++i) {
                        result += missing_inputs[i];
                        if (i < missing_inputs.size() - 1) result += ", ";
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

void InferenceJob::processReadyTask(TaskPtr nextTaskPtr)
{
    if (!nextTaskPtr)
    {
        return;
    }

    auto it = _taskStatusMap.find(nextTaskPtr->name());
    if (it == _taskStatusMap.end())
    {
        return;
    }

    if (it->second != Status::TASK_READY)
    {
        return;
    }

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Processing ready task '" + nextTaskPtr->name() + "' (" +
            (nextTaskPtr->processor() == Processor::NPU ? "NPU" : "CPU") + ")");

    Tensors next_input_tensors = nextTaskPtr->inputs();

    // Patch: populate input tensor data pointers from produced tensors map.
    // After refactor, we only tracked readiness via _tensors but did not
    // propagate the actual buffer addresses into the task's input tensors,
    // resulting in null (0) data_ptr passed to ONNX Runtime (segfault).
    // For each input tensor by name, if a produced tensor exists in _tensors,
    // copy its data pointer (and physical address) into the local tensor copy.
    {
        std::unique_lock<std::mutex> lk(_lock);
        LOG_DBG("[Job_" + std::to_string(_jobId) + "] Mapping " + std::to_string(next_input_tensors.size()) + " input tensors for task '" + nextTaskPtr->name() + "'");

        for (auto & inTensor : next_input_tensors)
        {
            auto itTensor = _tensors.find(inTensor.name());
            if (itTensor != _tensors.end())
            {
                inTensor.data() = itTensor->second.data();
                inTensor.phy_addr() = itTensor->second.phy_addr();
                LOG_DBG("[Job_" + std::to_string(_jobId) + "] Mapped tensor '" + inTensor.name() + "' (data: " +
                        std::to_string(SafeCast::PointerToInteger<void*>(inTensor.data())) + ")");
            }
            else
            {
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
        bool missing_ptr = false;
        for (auto & t : next_input_tensors)
        { // non-const to allow calling non-const accessor
            if (t.data() == nullptr)
            {
                missing_ptr = true;
                LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] processReadyTask: Input tensor '" + t.name() + "' has null data pointer (unexpected)");
            }
        }
        if (missing_ptr)
        {
            LOG_DXRT_ERR("[Job_" + std::to_string(_jobId) + "] Aborting scheduling of task '" + nextTaskPtr->name() + "' due to invalid input tensor pointers");
            // Revert status to IDLE so that if tensors become available later it can retry.
            it->second = Status::TASK_IDLE;
            return; // Do not create Request
        }
    }
    RequestPtr req = Request::Create(nextTaskPtr.get(), next_input_tensors, {}, _userArg, _jobId);
    req->setInferenceJob(this);
    req->SetStatus(Request::Status::REQ_BUSY);
    req->requestor_name() = nextTaskPtr->name();
    _requests.push_back(req);
    it->second = Status::TASK_BUSY;

    LOG_DBG("[Job_" + std::to_string(_jobId) + "] Task '" + nextTaskPtr->name() + "' scheduled for execution (request ID: " + std::to_string(req->id()) + ")");
    RequestResponse::InferenceRequest(req);
}

}  // namespace dxrt
