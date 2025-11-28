/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/inference_engine.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
// #include <errno.h>

#include <stdexcept>
#include <cstring>
#ifdef __linux__
    #include <cxxabi.h>
#elif _WIN32
#include <chrono>
#include <thread>
#endif
// #include <regex>
#include <set>

#include "resource/log_messages.h"
#include "dxrt/objects_pool.h"
#include "dxrt/configuration.h"
#include "dxrt/datatype.h"
#include "dxrt/task.h"
#include "dxrt/device_pool.h"
// #include "dxrt/util.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/filesys_support.h"
#include "dxrt/inference_job.h"
#include "dxrt/exception/exception.h"
#include "dxrt/device_info_status.h"
#include "dxrt/service_util.h"

#define PRINT_ALL_INFERENCE_ENGINE

using std::cout;
using std::endl;

namespace dxrt
{

static const int SUB_BATCH_MAX_COUNT = 128;
std::mutex InferenceEngine::_sInferenceEngineMutex;
constexpr int InferenceEngine::INFERENCE_JOB_MAX_COUNT;

InferenceEngine::InferenceEngine(const std::string &path_, InferenceOption &option_)
:_modelFile(path_), _option(option_)
{
#ifdef USE_SERVICE
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
    {
        if (isDxrtServiceRunning() == false)
        {
            throw dxrt::ServiceIOException("dxrt service is not running");
        }
    }
#endif

    DevicePool::GetInstance().InitTaskLayers();
    DevicePool::GetInstance().InitNFHLayers();

    std::lock_guard<std::mutex> lock(_sInferenceEngineMutex);

    _modelDir = getParentPath(getAbsolutePath(_modelFile));

    LOG_DXRT_DBG <<_modelFile << endl;
    LOG_DXRT_DBG << getAbsolutePath(_modelFile) << endl;
    LOG_DXRT_DBG << _modelDir << endl;

    initializeEnvironmentVariables();
    initializeModel();
    buildTasksAndSubgraphMap();

    // Parse multi-input information from model data
    #ifdef USE_ORT
    if (_option.useORT == true)
    {
        _modelInputOrder = _modelData.deepx_graph.inputs();
    }
    else
    #endif
    {
        // Non-ORT mode: collect inputs from head tasks
        _modelInputOrder.clear();
        // Collect input tensors from head tasks
        for (auto &task : _tasks)
        {
            if (task->is_head())
            {
                for (auto& input : task->inputs())
                {
                    _modelInputOrder.push_back(input.name());
                }
            }
        }
    }
    _isMultiInput = (_modelInputOrder.size() > 1);
    LOG_DBG("Multi-input model: " + std::to_string(_isMultiInput));
    LOG_DBG("Input tensor count: " + std::to_string(_modelInputOrder.size()));

    buildInputTensorMapping();
    for (const auto &pair : _inputTensorToTaskMap)
    {
        std::ignore = pair;  // Suppress unused variable warning
        LOG_DBG("Input tensor '" + pair.first + "' -> Task '" + pair.second + "'");
    }

    buildTaskGraph();

    // Initialize _lastOutputOrder and collect tail tasks
    _numTails = 0;
    std::vector<std::pair<TaskPtr, std::vector<std::string>>> tailTaskOutputs;


    // Use output order from graph_info (already parsed)
#ifdef USE_ORT
    if (_option.useORT == true)
    {
        // ORT mode: Use graph outputs (already fixed for PPCPU in TEMPORARY FIX section)
        _lastOutputOrder = _modelData.deepx_graph.outputs();

        // Collect tail tasks for offset calculation
        // CRITICAL: Only include outputs that are actually model outputs (in _lastOutputOrder)
        // A tail task may produce multiple outputs, but not all are model outputs
        for (auto &task : _tasks)
        {
            if (task->is_tail())
            {
                std::vector<std::string> taskOutputNames;
                for (auto& output : task->outputs())
                {
                    // Only include outputs that are in the model's final output list
                    if (std::find(_lastOutputOrder.begin(), _lastOutputOrder.end(), output.name())
                        != _lastOutputOrder.end())
                    {
                        taskOutputNames.push_back(output.name());
                    }
                }

                // Only add task if it has model outputs
                if (!taskOutputNames.empty())
                {
                    tailTaskOutputs.push_back({task, taskOutputNames});
                    _numTails++;
                    LOG_DXRT_DBG << "Tail task '" << task->name() << "': " << taskOutputNames.size()
                                 << " model output(s) from " << task->outputs().size() << " total outputs" << std::endl;
                }
            }
        }
    }
    else
#endif
    {
        // Non-ORT mode: Build _lastOutputOrder from tail tasks
        // Note: task->outputs() already correctly set for PPCPU (contains "PPU_OUTPUT")
        //       via TaskData::set_from_npu() in task_data.cpp
        _lastOutputOrder.clear();

        for (auto &task : _tasks)
        {
            if (task->is_tail())
            {
                std::vector<std::string> taskOutputNames;

                for (auto& output : task->outputs())
                {
                    taskOutputNames.push_back(output.name());
                    _lastOutputOrder.push_back(output.name());
                }

                tailTaskOutputs.push_back({task, taskOutputNames});
                _numTails++;

                LOG_DXRT_DBG << "Tail task '" << task->name() << "': " << taskOutputNames.size()
                             << " output(s) added to _lastOutputOrder" << std::endl;
            }
        }
    }

    // Special handling for PPU/PPCPU models (_isPPU includes both PPU and PPCPU)
    if (_isPPU)
    {
        // Rebuild _lastOutputOrder to ensure consistency for PPU models
        std::vector<std::string> newLastOutputOrder;

        for (auto &task : _tasks)
        {
            if (task->is_tail())
            {
                for (auto& output : task->outputs())
                {
                    newLastOutputOrder.push_back(output.name());
                }
            }
        }

        // Update if different (important for PPU model consistency)
        if (newLastOutputOrder != _lastOutputOrder)
        {
            LOG_DBG("PPU/PPCPU model: Updating _lastOutputOrder for consistency");
            _lastOutputOrder = newLastOutputOrder;

            // Recalculate tailOffsets for PPU/PPCPU models using simple sequential offset
            int64_t cumulativeOffset = 0;
            for (auto &task : _tasks)
            {
                if (task->is_tail())
                {
                    task->setTailOffset(cumulativeOffset);
                    cumulativeOffset += task->output_size();
                    LOG_DBG("PPU/PPCPU Task '" + task->name() + "' tailOffset: "
                            + std::to_string(task->getTailOffset())
                            + ", size: " + std::to_string(task->output_size()));
                }
            }
        }
        else
        {
            // Offsets already set correctly, just log for verification
            for (auto &task : _tasks)
            {
                if (task->is_tail())
                {
                    LOG_DBG("PPU/PPCPU Task '" + task->name() + "' tailOffset maintained: "
                            + std::to_string(task->getTailOffset()));
                }
            }
        }
    }
    else
    {
        // Standard models: Calculate tailOffset based on _lastOutputOrder sequence
        // Create mapping from tensor name to its position in _lastOutputOrder
        std::map<std::string, size_t> tensorOrderMap;
        for (size_t i = 0; i < _lastOutputOrder.size(); ++i)
        {
            tensorOrderMap[_lastOutputOrder[i]] = i;
        }

        // Set tailOffset for each task based on cumulative tensor sizes in _lastOutputOrder
        for (const auto& pair : tailTaskOutputs)
        {
            TaskPtr task = pair.first;
            const auto& outputNames = pair.second;

            // Find the minimum position of this task's outputs in _lastOutputOrder
            size_t minPosition = std::numeric_limits<size_t>::max();
            for (const auto& outputName : outputNames)
            {
                auto it = tensorOrderMap.find(outputName);
                if (it != tensorOrderMap.end())
                {
                    minPosition = std::min(minPosition, it->second);
                }
            }

            if (minPosition == std::numeric_limits<size_t>::max())
            {
                LOG_DXRT_ERR("Task '" + task->name() + "' is classified as a tail but its outputs are not found in the model output list");
                LOG_DXRT_ERR("Task outputs: ");
                for (const auto& name : outputNames) {
                    LOG_DXRT_ERR("  - '" + name + "'");
                }
                LOG_DXRT_ERR("_lastOutputOrder: ");
                for (size_t i = 0; i < _lastOutputOrder.size(); ++i) {
                    LOG_DXRT_ERR("  [" + std::to_string(i) + "] '" + _lastOutputOrder[i] + "'");
                }
                throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::InferenceEngine_InvaildModel()));
            }

            // Calculate offset based on preceding tensors in _lastOutputOrder
            int64_t taskOffset = 0;
            for (size_t i = 0; i < minPosition; ++i)
            {
                const std::string& precedingTensorName = _lastOutputOrder[i];

                // Find the tensor size
                for (const auto& searchPair : tailTaskOutputs)
                {
                    TaskPtr searchTask = searchPair.first;
                    for (const auto& tensor : searchTask->outputs())
                    {
                        if (tensor.name() == precedingTensorName)
                        {
                            taskOffset += tensor.size_in_bytes();
                            break;
                        }
                    }
                }
            }

            task->setTailOffset(taskOffset);
            LOG_DBG("Task '" + task->name() + "' tailOffset set to: " + std::to_string(taskOffset));
        }
    }

    DXRT_ASSERT(_lastOutputOrder.size() > 0, "last output order is empty");

    /*
    LOG << "Last Output Tensors: ";
    for (size_t i = 0; i < _lastOutputOrder.size(); ++i) {
        cout << _lastOutputOrder[i];
        if (i < _lastOutputOrder.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
    */
    LOG_DBG("_numTails : "+std::to_string(_numTails));
    DXRT_ASSERT(_numTails != 0, "Invalid Graph : tail task is not found. Check the DX-COM compilation process.");

#ifdef PRINT_ALL_INFERENCE_ENGINE
    if ( Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_MODEL_INFO) )
    {
        cout << *this << endl;
    }
#endif

    _inferenceJobPool = std::make_shared<CircularDataPool<InferenceJob>>(InferenceEngine::INFERENCE_JOB_MAX_COUNT);

    // Build tensor registry for comprehensive tensor management
    buildTensorRegistry();
    calculateTensorOffsets();

    LOG_DBG("InferenceEngine created.");
}

InferenceEngine::~InferenceEngine(void)
{
    LOG_DXRT_DBG << endl;
    std::call_once(_disposeOnceFlag, [this]() { this->disposeOnce(); });
    LOG_DXRT_DBG <<" DONE"<< endl;
}

TensorPtrs InferenceEngine::Run(void *inputPtr, void *userArg, void *outputPtr)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }

    // Track user output buffer state for multi-tail models
    _userOutputPtr = outputPtr;
    _hasUserOutputBuffer = (outputPtr != nullptr);

    // Auto-split single input buffer for multi-input models if applicable
    if (shouldAutoSplitInput() && inputPtr != nullptr)
    {
        LOG_DBG("Auto-splitting single input buffer for multi-input model");

        // Get individual tensor sizes and split the input buffer
        auto tensorSizes = GetInputTensorSizes();
        std::vector<std::vector<uint8_t>> splitBuffers(tensorSizes.size());
        std::vector<void*> splitPtrs(tensorSizes.size());

        uint64_t offset = 0;
        for (size_t i = 0; i < tensorSizes.size(); ++i)
        {
            splitBuffers[i].resize(tensorSizes[i]);
            std::memcpy(splitBuffers[i].data(), static_cast<uint8_t*>(inputPtr) + offset, tensorSizes[i]);
            splitPtrs[i] = splitBuffers[i].data();
            offset += tensorSizes[i];
        }

        // Use multi-input API
        return RunMultiInput(splitPtrs, userArg, outputPtr);
    }

    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder, _modelInputOrder);
    infJob->setInferenceEngineInterface(this);
    infJob->SetStoreResult(true);
    infJob->setCallBack([this](TensorPtrs &outputs, void *userArg, int jobId)->int{
        int retval = 0;
        if (_userCallback != nullptr)
        {
            retval = _userCallback(outputs, userArg);
        }
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(false);
        return retval;
    });  // inference engine callback

    int jobId = infJob->startJob(inputPtr, userArg, outputPtr);
    {
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(true);
    }
    return Wait(jobId);
}

int InferenceEngine::RunAsync(void *inputPtr, void *userArg, void *outputPtr)
{
    // Auto-split single input buffer for multi-input models if applicable
    if (shouldAutoSplitInput() && inputPtr != nullptr)
    {
        LOG_DBG("Auto-splitting single input buffer for multi-input model (async)");

        // Get individual tensor sizes and split the input buffer
        auto tensorSizes = GetInputTensorSizes();
        std::vector<std::vector<uint8_t>> splitBuffers(tensorSizes.size());
        std::vector<void*> splitPtrs(tensorSizes.size());

        uint64_t offset = 0;
        for (size_t i = 0; i < tensorSizes.size(); ++i)
        {
            splitBuffers[i].resize(tensorSizes[i]);
            std::memcpy(splitBuffers[i].data(), static_cast<uint8_t*>(inputPtr) + offset, tensorSizes[i]);
            splitPtrs[i] = splitBuffers[i].data();
            offset += tensorSizes[i];
        }

        // Use multi-input async API
        return RunAsyncMultiInput(splitPtrs, userArg, outputPtr);
    }

    return runAsync(inputPtr, userArg, outputPtr, -1, nullptr);
}

int InferenceEngine::RunAsync(const std::vector<void*>& inputPtrs, void *userArg, void *outputPtr)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }

    if (inputPtrs.empty())
    {
        throw InvalidArgumentException("Input pointers vector cannot be empty");
    }

    // Check if this should be interpreted as multi-input rather than batch
    if (_isMultiInput && inputPtrs.size() == _modelInputOrder.size())
    {
        // Interpret as multi-input single inference
        LOG_DBG("RunAsync: Interpreting vector<void*> as multi-input - input count: " + std::to_string(inputPtrs.size()));
        return RunAsyncMultiInput(inputPtrs, userArg, outputPtr);
    }

    // For single-input models or batch inference, use first pointer
    LOG_DBG("RunAsync: Using traditional single-input approach");
    return runAsync(inputPtrs[0], userArg, outputPtr, -1, nullptr);
}

int InferenceEngine::RunAsyncMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg, void *outputPtr)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }

    if (!_isMultiInput)
    {
        throw InvalidArgumentException("This model is not a multi-input model. Use RunAsync() instead.");
    }

    // Validate input tensor names
    for (const auto& pair : inputTensors)
    {
        if (_inputTensorToTaskMap.find(pair.first) == _inputTensorToTaskMap.end())
        {
            throw InvalidArgumentException("Unknown input tensor name: " + pair.first);
        }
    }

    // Check if all required input tensors are provided
    if (inputTensors.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input tensors, but got " + std::to_string(inputTensors.size()));
    }

    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    // Use multi-head setup if we have multiple input tasks, otherwise use traditional setup
    if (_inputTasks.size() > 1)
    {
        infJob->SetInferenceJobMultiHead(_tasks, _inputTasks, _lastOutputOrder, _modelInputOrder);
    }
    else
    {
        infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder, _modelInputOrder);
    }

    // Store outputs if user didn't register a callback
    if (_userCallback == nullptr)
    {
        infJob->SetStoreResult(true);
    }

    infJob->setInferenceEngineInterface(this);
    infJob->setCallBack([this](TensorPtrs &outputs, void *userArg, int jobId)->int{
        int retval = 0;
        if (_userCallback != nullptr)
        {
            retval = _userCallback(outputs, userArg);
        }
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(false);
        return retval;
    });  // inference engine callback

    int jobId = infJob->startMultiInputJob(inputTensors, userArg, outputPtr);
    {
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(true);
    }
    return jobId;
}

int InferenceEngine::RunAsyncMultiInput(const std::vector<void*>& inputPtrs, void *userArg, void *outputPtr)
{
    if (inputPtrs.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input pointers, but got " + std::to_string(inputPtrs.size()));
    }

    // Convert vector to map using model input order
    std::map<std::string, void*> inputTensors;
    for (size_t i = 0; i < inputPtrs.size(); ++i)
    {
        inputTensors[_modelInputOrder[i]] = inputPtrs[i];
    }

    return RunAsyncMultiInput(inputTensors, userArg, outputPtr);
}

std::vector<TensorPtrs> InferenceEngine::Run(
    const std::vector<void*>& inputBuffers,
    const std::vector<void*>& outputBuffers,
    const std::vector<void*>& userArgs
)
{
    int buffer_count = static_cast<int>(inputBuffers.size());

    if ( buffer_count == 0 )
    {
        throw dxrt::InvalidArgumentException(EXCEPTION_MESSAGE("The number of elements in inputPtrs must be greater than 0."));
    }

    // Check if this should be interpreted as multi-input rather than batch
    if (_isMultiInput && buffer_count == static_cast<int>(_modelInputOrder.size()))
    {
        // This could be multi-input. Check if outputBuffers size suggests single inference
        if (outputBuffers.size() == 1 && (userArgs.empty() || userArgs.size() == 1))
        {
            // Interpret as multi-input single inference
            LOG_DBG("Interpreting vector<void*> as multi-input (not batch) - input count: " + std::to_string(buffer_count));

            void* outputPtr = outputBuffers.empty() ? nullptr : outputBuffers[0];
            void* userArg = userArgs.empty() ? nullptr : userArgs[0];

            TensorPtrs singleResult = RunMultiInput(inputBuffers, userArg, outputPtr);

            // Return as vector of single result for API consistency
            std::vector<TensorPtrs> result;
            result.push_back(singleResult);
            return result;
        }
    }

    // Interpret as batch inference (original behavior)
    int batch_count = buffer_count;
    LOG_DBG("Interpreting vector<void*> as batch inference - batch size: " + std::to_string(batch_count));

    // check arguments size
    if ( userArgs.size() > 0 )
    {
        if ( batch_count != static_cast<int>(userArgs.size()) )
        {
            throw dxrt::InvalidArgumentException(EXCEPTION_MESSAGE("The number of elements in inputPtrs does not match the number of elements in userArgs."));
        }
    }

    // check outputPtrs size
    // it must be same size as inputBuffers
    if ( batch_count != static_cast<int>(outputBuffers.size()) )
    {
        throw dxrt::InvalidArgumentException("The number of elements in inputPtrs does not match the number of elements in outputPtrs.");
    }

    // create outputs data
    std::vector<TensorPtrs> result(batch_count);

    try
    {
        int start_index = 0;
        int sub_batch_loop = static_cast<int>(batch_count / SUB_BATCH_MAX_COUNT);
        int sub_batch_remain = static_cast<int>(batch_count % SUB_BATCH_MAX_COUNT);

        // std::cout << "sub-batch-loop=" << sub_batch_loop << " sub-batch-count=" << SUB_BATCH_MAX_COUNT
        //        << " total=" << sub_batch_loop * SUB_BATCH_MAX_COUNT << std::endl;
        // std::cout << "sub-batch-remain=" << sub_batch_remain << std::endl;


        if ( sub_batch_loop > 0 )
        {
            for (int i = 0; i < sub_batch_loop; ++i)
            {
                runSubBatch(result, SUB_BATCH_MAX_COUNT, start_index,
                    inputBuffers, outputBuffers, userArgs);

                start_index += SUB_BATCH_MAX_COUNT;
            }  // for i
        }

        if ( sub_batch_remain > 0 )
        {
            runSubBatch(result, sub_batch_remain, start_index,
                inputBuffers, outputBuffers, userArgs);
        }

    }
    catch (const dxrt::Exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
    catch(const std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }

    return result;
}

void InferenceEngine::runSubBatch(std::vector<TensorPtrs>& result, int batchCount, int startIndex,
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs
    )
{

    std::atomic<int> complete_count{0};
    std::mutex mtx_cv;  // mutex lock
    std::condition_variable cv_complete;  // complete condition variable
    bool is_completed = false;

    auto batch_callback = [this, &complete_count, &cv_complete, &mtx_cv, &result, batchCount, &is_completed](TensorPtrs &outputs, void *userArg, int jobId) {
            std::ignore = userArg;

            auto infJob = _inferenceJobPool->GetById(jobId);
            if (infJob == nullptr)
            {
                throw dxrt::InvalidOperationException(EXCEPTION_MESSAGE("InferenceJob not found for jobId"));
            }

            int batch_index = -1;

            try {
                // Get batch_index from InferenceJob
                batch_index = infJob->GetBatchIndex();
                // std::cout << "callback batch-index=" << batch_index << std::endl;

                if ( batch_index >= 0 )
                {
                    result.at(batch_index) = outputs;
                }
                else
                {
                    LOG_DXRT << "ERROR jobId=" << jobId << ", batch_index=" << batch_index << std::endl;
                }
            }
            catch(std::exception &e)
            {
                LOG_DXRT_ERR(LogMessages::InferenceEngine_BatchFailToAllocateOutputBuffer() << e.what());
            }

            complete_count++;
            LOG_DXRT_DBG << "runAsync complete-count=" << complete_count.load() << std::endl;
            if ( complete_count.load() == batchCount && is_completed)
            {
                //std::unique_lock<std::mutex> lock(mtx_cv);
                cv_complete.notify_one();
                LOG_DXRT_DBG << "runAsync completed" << std::endl;
            }
        };

    try
    {
        // Run asynchronous operations for each batch
        for (int i = 0; i < batchCount; ++i)
        {
            void* userArg = userArgs.size() > 0 ? userArgs.at(i) : nullptr;
            int current_index = startIndex + i;

            int job_id = runAsync(inputBuffers.at(current_index), userArg, outputBuffers.at(current_index), current_index, batch_callback);

            // std::cout << "runAsync index=" << current_index << std::endl;
            // std::cout << "inputPtrs size=" << inputPtrs.size() << " OutputPtrs size=" << pOutputPtrs->size() << std::endl;

            // map_insert(job_id, i);
            LOG_DXRT_DBG << "Insert jobId=" << job_id << ", batch_index=" << i << std::endl;

        }  // for i

        is_completed = true;

        // wait for inference done
        std::unique_lock<std::mutex> lock(mtx_cv);
        cv_complete.wait(lock, [&complete_count, batchCount]() { return complete_count.load() == batchCount; });
        LOG_DXRT_DBG << "runAsync return" << std::endl;
    }
    catch(const dxrt::Exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
    catch (const std::exception& e)
    {
        LOG_DXRT_ERR(e.what());
    }
}


// private
int InferenceEngine::runAsync(void *inputPtr, void *userArg, void *outputPtr, int batchIndex,
    std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }
    // return InferenceJob instance from InferenceJob pool (reused)
    // std::shared_ptr<InferenceJob> infJob = ObjectsPool::GetInstance().PickInferenceJob();
    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder, _modelInputOrder);
    infJob->SetBatchIndex(batchIndex);
    infJob->setInferenceEngineInterface(this);
    infJob->setCallBack([this, batchCallback](TensorPtrs &outputs, void *userArg, int jobId)->int{
            int retval = 0;
            if (this->_userCallback != nullptr)
            {
                retval = this->_userCallback(outputs, userArg);
            }
            if ( batchCallback != nullptr )
            {
                batchCallback(outputs, userArg, jobId);
            }
            _inferenceJobPool->GetById(jobId)->SetOccupiedJob(false);
            return retval;
        });  // inference engine callback

    if (_userCallback == nullptr)
    {
        infJob->SetStoreResult(true);
    }
    // if(infJob->getId()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || infJob->getId()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)


    int jobId = infJob->startJob(inputPtr, userArg, outputPtr);

    // occupired inference job id
    {
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(true);
    }

    return jobId;
}

void InferenceEngine::RegisterCallback(std::function<int(TensorPtrs &outputs, void *userArg)> f)
{
    LOG_DXRT_DBG << std::endl;
    _userCallback = f;
}

float InferenceEngine::RunBenchmark(int num, void *inputPtr)
{
#ifdef _WIN32
    // Need to check if RunBenchMarkWindows is required separately
    return RunBenchMarkWindows(num, inputPtr);
#endif
    float fps;
    std::atomic<int> done_count{0};
    std::mutex cv_mutex;
    std::condition_variable cv;

    auto callBack = [&done_count, num, &cv_mutex, &cv](TensorPtrs &outputs, void *userArg) -> int{
        std::ignore = outputs;
        std::ignore = userArg;

        int current_count = done_count.fetch_add(1) + 1;

        if (current_count == num) {
            std::lock_guard<std::mutex> lock(cv_mutex);
            cv.notify_one();
        }
        return 0;
    };  // callback used to count inference

    RegisterCallback(callBack);
    // bool isStandalone = (dxrt::DeviceStatus::GetCurrentStatus(0).GetDeviceType() == DeviceType::STD_TYPE);

    uint64_t infTime = 0;
    int infCnt = std::max(1, num);
    auto start_clock = std::chrono::steady_clock::now();
    for (int i=0 ; i < infCnt ; i++)
    {
        RunAsync(inputPtr);
    }

    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [num, &done_count]{
        return done_count.load() >= num;
    });
    bool completed = true;
    auto end_clock = std::chrono::steady_clock::now();

    if (!completed) {
        LOG_DXRT_ERR("RunBenchmark timeout: completed " << done_count.load() << "/" << num << " requests");
        RegisterCallback(nullptr);
        throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::InferenceEngine_TimeoutRunBenchmark()));
    }

    infTime = std::chrono::duration_cast<std::chrono::microseconds>(end_clock - start_clock).count();
    fps = 1000000.0 * infCnt/infTime;
    RegisterCallback(nullptr);
    return fps;
}

#ifdef _WIN32

// Need to check if RunBenchMarkWindows is required separately
// in windows, verbose mode
float InferenceEngine::RunBenchMarkWindows(int num, void* inputPtr)
{
    float sum = 0.;
    auto& profiler = dxrt::Profiler::GetInstance();
    std::vector<float> fps;

    std::atomic<int> done_count, i_last, done_todo;
    auto callBack = [&done_count, &i_last, &done_todo](const TensorPtrs& outputs, void* userArg) -> int {
        std::ignore = outputs;
        std::ignore = userArg;
        int userArgInt = reinterpret_cast<uint64_t>(userArg);

        done_count++;
        i_last = userArgInt;
        return 0;
        };  // callback used to count inference
    RegisterCallback(callBack);
    done_todo = 0;
    while (num > 0)
    {
        uint64_t infTime = 0;
        int infCnt = std::min(num, ObjectsPool::REQUEST_MAX_COUNT);
        done_count = 0; i_last = 0;
        profiler.Start("benchmark");
        auto start_clock = std::chrono::steady_clock::now();
        for (int i = 0; i < infCnt; i++)
        {
            RunAsync(inputPtr, reinterpret_cast<void*>(i));
        }
        while (done_count < infCnt)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        auto end_clock = std::chrono::steady_clock::now();
        profiler.End("benchmark");
        infTime = std::chrono::duration_cast<std::chrono::microseconds>(end_clock - start_clock).count();
        num -= infCnt;
        done_todo += infCnt;
        fps.emplace_back(1000000.0 * infCnt / infTime);
    }
    profiler.Erase("benchmark");
    for (const auto& val : fps)
    {
        sum += val;
        // cout << "fps: " << val << std::endl;
    }
    RegisterCallback(nullptr);
    return sum / fps.size();
}
#endif  // _WIN32


#ifdef DXRT_USE_DEVICE_VALIDATION

} // namespace dxrt

#include "dxrt/request_response_class.h"
#include "dxrt/tensor.h"

namespace dxrt
{

static std::mutex sValidationMutex;

TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{
    DXRT_ASSERT(_tasks.size() == 1, "ONLY ONE TASK IS ALLOWED WHEN VALIDATE DEVICE");

    // Return empty result if not in debug mode
    if (_modelCompileType != "debug") {
        LOG_DXRT << "Models compiled in release mode from DX-COM are not supported in validate_device."<< std::endl;
        return TensorPtrs();
    }
    std::lock_guard<std::mutex> lock(sValidationMutex);
    // Auto-split single input buffer for multi-input models if applicable
    if (shouldAutoSplitInput() && inputPtr != nullptr)
    {
        LOG_DBG("Auto-splitting single input buffer for multi-input model (validate)");

        // Get individual tensor sizes and split the input buffer
        auto tensorSizes = GetInputTensorSizes();
        std::vector<std::vector<uint8_t>> splitBuffers(tensorSizes.size());
        std::vector<void*> splitPtrs(tensorSizes.size());

        uint64_t offset = 0;
        for (size_t i = 0; i < tensorSizes.size(); ++i)
        {
            splitBuffers[i].resize(tensorSizes[i]);
            std::memcpy(splitBuffers[i].data(), static_cast<uint8_t*>(inputPtr) + offset, tensorSizes[i]);
            splitPtrs[i] = splitBuffers[i].data();
            offset += tensorSizes[i];
        }

        // Use multi-input validate API
        return ValidateDeviceMultiInput(splitPtrs, deviceId);
    }

    auto npuTaskIter = std::find_if(_tasks.begin(), _tasks.end(), [](const std::shared_ptr<dxrt::Task>& task) {
        return task->processor() == Processor::NPU;
    });

    if (npuTaskIter == _tasks.end()) {
        throw InvalidModelException(EXCEPTION_MESSAGE("No NPU task found for device validation"));
    }

    auto npuTask = *npuTaskIter;
    if (_option.devices.size() != 0)
    {
        if (_option.devices[0] != 0)
        {
            throw DeviceIOException(EXCEPTION_MESSAGE("device 0 not registered InferenceEngine, so cannot do validation"));
        }
    }

    if (_validationOutputBuffer.size() == 0) {
        size_t output_size = npuTask->getData()->_npuModel.output_all_size;

        if (output_size == 0) {
            // For models with output_all_size == 0,
            // allocate minimum buffer to ensure valid pointer (prevents nullptr issues)
            LOG_DXRT_WARN("ValidateDevice: output_all_size is 0 for model " << GetModelName()
                         << ". Allocating minimum buffer (1 byte) to prevent nullptr.");
            _validationOutputBuffer.resize(1);
        } else {
            _validationOutputBuffer.resize(output_size);
        }

        LOG_DXRT_DBG << "ValidateDevice: allocated output buffer of size "
                     << _validationOutputBuffer.size()
                     << " (output_all_size=" << output_size << ")" << std::endl;
    }

    auto req = Request::CreateValidateRequest(npuTask.get(), inputPtr, _validationOutputBuffer.data());
    req->SetStatus(Request::Status::REQ_BUSY);
    req->model_type() = req->taskData()->_npuModel.type;
    int ret_validation = RequestResponse::ValidateRequest(req);
    if (ret_validation != 0) {
        LOG_DXRT_ERR("Device validation failed with error code: " << ret_validation);
        throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation failed."));
    }
    TensorPtrs ret;
    ret.emplace_back(
            std::make_shared<Tensor>("output", std::vector<int64_t>{req->taskData()->_npuModel.output_all_size},
            DataType::INT8, _validationOutputBuffer.data()));

    return ret;
}


TensorPtrs InferenceEngine::ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId)
{
    if (inputPtrs.empty())
    {
        throw InvalidArgumentException("Input pointers vector cannot be empty");
    }

    // Check if this should be interpreted as multi-input
    if (_isMultiInput && inputPtrs.size() == _modelInputOrder.size())
    {

        // Interpret as multi-input
        LOG_DBG("ValidateDevice: Interpreting vector<void*> as multi-input - input count: " + std::to_string(inputPtrs.size()));
        return ValidateDeviceMultiInput(inputPtrs, deviceId);
    }

    // For single-input models, use first pointer
    LOG_DBG("ValidateDevice: Using traditional single-input approach");
    return ValidateDevice(inputPtrs[0], deviceId);
}


TensorPtrs InferenceEngine::ValidateDeviceMultiInput(const std::map<std::string, void*>& inputTensors, int deviceId)
{
    if (!_isMultiInput)
    {
        throw InvalidArgumentException("This model is not a multi-input model. Use ValidateDevice() instead.");
    }

    // Validate input tensor names
    for (const auto& pair : inputTensors)
    {
        if (_inputTensorToTaskMap.find(pair.first) == _inputTensorToTaskMap.end())
        {
            throw InvalidArgumentException("Unknown input tensor name: " + pair.first);
        }
    }

    // Check if all required input tensors are provided
    if (inputTensors.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input tensors, but got " + std::to_string(inputTensors.size()));
    }
    std::lock_guard<std::mutex> lock(sValidationMutex);

    auto npuTaskIter = std::find_if(_tasks.begin(), _tasks.end(), [](const std::shared_ptr<dxrt::Task>& task) {
        return task->processor() == Processor::NPU;
    });

    if (npuTaskIter == _tasks.end()) {
        throw InvalidModelException(EXCEPTION_MESSAGE("No NPU task found for device validation"));
    }

    auto npuTask = *npuTaskIter;

    auto devicePtr = DevicePool::GetInstance().GetDeviceTaskLayer(deviceId);


    if (_option.devices.size() != 0)
    {
        if (_option.devices[0] != 0)
        {
            throw DeviceIOException(EXCEPTION_MESSAGE("device 0 not registered InferenceEngine, so cannot do validation"));
        }
    }

    auto firstInput = inputTensors.begin();


    // Create a simplified request with multi-input data
    // For validation, we'll use the first input as the base and validate the NPU task
    if (_validationOutputBuffer.size() == 0) {
        _validationOutputBuffer.resize(npuTask->getData()->_npuModel.output_all_size);
    }

    auto req = Request::CreateValidateRequest(npuTask.get(), firstInput->second, _validationOutputBuffer.data());
    req->SetStatus(Request::Status::REQ_BUSY);
    req->model_type() = req->taskData()->_npuModel.type;
    int ret_validation = RequestResponse::ValidateRequest(req);
    if (ret_validation != 0) {
        LOG_DXRT_ERR("Device validation failed with error code: " << ret_validation);
        throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation failed."));
    }
    TensorPtrs ret;
    ret.emplace_back(
            std::make_shared<Tensor>("output", std::vector<int64_t>{req->taskData()->_npuModel.output_all_size},
            DataType::INT8, _validationOutputBuffer.data()));

    return ret;
}


TensorPtrs InferenceEngine::ValidateDeviceMultiInput(const std::vector<void*>& inputPtrs, int deviceId)
{
    if (inputPtrs.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input pointers, but got " + std::to_string(inputPtrs.size()));
    }

    // Convert vector to map using model input order
    std::map<std::string, void*> inputTensors;
    for (size_t i = 0; i < inputPtrs.size(); ++i)
    {
        inputTensors[_modelInputOrder[i]] = inputPtrs[i];
    }

    return ValidateDeviceMultiInput(inputTensors, deviceId);
}

#else
TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{
    std::ignore = inputPtr;
    std::ignore = deviceId;
    throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation is only supported in debug builds with DXRT_USE_DEVICE_VALIDATION enabled."));
}
TensorPtrs InferenceEngine::ValidateDevice(const std::vector<void*>& inputPtrs, int deviceId)
{
    std::ignore = inputPtrs;
    std::ignore = deviceId;
    throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation is only supported in debug builds with DXRT_USE_DEVICE_VALIDATION enabled."));
}
TensorPtrs InferenceEngine::ValidateDeviceMultiInput(const std::map<std::string, void*>& inputTensors, int deviceId)
{
    std::ignore = inputTensors;
    std::ignore = deviceId;
    throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation is only supported in debug builds with DXRT_USE_DEVICE_VALIDATION enabled."));
}
TensorPtrs InferenceEngine::ValidateDeviceMultiInput(const std::vector<void*>& inputPtrs, int deviceId)
{
    std::ignore = inputPtrs;
    std::ignore = deviceId;
    throw InvalidOperationException(EXCEPTION_MESSAGE("Device validation is only supported in debug builds with DXRT_USE_DEVICE_VALIDATION enabled."));
}

#endif
TensorPtrs InferenceEngine::Wait(int jobId)
{
    LOG_DXRT_DBG << jobId << std::endl;

    // std::shared_ptr<InferenceJob> infJob = ObjectsPool::GetInstance().GetInferenceJobById(jobId);
    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->GetById(jobId);
    if (infJob == nullptr)
    {
        std::string error_string = LogMessages::InferenceEngine_InvalidJobId(jobId);
        throw dxrt::InvalidOperationException(EXCEPTION_MESSAGE(error_string));
    }
    infJob->Wait();
    // this_thread::sleep_for(chrono::microseconds(1));
    // while (infJob->getStatus() == Request::Status::REQ_BUSY)
    // {    this_thread::sleep_for(chrono::microseconds(1)); }

    LOG_DXRT_DBG << jobId << " done" << std::endl;
    return infJob->getOutput();
}

Tensors InferenceEngine::GetInputs(void *ptr, uint64_t phyAddr)
{
    // Return only external input tensors (exclude intermediate tensors)
    // This ensures correct tensors for complex models where tasks may receive both
    // external inputs and intermediate tensors from other tasks
    Tensors externalInputs;
    uint64_t currentOffset = 0;

    for (const auto& inputTensorName : _modelInputOrder)
    {
        // Find the task that receives this input tensor
        auto taskNameIt = _inputTensorToTaskMap.find(inputTensorName);
        if (taskNameIt == _inputTensorToTaskMap.end())
        {
            LOG_DXRT_ERR("[GetInputs] Input tensor '" + inputTensorName + "' not found in task mapping");
            continue;
        }

        // Find the task
        auto taskIt = _taskMap.find(taskNameIt->second);
        if (taskIt == _taskMap.end())
        {
            LOG_DXRT_ERR("[GetInputs] Task '" + taskNameIt->second + "' not found");
            continue;
        }

        // Find the specific input tensor in the task's inputs
        auto taskInputs = taskIt->second->inputs();
        for (const auto& tensor : taskInputs)
        {
            if (tensor.name() == inputTensorName)
            {
                Tensor externalTensor = tensor;

                // Update data pointer and physical address if provided
                if (ptr != nullptr)
                {
                    externalTensor.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + currentOffset);
                    externalTensor.phy_addr() = phyAddr + currentOffset;
                    currentOffset += tensor.size_in_bytes();
                }

                externalInputs.push_back(externalTensor);
                LOG_DBG("[GetInputs] External input tensor '" + inputTensorName + "' added, size: " + std::to_string(tensor.size_in_bytes()));
                break;
            }
        }
    }

    LOG_DBG("[GetInputs] Total external input tensors: " + std::to_string(externalInputs.size()));
    return externalInputs;
}

std::vector<Tensors> InferenceEngine::GetInputs(int devId)
{

    auto devicePtr = DevicePool::GetInstance().GetDeviceTaskLayer(devId);
    if (devicePtr == nullptr)
    {
        throw DeviceIOException(EXCEPTION_MESSAGE("invalid device id"));
    }
    return devicePtr->inputs(_head->id());
}

Tensors InferenceEngine::GetOutputs(void *ptr, uint64_t phyAddr)
{
    // Use _lastOutputOrder for tensor ordering
    const std::vector<std::string>& outputTensorOrder = _lastOutputOrder;

    Tensors filteredTensors(outputTensorOrder.size(), Tensor("", {}, DataType::FLOAT));

    // Calculate cumulative offset for final output tensors in user buffer
    uint64_t cumulativeOffset = 0;
    std::map<std::string, uint64_t> finalTensorOffsets;

    for (const auto& tensorName : outputTensorOrder)
    {
        finalTensorOffsets[tensorName] = cumulativeOffset;

        // Find tensor size
        for (auto &task : _tasks)
        {
            for (const auto& tensor : task->outputs())
            {
                if (tensor.name() == tensorName)
                {
                    cumulativeOffset += tensor.size_in_bytes();
                    goto next_tensor;
                }
            }
        }
        next_tensor:;  // goto point
    }

    for (auto &task : _tasks)
    {
        TaskData* taskDataPtr = task->getData();
        Tensors outputTensors = taskDataPtr->output_tensors();

        if (ptr == nullptr) {
            for (size_t i = 0; i < outputTensorOrder.size(); i++)
            {
                for (Tensor &tensor : outputTensors)
                {
                    if (tensor.name() == outputTensorOrder[i]) {
                        filteredTensors[i] = tensor;
                    }
                }
            }
        }
        else
        {
            int i = 0;
            for (auto &t : outputTensors)
            {
                // Check if this tensor is a final output tensor
                auto finalOffsetIt = finalTensorOffsets.find(t.name());
                if (finalOffsetIt != finalTensorOffsets.end())
                {
                    // This is a final output tensor - use calculated offset
                    t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + finalOffsetIt->second);
                    t.phy_addr() = phyAddr + finalOffsetIt->second;
                }
                else
                {
                    // This is an intermediate tensor - use task offset
                    t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + taskDataPtr->_outputOffsets[i] + task->getTailOffset());
                    t.phy_addr() = phyAddr + taskDataPtr->_outputOffsets[i];
                }

                for (size_t j = 0; j < outputTensorOrder.size(); j++)
                {
                    if (t.name() == outputTensorOrder[j])
                    {
                        filteredTensors[j] = t;
                    }
                }
                i++;
            }
        }
    }

    return filteredTensors;
}


uint64_t InferenceEngine::GetInputSize()
{
    // Calculate size based on actual model input tensors only (exclude intermediate tensors)
    // This ensures correct size calculation for complex models where tasks may receive both
    // external inputs and intermediate tensors from other tasks
    uint64_t totalSize = 0;

    for (const auto& inputTensorName : _modelInputOrder)
    {
        // Find the task that receives this input tensor
        auto taskNameIt = _inputTensorToTaskMap.find(inputTensorName);
        if (taskNameIt == _inputTensorToTaskMap.end())
        {
            LOG_DXRT_ERR("[GetInputSize] Input tensor '" + inputTensorName + "' not found in task mapping");
            continue;
        }

        // Find the task
        auto taskIt = _taskMap.find(taskNameIt->second);
        if (taskIt == _taskMap.end())
        {
            LOG_DXRT_ERR("[GetInputSize] Task '" + taskNameIt->second + "' not found");
            continue;
        }

        // Find the specific input tensor in the task's inputs
        auto taskInputs = taskIt->second->inputs();
        for (const auto& tensor : taskInputs)
        {
            if (tensor.name() == inputTensorName)
            {
                totalSize += tensor.size_in_bytes();
                LOG_DBG("[GetInputSize] External input tensor '" + inputTensorName + "' size: " + std::to_string(tensor.size_in_bytes()));
                break;
            }
        }
    }

    LOG_DBG("[GetInputSize] Total external input size: " + std::to_string(totalSize));
    return totalSize;
}

std::vector<uint64_t> InferenceEngine::GetInputTensorSizes()
{
    std::vector<uint64_t> tensorSizes;
    tensorSizes.reserve(_modelInputOrder.size());

    for (const auto& inputTensorName : _modelInputOrder)
    {
        // Find the task that receives this input tensor
        auto taskNameIt = _inputTensorToTaskMap.find(inputTensorName);
        if (taskNameIt == _inputTensorToTaskMap.end())
        {
            LOG_DXRT_ERR("[GetInputTensorSizes] Input tensor '" + inputTensorName + "' not found in task mapping");
            continue;
        }

        // Find the task
        auto taskIt = _taskMap.find(taskNameIt->second);
        if (taskIt == _taskMap.end())
        {
            LOG_DXRT_ERR("[GetInputTensorSizes] Task '" + taskNameIt->second + "' not found");
            continue;
        }

        // Find the specific input tensor in the task's inputs
        auto taskInputs = taskIt->second->inputs();
        for (const auto& tensor : taskInputs)
        {
            if (tensor.name() == inputTensorName)
            {
                tensorSizes.push_back(tensor.size_in_bytes());
                LOG_DBG("[GetInputTensorSizes] Input tensor '" + inputTensorName + "' size: " + std::to_string(tensor.size_in_bytes()));
                break;
            }
        }
    }

    return tensorSizes;
}

std::vector<uint64_t> InferenceEngine::GetOutputTensorSizes()
{
    std::vector<uint64_t> tensorSizes;

    // Use _lastOutputOrder for tensor ordering
    const std::vector<std::string>& outputTensorOrder = _lastOutputOrder;

    tensorSizes.reserve(outputTensorOrder.size());

    for (const auto& outputTensorName : outputTensorOrder)
    {
        bool found = false;
        for (const auto& task : _tasks)
        {
            if (task->is_PPU())
            {
                // For PPU models, return single output size
                tensorSizes.push_back(task->output_size());
                found = true;
                break;
            }

            for (const auto& tensor : task->outputs())
            {
                if (tensor.name() == outputTensorName)
                {
                    uint64_t tensorSize = tensor.size_in_bytes();

                    // Check if this is a dynamic shape case or genuine zero size
                    if (tensorSize == 0) {
                        if (HasDynamicOutput()) {
                            LOG_DXRT_WARN("[GetOutputTensorSizes] Dynamic shape tensor '" + outputTensorName + "' returns size 0. "
                                          "Actual size will be available after inference execution.");
                        } else {
                            LOG_DBG("[GetOutputTensorSizes] Static tensor '" + outputTensorName + "' has zero size (empty tensor).");
                        }
                    }

                    tensorSizes.push_back(tensorSize);
                    LOG_DBG("[GetOutputTensorSizes] Tensor '" + outputTensorName + "' size: " + std::to_string(tensorSize));
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found)
        {
            LOG_DXRT_ERR("[GetOutputTensorSizes] Output tensor '" + outputTensorName + "' not found in tasks");
        }
    }

    return tensorSizes;
}

uint64_t InferenceEngine::GetOutputSize()
{
    // Check if any output has dynamic shape first
    if (HasDynamicOutput()) {
        LOG_DXRT_WARN("[DXRT] Dynamic shape model detected - GetOutputSize() returns 0. "
                      "Use nullptr for output buffer to enable auto-allocation.");
        return 0;
    }

    uint64_t outputSize = 0;

    // Use _lastOutputOrder for tensor ordering
    const std::vector<std::string>& outputTensorOrder = _lastOutputOrder;

    for (const auto &name : outputTensorOrder)
    {
        for (auto &task : _tasks)
        {
            if (task->is_PPU())
            {
                return task->output_size();
            }
            for (Tensor tensor : task->outputs())
            {
                if (tensor.name() == name)
                {
                    outputSize += tensor.size_in_bytes();
                }
            }
        }
    }
    return outputSize;
}

std::string InferenceEngine::GetModelName()
{
    return _name;
}

std::vector<std::string> InferenceEngine::GetTaskOrder()
{
    return _taskOrder;
}

int InferenceEngine::GetLatency()
{
    LOG_DXRT_DBG << std::endl;
    return _inferenceTimer.latency();
}

std::vector<int> InferenceEngine::GetLatencyVector()
{
    LOG_DXRT_DBG << std::endl;
    return _inferenceTimer.GetLatencyVector();
}

uint32_t InferenceEngine::GetNpuInferenceTime()
{
    LOG_DXRT_DBG << std::endl;
    return _inferenceTimer.inference_time();
}

std::vector<uint32_t> InferenceEngine::GetNpuInferenceTimeVector()
{
    LOG_DXRT_DBG << std::endl;
    return _inferenceTimer.GetNpuInferenceTimeVector();
}

double InferenceEngine::GetLatencyMean()
{
    return _inferenceTimer.GetLatencyMean();
}

double InferenceEngine::GetNpuInferenceTimeMean()
{
    return _inferenceTimer.GetNpuInferenceTimeMean();
}

double InferenceEngine::GetLatencyStdDev()
{
    return _inferenceTimer.GetLatencyStdDev();
}

double InferenceEngine::GetNpuInferenceTimeStdDev()
{
    return _inferenceTimer.GetNpuInferenceTimeStdDev();
}

int InferenceEngine::GetLatencyCnt()
{
    return _inferenceTimer.GetLatencyCnt();
}

int InferenceEngine::GetNpuInferenceTimeCnt()
{
    return _inferenceTimer.GetNpuInferenceTimeCnt();
}

std::vector<TensorPtrs> InferenceEngine::GetAllTaskOutputs()
{
    LOG_DXRT_DBG << "Collecting outputs from all tasks in order." << std::endl;
    std::vector<TensorPtrs> all_outputs;

    // Iterate through each task in the task order
    for (const auto& task_name : _taskOrder)
    {
        // Find the corresponding task in the task map
        auto it = _taskMap.find(task_name);
        if (it != _taskMap.end())
        {
            // Get the outputs of the task
            auto task = it->second;
            TensorPtrs task_outputs;
            for (auto& tensor : task->getLastOutput())
            {
                task_outputs.emplace_back(std::make_shared<Tensor>(tensor));
            }
            // Add the outputs to the list
            all_outputs.push_back(task_outputs);
        }
        #ifdef USE_ORT
        else
        {
            LOG_DXRT << "Task " << task_name << " not found in task map." << std::endl;
        }
        #endif
    }
    return all_outputs;
}

int InferenceEngine::GetNumTailTasks()
{
    return _numTails;
}

std::string InferenceEngine::GetCompileType()
{
    return _modelCompileType;
}

std::string InferenceEngine::GetModelVersion()
{
    return std::to_string(_modelData.deepx_binary._dxnnFileFormatVersion);
}

bool InferenceEngine::IsPPU()
{
    return _isPPU;
}

bool InferenceEngine::IsOrtConfigured()
{
#ifdef USE_ORT
    return _option.useORT;
#else
    if (_option.useORT)
    {
        throw dxrt::InvalidArgumentException("USE_ORT NOT SUPPORTED");
    }
    return false;
#endif
}

bool InferenceEngine::IsMultiInputModel() const
{
    return _isMultiInput;
}

int InferenceEngine::GetInputTensorCount() const
{
    return static_cast<int>(_modelInputOrder.size());
}

std::vector<std::string> InferenceEngine::GetInputTensorNames() const
{
    return _modelInputOrder;
}

std::vector<std::string> InferenceEngine::GetOutputTensorNames() const
{
    return _lastOutputOrder;
}

std::map<std::string, std::string> InferenceEngine::GetInputTensorToTaskMapping() const
{
    return _inputTensorToTaskMap;
}

TensorPtrs InferenceEngine::RunMultiInput(const std::map<std::string, void*>& inputTensors, void *userArg, void *outputPtr)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }

    if (!_isMultiInput)
    {
        throw InvalidArgumentException("This model is not a multi-input model. Use Run() instead.");
    }

    // Validate input tensor names
    for (const auto& pair : inputTensors)
    {
        if (_inputTensorToTaskMap.find(pair.first) == _inputTensorToTaskMap.end())
        {
            throw InvalidArgumentException("Unknown input tensor name: " + pair.first);
        }
    }

    // Check if all required input tensors are provided
    if (inputTensors.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input tensors, but got " + std::to_string(inputTensors.size()));
    }

    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    // Use multi-head setup if we have multiple input tasks, otherwise use traditional setup
    if (_inputTasks.size() > 1)
    {
        infJob->SetInferenceJobMultiHead(_tasks, _inputTasks, _lastOutputOrder, _modelInputOrder);
    }
    else
    {
        infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder, _modelInputOrder);
    }

    // Store outputs if user didn't register a callback
    if (_userCallback == nullptr)
    {
        infJob->SetStoreResult(true);
    }

    infJob->setInferenceEngineInterface(this);
    infJob->setCallBack([this](TensorPtrs &outputs, void *userArg, int jobId)->int{
        int retval = 0;
        if (_userCallback !=nullptr)
        {
            retval = _userCallback(outputs, userArg);
        }
        {
            _inferenceJobPool->GetById(jobId)->SetOccupiedJob(false);
        }

        return retval;
    });  // inference engine callback

    int jobId = infJob->startMultiInputJob(inputTensors, userArg, outputPtr);
    {
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(true);
    }
    return Wait(jobId);
}

TensorPtrs InferenceEngine::RunMultiInput(const std::vector<void*>& inputPtrs, void *userArg, void *outputPtr)
{
    if (inputPtrs.size() != _modelInputOrder.size())
    {
        throw InvalidArgumentException("Expected " + std::to_string(_modelInputOrder.size()) +
                                     " input pointers, but got " + std::to_string(inputPtrs.size()));
    }

    // Convert vector to map using model input order
    std::map<std::string, void*> inputTensors;
    for (size_t i = 0; i < inputPtrs.size(); ++i)
    {
        inputTensors[_modelInputOrder[i]] = inputPtrs[i];
    }

    return RunMultiInput(inputTensors, userArg, outputPtr);
}

void InferenceEngine::disposeOnce()
{
    std::lock_guard<std::mutex> lock(_sInferenceEngineMutex);

    _isDisposed = true;
    LOG_DXRT_DBG << std::endl;

    for (size_t i = 0; i < _inferenceJobPool->GetSize(); ++i)
    {
        auto job = _inferenceJobPool->GetById(i);

        // wait for the job to finish
        if ( job->GetOccupiedJob() ) {
            // lock.unlock();
            Wait(static_cast<int>(i));
            // lock.lock();
        }
        // job->Clear();
    }

    for (auto &task : _tasks)
    {
        // cout << *task << std::endl;
        // LOG_VALUE(task.use_count());
        task->prevs().clear();
        task->nexts().clear();
        task->ClearOutputBuffer();
        task->ClearEncodedInputBuffer();
        // task->getData()->_inputTensors.clear();
        // task->getData()->_outputTensors.clear();
    }
    _tasks.clear();
    _taskMap.clear();
    _head.reset();
    _tails.clear();
    _userCallback = nullptr;

    // inference job pool for IE
    _inferenceJobPool = nullptr;

    LOG_DXRT_DBG <<" Done"<< endl;
}
void InferenceEngine::Dispose()
{
    std::call_once(_disposeOnceFlag, [this]() { this->disposeOnce(); });
}

bool InferenceEngine::shouldAutoSplitInput() const
{
    return _isMultiInput && _inputTasks.size() == 1;
}

bool InferenceEngine::shouldUseUserOutputBuffer() const
{
    return _hasUserOutputBuffer && _userOutputPtr != nullptr;
}

std::vector<uint8_t> InferenceEngine::GetBitmatchMask(int index) {
    const std::vector<char>& maskBuffer = _modelData.deepx_binary.bitmatch_mask(index).buffer();
    std::vector<uint8_t> data(maskBuffer.begin(), maskBuffer.end());
    return data;
}

std::ostream& operator<<(std::ostream& os, const InferenceEngine& ie)
{
    os << "\n=== Model File: " << ie._name << " ===" << endl;

    // Print input tensor names
    os << "\nModel Input Tensors:" << endl;
    for (const auto& input : ie._modelInputOrder) {
        os << "  - " << input << endl;
    }

    // Print output tensor names
    os << "Model Output Tensors:" << endl;
    for (const auto& output : ie._lastOutputOrder) {
        os << "  - " << output << endl;
    }

    // Print tasks
    os << "\nTasks:" << endl;
    for (const auto& task_name : ie._taskOrder) {
        auto it = ie._taskMap.find(task_name);
        if (it != ie._taskMap.end()) {
            cout << "  [ ";
            for (const auto& prev : it->second->prevs())
            {
                cout << prev->name() << ", ";
            }
            cout << "] -> " << it->second->name() << " -> [";
            for (const auto& next : it->second->nexts())
            {
                cout << next->name() << ", ";
            }
            cout << "]" << endl;

            os << *(it->second) << endl;
        }
    }

    return os;
}

// Tensor-centric management implementation
void InferenceEngine::initializeEnvironmentVariables()
{
    const char* dxrt_debug_data_env = getenv("DXRT_DEBUG_DATA");
    const char* dxrt_show_profile_env = getenv("DXRT_SHOW_PROFILE");
    if (dxrt_debug_data_env != nullptr) {
        try {
            DEBUG_DATA = std::stoi(dxrt_debug_data_env);
        } catch (const std::invalid_argument&) {
            LOG_DXRT_ERR("Environment variable DXRT_DEBUG_DATA is not a valid integer.");
        } catch (const std::out_of_range&) {
            LOG_DXRT_ERR("Environment variable DXRT_DEBUG_DATA is out of range.");
        }
    }
    if (dxrt_show_profile_env != nullptr) {
        try {
            SHOW_PROFILE = std::stoi(dxrt_show_profile_env);
        } catch (const std::invalid_argument&) {
            LOG_DXRT_ERR("Environment variable DXRT_SHOW_PROFILE is not a valid integer.");
        } catch (const std::out_of_range&) {
            LOG_DXRT_ERR("Environment variable DXRT_SHOW_PROFILE is out of range.");
        }
    }
#ifdef USE_ORT
    if (_option.useORT == true)
    {
        CpuHandle::SetDynamicCpuThread();
    }
#else
    if (_option.useORT == true)
    {
        // Gracefully degrade when built without USE_ORT: disable CPU fallback instead of throwing.
        LOG_DXRT_ERR("[dxrt] Warning: USE_ORT is disabled in this build. Forcing useORT=false.");
        _option.useORT = false;
    }
#endif
}

void InferenceEngine::initializeModel()
{
    if (!dxrt::fileExists(_modelFile))
    {
        // DXRT_ASSERT(false, "Can't find " + _modelFile);
        throw dxrt::FileNotFoundException(_modelFile);
    }

    _modelFile = std::string(getAbsolutePath(_modelFile));
    _name = _modelFile;
    _modelCompileType = LoadModelParam(_modelData, _modelFile);
    if (_modelCompileType == "debug")
    {
            LOG << "NOTICE: Only one NPU task will run because the compile type is debug." << std::endl;
            _option.useORT = false;
    }
    _isOffloadingModel = _modelData.deepx_graph.use_offloading();
}

void InferenceEngine::buildTasksAndSubgraphMap()
{

    std::vector<std::string> orginal_task_order;
    orginal_task_order = _modelData.deepx_graph.topoSort_order();

    if (orginal_task_order.empty())
    {
        orginal_task_order.push_back(
            _modelData.deepx_binary.rmap_info(0).name()  // npu task name
        );
    }

    // Precompute lookup maps
    std::unordered_map<std::string, deepx_graphinfo::SubGraph> subGraphMap;
    for (auto& subGraph : _modelData.deepx_graph.subgraphs())
    {
        subGraphMap.emplace(subGraph.name(), subGraph);
    }

    std::unordered_map<std::string, size_t> rmapIndexMap;
    for (size_t j = 0; j < _modelData.deepx_binary.rmap_info().size(); ++j)
    {
        rmapIndexMap.emplace(_modelData.deepx_binary.rmap_info(j).name(), j);
    }

#ifdef USE_ORT
    std::unordered_map<std::string, size_t> cpuModelIndexMap;
        if (_option.useORT == true)
        {
            for (size_t j = 0; j < _modelData.deepx_binary.cpu_models().size(); ++j)
            {
                cpuModelIndexMap.emplace(_modelData.deepx_binary.cpu_models(j).name(), j);
            }
        }
#endif

    // Cache devices once

    int max_device_count = DevicePool::GetInstance().GetDeviceCount();

    std::vector<int> selected_devices = {};
    if (_option.devices.size() == 0)
    {
        for (int i = 0; i < max_device_count; i++)
            selected_devices.push_back(i);
    }
    else
    {
        for (int dev_id : _option.devices)
        {
            selected_devices.push_back(dev_id);
        }
    }

    bool found = false;
    for (const auto &order : orginal_task_order )
    {
        dxrt::rmap_info rmap_info;
        std::vector<std::vector<uint8_t>> data;
        found = false;

        // Populate subgraph if present
        auto subGraphIterator = subGraphMap.find(order);
        if (subGraphIterator != subGraphMap.end())
        {
            _subGraphMap.emplace(order, subGraphIterator->second);
        }

        // Try NPU rmap info
        auto rmapIterator = rmapIndexMap.find(order);
        if (rmapIterator != rmapIndexMap.end())
        {
            size_t j = rmapIterator->second;
            rmap_info = _modelData.deepx_rmap.rmap_info(j);

            // version check
            std::string version_str = _modelData.deepx_binary._compilerVersion;
            if ( !isSupporterModelVersion(version_str) )
                throw InvalidModelException(EXCEPTION_MESSAGE(LogMessages::NotSupported_ModelCompilerVersion(version_str, MIN_COMPILER_VERSION)));

            // Unrolled loop to avoid conditional branches and improve performance/readability
            const auto& rmapBuffer = _modelData.deepx_binary.rmap(j).buffer();
            data.emplace_back(rmapBuffer.begin(), rmapBuffer.end());
            if (data.back().empty())
            {
                throw InvalidModelException(EXCEPTION_MESSAGE("invalid model"));
            }

            const auto& weightBuffer = _modelData.deepx_binary.weight(j).buffer();
            data.emplace_back(weightBuffer.begin(), weightBuffer.end());

            // weight can be empty for some models
            //if (data.back().empty())
            //    throw InvalidModelException(EXCEPTION_MESSAGE("invalid model"));

            // v8: Add PPU binary if exists (for PPCPU type)
            if (_modelData.deepx_binary._dxnnFileFormatVersion == 8 &&
                j < _modelData.deepx_binary.ppu().size() &&
                _modelData.deepx_binary.ppu(j).size() > 0) {
                const auto& ppuBuffer = _modelData.deepx_binary.ppu(j).buffer();
                data.emplace_back(ppuBuffer.begin(), ppuBuffer.end());
                LOG_DXRT_DBG << "Added PPU binary to data vector for task '" << order
                             << "', size: " << data.back().size() << " bytes" << std::endl;
            }

            found = true;
        }

#ifdef USE_ORT
        if ((found == false) && (_option.useORT == true))
        {
            // Try CPU model
            auto cpuIterator = cpuModelIndexMap.find(order);
            if (cpuIterator != cpuModelIndexMap.end())
            {
                const auto& bufferSource = _modelData.deepx_binary.cpu_models(cpuIterator->second).buffer();
                data.emplace_back(bufferSource.begin(), bufferSource.end());
                found = true;
            }
        }
#endif
        // DXRT_ASSERT(found==true, "invalid graph info in model");
        if (found)
        {
            // Check if this task has PPU binary (v8 PPCPU type)
            bool hasPpuBinary = false;
            if (_modelData.deepx_binary._dxnnFileFormatVersion == 8) {
                auto rmapIterator = rmapIndexMap.find(order);
                if (rmapIterator != rmapIndexMap.end()) {
                    size_t j = rmapIterator->second;
                    if (j < _modelData.deepx_binary.ppu().size() &&
                        _modelData.deepx_binary.ppu(j).size() > 0) {
                        hasPpuBinary = true;
                    }
                }
            }
            std::shared_ptr<Task> task;
            if (_option.devices.size() != 0)
            {
                task = std::make_shared<Task>(order, rmap_info, std::move(data),
                    static_cast<npu_bound_op>(_option.boundOption), _option.devices, hasPpuBinary);
            }
            else
            {
                task = std::make_shared<Task>(order, rmap_info, std::move(data),
                    static_cast<npu_bound_op>(_option.boundOption), hasPpuBinary);
            }
            _tasks.emplace_back(task);

#ifdef USE_ORT
            if (_option.useORT == true)
            {
                auto &subgraph = _subGraphMap[order];

                // Use head/tail flags directly from graph_info
                if (subgraph.head())
                {
                    if (_head == nullptr)
                    {
                        _head = task;
                    }
                    else
                    {
                        LOG_DBG("Multi-head model detected: Additional head task '" + task->name() + "'");
                    }
                    task->set_head();
                }

                if (subgraph.tail())
                {
                    task->set_tail();
                    _tails.push_back(task);
                }
            }
            else
#endif
            {
                _head = task;
                task->set_head();
                _tails.push_back(task);
                task->set_tail();
            }
            _taskMap[task->name()] = task;
            _taskOrder.push_back(task->name());
#ifdef USE_ORT
            if (_option.useORT == false)
#endif
                break;  // force single task
        }
    }
    DXRT_ASSERT(found == true, "invalid graph info in model");
}

void InferenceEngine::buildInputTensorMapping()
{
    // Build input tensor to task mapping
    #ifdef USE_ORT
    if (_option.useORT == true)
    {
        // ORT mode: use subgraph inputs with owner check
        for (const auto &tensorName : _modelInputOrder)
        {
            // Find which task this input tensor belongs to
            for (auto &task : _tasks)
            {
                auto &subgraph = _subGraphMap[task->name()];
                auto &inputs = subgraph.inputs();

                for (auto &inputTensor : inputs)
                {
                    if (inputTensor.name() == tensorName && inputTensor.owner().empty())
                    {
                        // This is a model input tensor (no owner means it's an external input)
                        _inputTensorToTaskMap[tensorName] = task->name();

                        // Add to input tasks if not already present
                        auto it = std::find(_inputTasks.begin(), _inputTasks.end(), task);
                        if (it == _inputTasks.end())
                        {
                            _inputTasks.push_back(task);
                        }
                        break;
                    }
                }
            }
        }
    }
    else
    #endif
    {
        // Non-ORT mode: directly map all head task inputs
        for (auto &task : _tasks)
        {
            if (task->is_head())
            {
                for (auto& input : task->inputs())
                {
                    _inputTensorToTaskMap[input.name()] = task->name();

                    // Add to input tasks if not already present
                    auto it = std::find(_inputTasks.begin(), _inputTasks.end(), task);
                    if (it == _inputTasks.end())
                    {
                        _inputTasks.push_back(task);
                    }
                }
            }
        }
    }
}

void InferenceEngine::buildTaskGraph()
{
        // task chain
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it)
    {
        auto elem = *it;
        if (next(it) != _tasks.end())
            elem->next() = *(next(it));
        else
            elem->next() = nullptr;
    }

    for (auto &task : _tasks)
    {
        auto &subgraph = _subGraphMap[task->name()];
        auto &inputs = subgraph.inputs();
        auto &outputs = subgraph.outputs();
        // cout << subgraph.name() << endl;
        std::ignore = inputs;  // for(auto &v:inputs) cout << v.key() << ", " << v.val() << endl;
        std::ignore = outputs;  // for(auto &v:outputs) cout << v.key() << ", " << v.val() << endl;

        {
            auto &nexts = task->nexts();

            for (auto &tensor : outputs) {
                std::string tensor_name = tensor.name();
                // std::vecotr<std::string> user_task_names = tensor.users();
                for (auto user_task_name : tensor.users())
                {
                    auto user_task = _taskMap.find(user_task_name);

                    if (user_task != _taskMap.end())
                    {
                        auto it = std::find(nexts.begin(), nexts.end(), user_task->second);

                        if (it == nexts.end()) {
                            nexts.emplace_back(user_task->second);
                        }
                        LOG_DBG("[OUTPUT][" + task->name() + "] tensor : " + tensor_name + " / next task : " + user_task_name);
                    }
                }
            }
        }

        // Build prevs for all tasks (including head tasks that may depend on other tasks)
        // Head tasks can still have dependencies on other tasks even though they also receive model inputs
        {
            auto &prevs = task->prevs();
            for (auto &tensor : inputs)
            {
                std::string tensor_name = tensor.name();
                std::string owner_task_name = tensor.owner();

                // Skip model input tensors (owner is empty) - these are provided by user, not from previous tasks
                if (owner_task_name.empty())
                {
                    LOG_DBG("[INPUT][" + task->name() + "] Tensor '" + tensor_name + "' is model input (no owner), skipping prev task linkage");
                    continue;
                }

                auto owner_task_it = _taskMap.find(owner_task_name);
                if (owner_task_it == _taskMap.end())
                {
                    LOG_DXRT_ERR("[buildTaskGraph] Owner task '" + owner_task_name + "' not found for tensor '" + tensor_name + "'");
                    continue;
                }

                auto owner_task = owner_task_it->second;
                auto it = std::find(prevs.begin(), prevs.end(), owner_task);

                if (it == prevs.end()) {
                    prevs.emplace_back(owner_task);
                }
                LOG_DBG("[INPUT][" + task->name() + "] Tensorname : " + tensor_name + " / prev task : " + owner_task_name);
            }
        }
        task->SetInferenceEngineTimer(&_inferenceTimer);
        if(task->is_PPU())
        {
            _isPPU = true;
        }
    }
}

void InferenceEngine::buildTensorRegistry()
{
    LOG_DBG("Building tensor registry for comprehensive tensor management");

    _tensorRegistry.clear();

    // Step 1: Identify all tensors in the model
    std::set<std::string> allTensorNames;

    // Collect all input and output tensor names from all tasks
    for (const auto& task : _tasks)
    {
        // Process input tensors
        for (const auto& input : task->inputs())
        {
            allTensorNames.insert(input.name());
        }

        // Process output tensors
        for (const auto& output : task->outputs())
        {
            allTensorNames.insert(output.name());
        }
    }

    // Step 2: Build tensor descriptors
    for (const std::string& tensorName : allTensorNames)
    {
        TensorDescriptor descriptor(tensorName, "");

        // Find producer task
        for (const auto& task : _tasks)
        {
            for (const auto& output : task->outputs())
            {
                if (output.name() == tensorName)
                {
                    descriptor.producerTask = task->name();
                    descriptor.sizeInBytes = output.size_in_bytes();
                    break;
                }
            }
            if (!descriptor.producerTask.empty()) break;
        }

        // Find consumer tasks
        for (const auto& task : _tasks)
        {
            for (const auto& input : task->inputs())
            {
                if (input.name() == tensorName)
                {
                    descriptor.consumerTasks.push_back(task->name());
                }
            }
        }

        // Determine if it's a model input or output
        descriptor.isModelInput = (std::find(_modelInputOrder.begin(), _modelInputOrder.end(), tensorName) != _modelInputOrder.end());

        // Check if tensor is a final model output
        // A tensor is a final output ONLY if it's in the _lastOutputOrder
        // This ensures we only include tensors that should be returned to the user
        descriptor.isModelOutput = (std::find(_lastOutputOrder.begin(), _lastOutputOrder.end(), tensorName) != _lastOutputOrder.end());

        _tensorRegistry[tensorName] = descriptor;

        LOG_DBG("Tensor '" + tensorName +
                "': producer=" + descriptor.producerTask +
                ", consumers=" + std::to_string(descriptor.consumerTasks.size()) +
                ", modelInput=" + (descriptor.isModelInput ? "true" : "false") +
                ", modelOutput=" + (descriptor.isModelOutput ? "true" : "false") +
                ", size=" + std::to_string(descriptor.sizeInBytes));
    }

    LOG_DBG("Tensor registry built with " + std::to_string(_tensorRegistry.size()) + " tensors");
}

void InferenceEngine::calculateTensorOffsets()
{
    LOG_DBG("Calculating tensor offsets for final output buffer");

    std::lock_guard<std::mutex> lock(_outputBufferMutex);

    if (_outputOffsetsCalculated.load()) {
        LOG_DBG("Output offsets already calculated, skipping");
        return;
    }

    _cachedOutputOffsets.clear();
    uint64_t currentOffset = 0;

    // Calculate offsets based on _lastOutputOrder
    for (const std::string& tensorName : _lastOutputOrder)
    {
        auto it = _tensorRegistry.find(tensorName);
        if (it != _tensorRegistry.end())
        {
            it->second.outputBufferOffset = currentOffset;
            _cachedOutputOffsets[tensorName] = currentOffset;
            currentOffset += it->second.sizeInBytes;

            LOG_DBG("Tensor '" + tensorName + "' offset: " + std::to_string(it->second.outputBufferOffset) +
                    ", size: " + std::to_string(it->second.sizeInBytes));
        }
        else
        {
            LOG_DXRT_ERR("Tensor '" + tensorName + "' not found in registry while calculating offsets");
        }
    }

    _outputOffsetsCalculated.store(true);
    LOG_DBG("Total output buffer size needed: " + std::to_string(currentOffset) + " bytes");
}

size_t InferenceEngine::GetOutputTensorOffset(const std::string& tensorName) const
{
    // Ensure offsets are calculated first
    if (!_outputOffsetsCalculated.load()) {
        const_cast<InferenceEngine*>(this)->calculateTensorOffsets();
    }

    std::lock_guard<std::mutex> lock(_outputBufferMutex);
    auto it = _cachedOutputOffsets.find(tensorName);
    if (it != _cachedOutputOffsets.end())
    {
        return static_cast<size_t>(it->second);
    }

    LOG_DXRT_ERR("Tensor '" + tensorName + "' not found in cached offsets");
    return 0;
}

bool InferenceEngine::isTensorModelOutput(const std::string& tensorName) const
{
    auto it = _tensorRegistry.find(tensorName);
    return (it != _tensorRegistry.end()) && it->second.isModelOutput;
}

bool InferenceEngine::isTensorModelInput(const std::string& tensorName) const
{
    auto it = _tensorRegistry.find(tensorName);
    return (it != _tensorRegistry.end()) && it->second.isModelInput;
}

bool InferenceEngine::supportsTensorCentricOffsets() const
{
    // Return true if tensor registry is built and has output tensors
    return !_tensorRegistry.empty() && !_lastOutputOrder.empty();
}

void InferenceEngine::LogModelDataDetails()
{
    LOG_DXRT << "=== MODEL DATA DETAILS ===" << endl;

    // 1. Binary Info
    LOG_DXRT << "[BINARY_INFO] Compiler Version: " << _modelData.deepx_binary._compilerVersion << endl;
    LOG_DXRT << "[BINARY_INFO] Graph Info Offset: " << _modelData.deepx_binary.graph_info().offset() << endl;
    LOG_DXRT << "[BINARY_INFO] Graph Info Size: " << _modelData.deepx_binary.graph_info().size() << endl;

    // Rmap Info
    LOG_DXRT << "[BINARY_INFO] Rmap Info Count: " << _modelData.deepx_binary.rmap_info().size() << endl;
    for (size_t i = 0; i < _modelData.deepx_binary.rmap_info().size(); ++i) {
        LOG_DXRT << "[BINARY_INFO] Rmap[" << i << "] Name: " << _modelData.deepx_binary.rmap_info(i).name() << endl;
        LOG_DXRT << "[BINARY_INFO] Rmap[" << i << "] Offset: " << _modelData.deepx_binary.rmap_info(i).offset() << endl;
        LOG_DXRT << "[BINARY_INFO] Rmap[" << i << "] Size: " << _modelData.deepx_binary.rmap_info(i).size() << endl;
    }

    // Weight Info
    LOG_DXRT << "[BINARY_INFO] Weight Info Count: " << _modelData.deepx_binary.weight().size() << endl;
    for (size_t i = 0; i < _modelData.deepx_binary.weight().size(); ++i) {
        LOG_DXRT << "[BINARY_INFO] Weight[" << i << "] Name: " << _modelData.deepx_binary.weight(i).name() << endl;
        LOG_DXRT << "[BINARY_INFO] Weight[" << i << "] Offset: " << _modelData.deepx_binary.weight(i).offset() << endl;
        LOG_DXRT << "[BINARY_INFO] Weight[" << i << "] Size: " << _modelData.deepx_binary.weight(i).size() << endl;
    }

    // 2. Graph Info
    LOG_DXRT << "[GRAPH_INFO] Subgraphs Count: " << _modelData.deepx_graph.subgraphs().size() << endl;
    for (size_t i = 0; i < _modelData.deepx_graph.subgraphs().size(); ++i) {
        auto& subgraph = _modelData.deepx_graph.subgraphs(i);
        LOG_DXRT << "[GRAPH_INFO] Subgraph[" << i << "] Name: " << subgraph.name() << endl;
        LOG_DXRT << "[GRAPH_INFO] Subgraph[" << i << "] Inputs Count: " << subgraph.inputs().size() << endl;
        LOG_DXRT << "[GRAPH_INFO] Subgraph[" << i << "] Outputs Count: " << subgraph.outputs().size() << endl;

        for (size_t j = 0; j < subgraph.inputs().size(); ++j) {
            LOG_DXRT << "[GRAPH_INFO] Subgraph[" << i << "] Input[" << j << "] Name: " << subgraph.inputs(j).name() << endl;
        }

        for (size_t j = 0; j < subgraph.outputs().size(); ++j) {
            LOG_DXRT << "[GRAPH_INFO] Subgraph[" << i << "] Output[" << j << "] Name: " << subgraph.outputs(j).name() << endl;
        }
    }

    // 3. Rmap Info
    LOG_DXRT << "[RMAP_INFO] Rmap Info Count: " << _modelData.deepx_rmap.rmap_info().size() << endl;
    for (size_t i = 0; i < _modelData.deepx_rmap.rmap_info().size(); ++i) {
        auto& rmap = _modelData.deepx_rmap.rmap_info(i);
        LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Name: " << rmap.name() << endl;
        LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Input Count: " << rmap.inputs().size() << endl;
        LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Output Count: " << rmap.outputs().size() << endl;

        for (size_t j = 0; j < rmap.inputs().size(); ++j) {
            auto& input = rmap.inputs()[j];
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Input[" << j << "] Name: " << input.name() << endl;
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Input[" << j << "] Memory Offset: " << input.memory().offset() << endl;
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Input[" << j << "] Memory Size: " << input.memory().size() << endl;
        }

        for (size_t j = 0; j < rmap.outputs().size(); ++j) {
            auto& output = rmap.outputs()[j];
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Output[" << j << "] Name: " << output.name() << endl;
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Output[" << j << "] Memory Offset: " << output.memory().offset() << endl;
            LOG_DXRT << "[RMAP_INFO] Rmap[" << i << "] Output[" << j << "] Memory Size: " << output.memory().size() << endl;
        }
    }

    LOG_DXRT << "=== END MODEL DATA DETAILS ===" << endl;
}

bool InferenceEngine::HasDynamicOutput()
{
    // Check if any task has dynamic outputs
    for (auto &task : _tasks)
    {
        // For CPU tasks, check if they have dynamic shape outputs
        CpuHandle* cpuHandle = task->getCpuHandle();
    if (cpuHandle && cpuHandle->HasDynamicOutput())
        {
            return true;
        }

        // For non-CPU tasks, check tensor shapes directly
        for (const Tensor& tensor : task->outputs())
        {
            // Access private _shape member directly since const version of shape() doesn't exist
            // Check if any dimension is <= 0 (dynamic) by examining size_in_bytes behavior
            if (tensor.size_in_bytes() == 0)  // This indicates dynamic shape due to negative dimensions
            {
                return true;
            }
        }
    }
    return false;
}



}  // namespace dxrt
