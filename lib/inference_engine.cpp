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
#include "dxrt/device.h"
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

struct BatchArgument
{
    void* userArg;
    int resultIndex;
};

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
#ifdef USE_ORT
    if (_option.useORT == true)
    {
        _lastOutputOrder = _modelData.deepx_graph.outputs();
    }
    else
#endif
    {
        _lastOutputOrder.clear();
    }
    _numTails = 0;

    // Step 1: Collect all tail tasks and their outputs in _lastOutputOrder
    std::vector<std::pair<TaskPtr, std::vector<std::string>>> tailTaskOutputs;
    for (auto &task : _tasks)
    {
        if (task->is_tail())
        {
            std::vector<std::string> taskOutputNames;
            for (auto& output : task->outputs())
            {
                taskOutputNames.push_back(output.name());
#ifdef USE_ORT
                if (_option.useORT == false)
                    _lastOutputOrder.push_back(output.name());
#else
                _lastOutputOrder.push_back(output.name());
#endif
            }
            tailTaskOutputs.push_back({task, taskOutputNames});
            _numTails++;
        }
    }
    // Temp. CODE for v7
    if (_isPPU)
    {
        // For PPU models, keep the _tailOffset settings but rebuild _lastOutputOrder
        // to ensure consistency
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

        // Only update if the order is different
        if (newLastOutputOrder != _lastOutputOrder)
        {
            LOG_DBG("PPU model: Updating _lastOutputOrder for consistency");
            _lastOutputOrder = newLastOutputOrder;

            // Recalculate tailOffsets for PPU consistency
            int64_t ppuOffset = 0;
            for (auto &task : _tasks)
            {
                if (task->is_tail())
                {
                    task->setTailOffset(ppuOffset);
                    ppuOffset += task->output_size();
                }
            }
        }
    }
    // Step 2: Calculate tailOffset based on _lastOutputOrder sequence
    // Create mapping from tensor name to its position in _lastOutputOrder
    std::map<std::string, size_t> tensorOrderMap;
    for (size_t i = 0; i < _lastOutputOrder.size(); ++i)
    {
        tensorOrderMap[_lastOutputOrder[i]] = i;
    }

    // Step 3: Set tailOffset for each task based on cumulative tensor sizes in _lastOutputOrder
    std::map<TaskPtr, int64_t> taskOffsetMap;

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
        taskOffsetMap[task] = taskOffset;

        LOG_DBG("Task '" + task->name() + "' tailOffset set to: " + std::to_string(taskOffset));
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

    infJob->DSP_SetDspEnable(0);
    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
    infJob->setInferenceEngineInterface(this);
    infJob->SetStoreResult(true);
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

    return runAsync(inputPtr, userArg, outputPtr, nullptr);
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
    return RunAsync(inputPtrs[0], userArg, outputPtr);
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
        infJob->SetInferenceJobMultiHead(_tasks, _inputTasks, _lastOutputOrder);
    }
    else
    {
        infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
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
        // argruments
        std::vector<BatchArgument> batch_args(SUB_BATCH_MAX_COUNT);

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
                    batch_args.data(), inputBuffers, outputBuffers, userArgs);

                start_index += SUB_BATCH_MAX_COUNT;
            }  // for i
        }

        if ( sub_batch_remain > 0 )
        {
            runSubBatch(result, sub_batch_remain, start_index,
                batch_args.data(), inputBuffers, outputBuffers, userArgs);
        }

        batch_args.clear();

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
        void* batchArgs,
        const std::vector<void*>& inputBuffers,
        const std::vector<void*>& outputBuffers,
        const std::vector<void*>& userArgs
    )
{

    BatchArgument* batch_args_array = reinterpret_cast<BatchArgument*>(batchArgs);

    std::atomic<int> complete_count{0};
    std::mutex mtx_cv;  // mutex lock
    std::condition_variable cv_complete;  // complete condition variable

    auto batch_callback = [&complete_count, &cv_complete, &mtx_cv, &result, batchCount](TensorPtrs &outputs, void *userArg, int jobId) {

            // std::ignore = userArg; // reserved
            BatchArgument* batch_arg = reinterpret_cast<BatchArgument*>(userArg);
            if ( batch_arg == nullptr )
            {
                throw dxrt::InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::InferenceEngine_BatchArgumentIsNull()));
            }

            int batch_index = -1;

            try {
                // find batch_index by jobId
                batch_index = batch_arg->resultIndex;
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
            if ( complete_count.load() == batchCount )
            {
                std::unique_lock<std::mutex> lock(mtx_cv);
                cv_complete.notify_one();
                LOG_DXRT_DBG << "runAsync completed" << std::endl;
            }
        };

    try
    {
        // Run asynchronous operations for each batch
        for (int i = 0; i < batchCount; ++i)
        {
            BatchArgument* batchArg = reinterpret_cast<BatchArgument*>(&batch_args_array[i]);
            void* userArg = userArgs.size() > 0 ? userArgs.at(i) : nullptr;
            int current_index = startIndex + i;

            batchArg->userArg = userArg;
            batchArg->resultIndex = current_index;
            int job_id = runAsync(inputBuffers.at(current_index), batchArg, outputBuffers.at(current_index), batch_callback);

            // std::cout << "runAsync index=" << current_index << std::endl;
            // std::cout << "inputPtrs size=" << inputPtrs.size() << " OutputPtrs size=" << pOutputPtrs->size() << std::endl;

            // map_insert(job_id, i);
            LOG_DXRT_DBG << "Insert jobId=" << job_id << ", batch_index=" << i << std::endl;

        }  // for i

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
int InferenceEngine::runAsync(void *inputPtr, void *userArg, void *outputPtr, 
    std::function<void(TensorPtrs &outputs, void *userArg, int jobId)> batchCallback)
{
    if (_isDisposed)
    {
        throw InvalidOperationException("InferenceEngine already Disposed");
    }
    // return InferenceJob instance from InferenceJob pool (reused)
    // std::shared_ptr<InferenceJob> infJob = ObjectsPool::GetInstance().PickInferenceJob();
    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    infJob->DSP_SetDspEnable(0);
    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
    infJob->setInferenceEngineInterface(this);
    infJob->setCallBack([this, batchCallback](TensorPtrs &outputs, void *userArg, int jobId)->int{

            int retval = 0;
            if (this->_userCallback != nullptr)
            {
                if ( batchCallback != nullptr && userArg != nullptr )
                {
                    retval = this->_userCallback(outputs,
                        (reinterpret_cast<BatchArgument*>(userArg))->userArg);
                }
                else
                {
                    retval = this->_userCallback(outputs, userArg);
                }
            }

            // LOG_DXRT << "InferenceJob Callback-1 jobId=" << jobId << std::endl;
            if ( batchCallback != nullptr )
            {
                batchCallback(outputs, userArg, jobId);
            }
            {
                _inferenceJobPool->GetById(jobId)->SetOccupiedJob(false);
            }

            return retval;
        });  // inference engine callback

    if (_userCallback == nullptr)
    {
        infJob->SetStoreResult(true);
    }
    // if(infJob->getId()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || infJob->getId()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
    //    cout<<"[PROC         ][Job_"<<infJob->getId()<<"] Start"<<endl;

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
        // cout << "BenchMark(" << ((int)userArg) << ")" << std::endl;
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
        // profiler.Start("req");
        for (int i = 0; i < infCnt; i++)
        {
            RunAsync(inputPtr, reinterpret_cast<void*>(i));
        }
        // profiler.End("req");
        while (done_count < infCnt)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        // while (done_count < infCnt || (i_last!=(infCnt-1)) )continue;
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

TensorPtrs InferenceEngine::ValidateDevice(void *inputPtr, int deviceId)
{

    // Return empty result if not in debug mode
    if (_modelCompileType != "debug") {
        LOG_DXRT << "Models compiled in release mode from DX-COM are not supported in validate_device."<< std::endl;
        return TensorPtrs();
    }

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

    Device::_sNpuValidateOpt.store(true);

    auto npuTaskIter = std::find_if(_tasks.begin(), _tasks.end(), [](const std::shared_ptr<dxrt::Task>& task) {
        return task->processor() == Processor::NPU;
    });

    if (npuTaskIter == _tasks.end()) {
        throw InvalidModelException(EXCEPTION_MESSAGE("No NPU task found for device validation"));
    }

    auto npuTask = *npuTaskIter;

    std::vector<dxrt::DevicePtr>& devices = dxrt::CheckDevices();
    if (static_cast<size_t>(deviceId) >= devices.size()) {
        throw DeviceIOException(EXCEPTION_MESSAGE("invalid device id"));
    }
    auto req = Request::Create(npuTask.get(), inputPtr, nullptr, nullptr);
    req->setInferenceJob(nullptr);
    req->SetStatus(Request::Status::REQ_BUSY);
    req->DSP_SetDspEnable(0);
    req->model_type() = req->taskData()->_npuModel.type;
    TensorPtrs ret = devices[deviceId]->Validate(req, false);
    Device::_sNpuValidateOpt.store(false);
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

    Device::_sNpuValidateOpt.store(true);

    auto npuTaskIter = std::find_if(_tasks.begin(), _tasks.end(), [](const std::shared_ptr<dxrt::Task>& task) {
        return task->processor() == Processor::NPU;
    });

    if (npuTaskIter == _tasks.end()) {
        throw InvalidModelException(EXCEPTION_MESSAGE("No NPU task found for device validation"));
    }

    auto npuTask = *npuTaskIter;

    std::vector<dxrt::DevicePtr>& devices = dxrt::CheckDevices();
    if (static_cast<size_t>(deviceId) >= devices.size()) {
        throw DeviceIOException(EXCEPTION_MESSAGE("invalid device id"));
    }

    // Create a simplified request with multi-input data
    // For validation, we'll use the first input as the base and validate the NPU task
    auto firstInput = inputTensors.begin();
    auto req = Request::Create(npuTask.get(), firstInput->second, nullptr, nullptr);
    req->setInferenceJob(nullptr);
    req->SetStatus(Request::Status::REQ_BUSY);
    req->model_type() = req->taskData()->_npuModel.type;

    TensorPtrs ret = devices[deviceId]->Validate(req, false);
    Device::_sNpuValidateOpt.store(false);
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
    std::vector<dxrt::DevicePtr>& devices = dxrt::CheckDevices();
    if (devices.empty()) return {};
    auto device = devices[devId];
    return device->inputs(_head->id());
}

Tensors InferenceEngine::GetOutputs(void *ptr, uint64_t phyAddr)
{
    // Use the same tensor order logic as GetOutputTensorNames() for consistency
    std::vector<std::string> outputTensorOrder;
    if (!_finalOutputOrder.empty())
    {
        outputTensorOrder = _finalOutputOrder;
    }
    else
    {
        outputTensorOrder = _lastOutputOrder;
    }

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
        TaskData* tempTaskDataPtr = task->getData();
        Tensors tempTensors = tempTaskDataPtr->output_tensors();

        if (ptr == nullptr) {
            for (size_t i = 0; i < outputTensorOrder.size(); i++)
            {
                for (Tensor &tensor : tempTensors)
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
            for (auto &t : tempTensors)
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
                    t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + tempTaskDataPtr->_outputOffsets[i] + task->getTailOffset());
                    t.phy_addr() = phyAddr + tempTaskDataPtr->_outputOffsets[i];
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

    // Use the same tensor order logic as GetOutputTensorNames() for consistency
    std::vector<std::string> outputTensorOrder;
    if (!_finalOutputOrder.empty())
    {
        outputTensorOrder = _finalOutputOrder;
    }
    else
    {
        outputTensorOrder = _lastOutputOrder;
    }

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
                    tensorSizes.push_back(tensor.size_in_bytes());
                    LOG_DBG("[GetOutputTensorSizes] Output tensor '" + outputTensorName + "' size: " + std::to_string(tensor.size_in_bytes()));
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
    uint64_t outputSize = 0;

    // Use the same tensor order logic as GetOutputTensorNames() for consistency
    std::vector<std::string> outputTensorOrder;
    if (!_finalOutputOrder.empty())
    {
        outputTensorOrder = _finalOutputOrder;
    }
    else
    {
        outputTensorOrder = _lastOutputOrder;
    }

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
    // Use tensor-centric final output order if available, otherwise fall back to legacy
    if (!_finalOutputOrder.empty())
    {
        return _finalOutputOrder;
    }
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
        infJob->SetInferenceJobMultiHead(_tasks, _inputTasks, _lastOutputOrder);
    }
    else
    {
        infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);
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
    // Debug logging for model data comparison
    // LogModelDataDetails();
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
    std::vector<dxrt::DevicePtr>& devices = CheckDevices();
    std::vector<dxrt::DevicePtr> selected_devices = {};
    if (_option.devices.size() == 0)
    {
        selected_devices = devices;
    }
    else
    {
        for (int dev_id : _option.devices)
        {
            selected_devices.push_back(devices[dev_id]);
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
            if (data.back().empty())
                throw InvalidModelException(EXCEPTION_MESSAGE("invalid model"));

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
            auto task = std::make_shared<Task>(order, rmap_info, std::move(data),
                static_cast<npu_bound_op>(_option.boundOption), selected_devices);
            _tasks.emplace_back(task);

#ifdef USE_ORT
            if (_option.useORT == true)
            {
                auto &subraph = _subGraphMap[order];

                for (const auto& tensor : subraph.inputs())
                {
                    if (tensor.owner().empty())
                    {
                        if (_head == nullptr)
                        {
                            _head = task;
                            task->set_head();
                        }
                        else
                        {
                            task->set_head();
                            LOG_DBG("Multi-head model detected: Additional head task '" + task->name() + "'");
                        }
                    }
                }
                bool all_outputs_have_no_valid_users = true;
                for (const auto& tensor : subraph.outputs())
                {
                    bool has_valid_user = false;
                    for (const auto& user : tensor.users()) {
                        // Check if user exists in original task order
                        if (std::find(orginal_task_order.begin(), orginal_task_order.end(), user) != orginal_task_order.end()) {
                            has_valid_user = true;
                            LOG_DBG("["+task->name()+"] tensor has valid user: " + user);
                            break;
                        }
                    }
                    if (has_valid_user) {
                        all_outputs_have_no_valid_users = false;
                        break;
                    }
                }
                if (all_outputs_have_no_valid_users)
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
        if (!(task->is_tail()))
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

        if (task->is_head() == false)
        {
            auto &prevs = task->prevs();
            // for (size_t i = 0; i < task->inputs().size(); ++i) {
            //    cout <<task->name() << "->inputs (" <<to_std::string(i) <<") " <<task->inputs()[i].name() << endl;
            //}
            for (auto &tensor : inputs)
            {
                std::string tensor_name = tensor.name();
                std::string owner_task_name = tensor.owner();

                auto owner_task = _taskMap[owner_task_name];
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
    _finalOutputOrder.clear();

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
    
    // Step 3: Build _finalOutputOrder in the same order as _lastOutputOrder
    // This ensures proper tensor ordering for offset calculations
    for (const std::string& tensorName : _lastOutputOrder)
    {
        if (_tensorRegistry.find(tensorName) != _tensorRegistry.end() &&
            _tensorRegistry[tensorName].isModelOutput)
        {
            _finalOutputOrder.push_back(tensorName);
        }
    }

    LOG_DBG("Tensor registry built with " + std::to_string(_tensorRegistry.size()) + " tensors");
    LOG_DBG("Final output order: " + std::to_string(_finalOutputOrder.size()) + " tensors");
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

    // Calculate offsets based on _finalOutputOrder
    for (const std::string& tensorName : _finalOutputOrder)
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
    return !_tensorRegistry.empty() && !_finalOutputOrder.empty();
}

int InferenceEngine::DSP_GetDeviceBufferPtr(uint64_t *inputPtr, uint64_t *outputPtr)
{
    int ret = 0;

    ret = DSP_GetBufferPtrFromObjPools(inputPtr, outputPtr);

    return ret;
}

void *InferenceEngine::DSP_Run(void *inputPtr, void *outputPtr, void *userArg)
{
    dxrt_dspcvmat_t dspCvMatIn, dspCvMatOut;
    dspCvMatIn.cols = 640;
    dspCvMatIn.rows = 480;
    dspCvMatIn.data = reinterpret_cast<uint8_t*>(inputPtr);
    dspCvMatIn.step[0] = dspCvMatIn.cols;
    dspCvMatIn.step[1] = 1;
    dspCvMatIn.flags = DSPCV_8UC3;
    dspCvMatIn.dims = 2;

    dspCvMatOut.cols = 640;
    dspCvMatOut.rows = 640;
    dspCvMatOut.data = reinterpret_cast<uint8_t*>(outputPtr);
    dspCvMatOut.step[0] = dspCvMatOut.cols;
    dspCvMatOut.step[1] = 1;
    dspCvMatOut.flags = DSPCV_8UC3;
    dspCvMatOut.dims = 2;

    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->pick();

    infJob->DSP_SetDspEnable(1);
    infJob->SetInferenceJob(_tasks, _head, _lastOutputOrder);

    int jobId = infJob->DSP_StartJob(&dspCvMatIn, &dspCvMatOut, userArg);
    {
        _inferenceJobPool->GetById(jobId)->SetOccupiedJob(true);
    }
    return DSP_Wait(jobId);
}

void *InferenceEngine::DSP_Wait(int jobId)
{
    LOG_DXRT_DBG << jobId << std::endl;

    // std::shared_ptr<InferenceJob> infJob = ObjectsPool::GetInstance().GetInferenceJobById(jobId);
    std::shared_ptr<InferenceJob> infJob = _inferenceJobPool->GetById(jobId);
    infJob->Wait();
    // this_thread::sleep_for(chrono::microseconds(1));
    // while (infJob->getStatus() == Request::Status::REQ_BUSY)
    // {    this_thread::sleep_for(chrono::microseconds(1)); }

    LOG_DXRT_DBG << jobId << " done" << std::endl;
    return infJob->DSP_GetOutput();
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

}  // namespace dxrt
