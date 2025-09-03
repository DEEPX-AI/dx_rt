/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 * 
 * This file uses ONNX Runtime (MIT License) - Copyright (c) Microsoft Corporation.
 */

#include "dxrt/cpu_handle.h"
#include <chrono>
#include <cmath>
#include <exception>
#include <limits>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#ifdef __linux__
    #include <sys/time.h>
#elif _WIN32
    #include <windows.h>
#endif
#include <time.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "dxrt/buffer.h"
#include "dxrt/task.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/exception/exception.h"
#include "dxrt/configuration.h"
#include "./resource/log_messages.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

using std::endl;

#define MINIMUM_ORT_VERSION "1.20.0"

namespace dxrt
{
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i + 1 != v.size())
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

std::atomic<int> CpuHandle::_totalNumThreads{0};
bool CpuHandle::_dynamicCpuThread = false;

#ifdef USE_ORT
DataType convertDataType(ONNXTensorElementDataType dataType)
{
    switch (dataType) {
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT: return DataType::FLOAT;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8: return DataType::UINT8;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8: return DataType::INT8;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16: return DataType::UINT16;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16: return DataType::INT16;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32: return DataType::UINT32;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32: return DataType::INT32;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64: return DataType::INT64;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64: return DataType::UINT64;
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED:
        default:
            return DataType::NONE_TYPE;
    }
}
ONNXTensorElementDataType convertONNXTensorElementDataType(DataType dataType) {
    switch (dataType) {
        case DataType::FLOAT: return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
        case DataType::UINT8: return ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8;
        case DataType::INT8: return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8;
        case DataType::UINT16: return ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16;
        case DataType::INT16: return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16;
        case DataType::UINT32: return ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32;
        case DataType::INT32: return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32;
        case DataType::INT64: return ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64;
        case DataType::UINT64: return ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64;
        default:
            return ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED;
    }
}

size_t convertElementSize(ONNXTensorElementDataType dataType)
{
    switch (dataType) {
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT: return sizeof(float);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8: return sizeof(uint8_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8: return sizeof(int8_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16: return sizeof(uint16_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16: return sizeof(int16_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT32: return sizeof(uint32_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32: return sizeof(int32_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64: return sizeof(int64_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64: return sizeof(uint64_t);
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED:
        default:
            return 0;
    }
}

std::pair<int, int> verson_parse(const string& str)
{
    std::stringstream vs(str);
    char dot = '.';
    int major = 0, minor = 0;
    vs >> major >> dot >> minor;
    return std::make_pair(major, minor);
}

bool version_check()
{
    std::pair<int, int> ver = verson_parse(Ort::GetVersionString());
    std::pair<int, int> min_ver =  verson_parse(MINIMUM_ORT_VERSION);
    return ver >= min_ver;
}


CpuHandle::CpuHandle(void* data_, int64_t size_, string name_, size_t device_num_)
: _name(name_), _device_num(device_num_)
{
    if (version_check() == false)
    {
        throw InvalidOperationException("NOT SUPPORTED ORT VERSION "+ Ort::GetVersionString());
    }
    
    // Store model data for worker session creation (DYNAMIC THREAD mode)
    _modelSize = size_;
    _modelData.resize(size_);
    std::memcpy(_modelData.data(), data_, size_);
    
    // _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_VERBOSE, "dxrt cpu handle");
    // _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO, "dxrt cpu handle");
    _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO);
    /* Graph Optimization Level
    ORT_DISABLE_ALL = 0,
    ORT_ENABLE_BASIC = 1,
    ORT_ENABLE_EXTENDED = 2,
    ORT_ENABLE_ALL = 99 */
    _sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
 
    // Configure ONNX Runtime thread settings from configuration
    auto& config = Configuration::GetInstance();

    // Get intra-op threads setting (default: 0, auto)
    if (config.GetEnable(Configuration::ITEM::CUSTOM_INTRA_OP_THREADS)) {
        int intraOpThreads = config.GetIntAttribute(Configuration::ITEM::CUSTOM_INTRA_OP_THREADS, Configuration::ATTRIBUTE::CUSTOM_INTRA_OP_THREADS_NUM);
        if (intraOpThreads == 0) intraOpThreads = 1; // fallback to default if attribute returns 0
        _sessionOptions.SetIntraOpNumThreads(intraOpThreads);
        LOG_DXRT_DBG << "ONNX Runtime Session configured: IntraOpThreads=" << intraOpThreads << std::endl;
    }
    
    // Get inter-op threads setting (default: 1)  
    if (config.GetEnable(Configuration::ITEM::CUSTOM_INTER_OP_THREADS)) {
        int interOpThreads = config.GetIntAttribute(Configuration::ITEM::CUSTOM_INTER_OP_THREADS, Configuration::ATTRIBUTE::CUSTOM_INTER_OP_THREADS_NUM);
        if (interOpThreads == 0) interOpThreads = 1; // fallback to default if attribute returns 0
        if (interOpThreads > 1) {
            _sessionOptions.SetExecutionMode(ORT_PARALLEL);
        }
        else {
            _sessionOptions.SetExecutionMode(ORT_SEQUENTIAL);
        }
        _sessionOptions.SetInterOpNumThreads(interOpThreads);
        LOG_DXRT_DBG << "ONNX Runtime Session configured: InterOpThreads=" << interOpThreads << std::endl;
    }
    
    // DataDumpBin("tmp.onnx", data_, size_);
    _session = std::make_shared<Ort::Session>(_env, data_, size_, _sessionOptions);
    Ort::AllocatorWithDefaultOptions allocator;
    // Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    _numInputs = _session->GetInputCount();
    _numOutputs = _session->GetOutputCount();
    _inputNames.clear();
    _outputNames.clear();
    _inputNamesChar.clear();
    _outputNamesChar.clear();
    _inputShapes.clear();
    _outputShapes.clear();
    _inputOffsets.clear();
    _outputOffsets.clear();
    _inputSizes.clear();
    _outputSizes.clear();
    _inputSize = 0;
    _outputSize = 0;
    _inputOffsets.push_back(0);
    _outputOffsets.push_back(0);

    _inputNames.reserve(_numInputs);
    _inputNamesChar.reserve(_numInputs);
    for (int i = 0; i < _numInputs; i++)
    {
        _inputNames.push_back(move(string(_session->GetInputNameAllocated(i, allocator).get())) );
        _inputNamesChar.push_back(_inputNames[i].c_str());
    }
    _outputNames.reserve(_numOutputs);
    _outputNamesChar.reserve(_numOutputs);
    for (int i = 0; i < _numOutputs; i++)
    {
        _outputNames.push_back(move(string(_session->GetOutputNameAllocated(i, allocator).get())) );
        _outputNamesChar.push_back(_outputNames[i].c_str());
    }
    for (int i = 0; i < _numInputs; i++)
    {
        Ort::TypeInfo typeInfo = _session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        _inputDataTypes.push_back(convertDataType(dataType));
        _inputShapes.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_inputShapes.back()) * convertElementSize(dataType);
        _inputSize += size;
        _inputSizes.push_back(size);
        if (i < _numInputs-1)
        {
            _inputOffsets.push_back(_inputSize);
        }
    }
    for (int i = 0; i < _numOutputs; i++)
    {
        Ort::TypeInfo typeInfo = _session->GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        _outputDataTypes.push_back(convertDataType(dataType));
        _outputShapes.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_outputShapes.back()) * convertElementSize(dataType);
        _outputSize += size;
        _outputSizes.push_back(size);

        if (i < _numOutputs-1)
        {
            _outputOffsets.push_back(_outputSize);
        }
    }
    // To be replaced by a modeling method in the future
    if (_dynamicCpuThread) {
        if (size_ <= 64 * 1024) {
            _initDynamicThreads = 0;
        } else if (size_ <= 1 * 1024 * 1024) {
            _initDynamicThreads = 1;
        } else {
            _initDynamicThreads = 3;
        }
    }
    _totalNumThreads.fetch_add(_numThreads + _initDynamicThreads);
    LOG_DXRT_DBG << "Task " << name_ << " is set to " << std::to_string(_numThreads + _initDynamicThreads)
      << " threads (total : " << std::to_string(_totalNumThreads.load()) << ")" << endl;
}

CpuHandle::~CpuHandle()
{
    LOG_DXRT_DBG << endl;
    if (_worker != nullptr)
    {
        _worker->Stop();
        _worker = nullptr;
    }

    LOG_DXRT_DBG <<" Done"<< endl;
}

void CpuHandle::SetDynamicCpuThread() {
    const char* env = getenv("DXRT_DYNAMIC_CPU_THREAD");
    bool dynamic_cpu_thread_env = false;
    if (env != nullptr && string(env) == "ON") {
        dynamic_cpu_thread_env = true;
    } else {
        dynamic_cpu_thread_env = false;
    }

    _dynamicCpuThread = Configuration::GetInstance().GetEnable(Configuration::ITEM::DYNAMIC_CPU_THREAD);
    Configuration::GetInstance().LockEnable(Configuration::ITEM::DYNAMIC_CPU_THREAD);

    if (dynamic_cpu_thread_env || _dynamicCpuThread)
        _dynamicCpuThread = true;

    if (_dynamicCpuThread) {
        LOG_DXRT_DBG << "Dynamic Multi Threading : MULTI MODE" << endl;
    } else {
        LOG_DXRT_DBG << "Dynamic Multi Threading : SINGLE MODE" << endl;
    }
}

int CpuHandle::InferenceRequest(RequestPtr req)
{
    return _worker->request(req);
}

void CpuHandle::Run(RequestPtr req)
{
    RunWithSession(req, _session);
}

void CpuHandle::RunWithSession(RequestPtr req, std::shared_ptr<Ort::Session> session)
{
#ifdef USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
    string processedPU = req->processed_pu();
    int processedId = req->processed_id();
    string profileInstanceName = processedPU + "[Job_" + std::to_string(req->job_id()) + "][" + req->task()->name() + "][Req_" + std::to_string(req->id()) + "]_t" + std::to_string(processedId);
    profiler.Start(profileInstanceName);
#endif

    LOG_DXRT_DBG << "CpuHandleRun:" << req->id() << std::endl;
    auto task = req->task();

    // Set output tensors with proper buffer
    // If outputs are already prepared (e.g., mapped to user output buffer with global offsets), keep them.
    // Otherwise, build tensors from task-local offsets based on output_buffer_base.
    if (req->outputs().empty())
    {
        // req->getData()->output_buffer_base is the base address of continuous memory for this request
        // task->outputs() applies task-local offsets to set data pointer for each tensor
        req->setOutputs(task->outputs(req->getData()->output_buffer_base));
    }

    std::vector<Ort::Value> inputTensors, outputTensors;
    Ort::MemoryInfo memoryInfo =
        Ort::MemoryInfo::CreateCpu(
            OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);

    LOG_DXRT_DBG << task->id() << " - _numInputs : " << std::to_string(_numInputs) << std::endl;

    // Create input tensors for ONNX Runtime
    // Use the input tensors directly from the request - they already have correct pointers
    auto reqInputs = req->inputs();
    if (!reqInputs.empty() && reqInputs.size() >= static_cast<size_t>(_numInputs))
    {
        for (int i = 0; i < _numInputs; i++)
        {
            auto& inputTensor = reqInputs[i];

            LOG_DXRT_DBG << "CpuHandle Input[" << i << "]: " << _inputNames[i]
                        << ", data_ptr: " << inputTensor.data()
                        << ", size: " << _inputSizes[i] << std::endl;
            LOG_DXRT_DBG << "_inputShape[" << i << "]: " << _inputShapes[i] << std::endl;

            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    inputTensor.data(),          // Use tensor's data pointer directly
                    _inputSizes[i],              // Use ONNX Runtime's tensor size
                    _inputShapes[i].data(),
                    _inputShapes[i].size(),
                    convertONNXTensorElementDataType(_inputDataTypes[i])
                )
            );
        }
    }
    else
    {
        std::string err_msg = LogMessages::CPUHandle_NoInputTensorsAvailable(task->name(), reqInputs.size(), _numInputs);
        throw InvalidOperationException(EXCEPTION_MESSAGE(err_msg));
    }

    // Create output tensors for ONNX Runtime
    // Use the output tensors directly from the request - they already have correct pointers
    auto reqOutputs = req->outputs();
    if (!reqOutputs.empty() && reqOutputs.size() >= static_cast<size_t>(_numOutputs))
    {
        // Create ONNX output name -> index mapping for proper tensor ordering
        std::map<std::string, int> onnxOutputIndexMap;
        for (int i = 0; i < _numOutputs; ++i)
        {
            onnxOutputIndexMap[_outputNames[i]] = i;
        }

        // Pre-allocate output tensors vector with nullptr values
        outputTensors.clear();
        outputTensors.reserve(_numOutputs);
        for (int i = 0; i < _numOutputs; ++i)
        {
            outputTensors.emplace_back(nullptr);
        }

        // Map request outputs to ONNX outputs in correct order
        for (int i = 0; i < _numOutputs; i++)
        {
            auto& outputTensor = reqOutputs[i];
            std::string tensorName = outputTensor.name();

            // Find the corresponding ONNX output index
            auto it = onnxOutputIndexMap.find(tensorName);
            if (it == onnxOutputIndexMap.end())
            {
                //LOG_DXRT_ERR("Tensor '" + tensorName + "' not found in ONNX outputs for task: " + task->name());
                //throw std::runtime_error("Tensor '" + tensorName + "' not found in ONNX outputs");

                std::string err_msg = LogMessages::CPUHandle_NotFoundInONNXOutputs(tensorName, task->name());
                throw InvalidOperationException(EXCEPTION_MESSAGE(err_msg));
            }

            int onnxIndex = it->second;

            LOG_DXRT_DBG << "CpuHandle Output[" << i << "]: " << tensorName
                        << " -> ONNX[" << onnxIndex << "]: " << _outputNames[onnxIndex]
                        << ", dtype: " << _outputDataTypes[onnxIndex]
                        << ", data_ptr: " << outputTensor.data()
                        << ", size: " << _outputSizes[onnxIndex] << std::endl;
            LOG_DXRT_DBG << "_outputShape[" << onnxIndex << "]: " << _outputShapes[onnxIndex] << std::endl;

            // Create ONNX tensor at the correct index
            outputTensors[onnxIndex] = Ort::Value::CreateTensor(
                memoryInfo,
                outputTensor.data(),         // Use tensor's data pointer directly
                _outputSizes[onnxIndex],     // Use ONNX Runtime's tensor size
                _outputShapes[onnxIndex].data(),
                _outputShapes[onnxIndex].size(),
                convertONNXTensorElementDataType(_outputDataTypes[onnxIndex])
            );
        }
    }
    else
    {
        std::string err_msg = LogMessages::CPUHandle_NoOutputTensorsAvailable(task->name(), reqOutputs.size(), _numOutputs);
        throw InvalidOperationException(EXCEPTION_MESSAGE(err_msg));
    }

    // Validate tensor counts match ONNX model expectations
    if (static_cast<int>(inputTensors.size()) != _numInputs)
    {
        std::string err_msg = LogMessages::CPUHandle_InputTensorCountMismatch(inputTensors.size(), _numInputs);
        throw InvalidOperationException(EXCEPTION_MESSAGE(err_msg));
    }

    if (static_cast<int>(outputTensors.size()) != _numOutputs)
    {
        std::string err_msg = LogMessages::CPUHandle_OutputTensorCountMismatch(outputTensors.size(), _numOutputs);
        throw InvalidOperationException(EXCEPTION_MESSAGE(err_msg));
    }

    LOG_DXRT_DBG << "session run start : " << req->id() << std::endl;
    session->Run(Ort::RunOptions{nullptr},
                  _inputNamesChar.data(), inputTensors.data(), inputTensors.size(),
                  _outputNamesChar.data(), outputTensors.data(), outputTensors.size());
    LOG_DXRT_DBG << "session run end : " << req->id() << std::endl;
#ifdef USE_PROFILER
    profiler.End(profileInstanceName);
#endif
}
void CpuHandle::Terminate()
{
    _worker->Stop();
}
void CpuHandle::Start()
{
    LOG_DXRT_DBG << "CpuHandleWorer Start : " << _numThreads << endl;
    _worker = CpuHandleWorker::Create(_name, _numThreads, _initDynamicThreads, this, _device_num);
}

#ifdef USE_ORT
std::shared_ptr<Ort::Session> CpuHandle::CreateWorkerSession()
{
    // Currently not in use but may be needed in the future
    
    // Create session options specifically for worker threads
    Ort::SessionOptions workerSessionOptions;
    
    // Use the same graph optimization level as main session
    workerSessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
    /*
    // Set execution mode for parallel execution
    workerSessionOptions.SetExecutionMode(ORT_PARALLEL);
    
    // Dynamic thread allocation based on system resources
    int systemCores = std::thread::hardware_concurrency();
    int totalActiveCpuTasks = _totalNumThreads.load();
    
    // Calculate optimal intra_op threads per session
    int intraOpThreads = 1; // Conservative default
    if (totalActiveCpuTasks > 0) {
        intraOpThreads = std::max(1, systemCores / totalActiveCpuTasks);
        intraOpThreads = std::min(intraOpThreads, 4); // Cap at 4 to avoid over-subscription
    }
    
    workerSessionOptions.SetIntraOpNumThreads(intraOpThreads);
    workerSessionOptions.SetInterOpNumThreads(1); // Keep simple for predictability
    
    LOG_DXRT_DBG << "Creating worker session: intra_op=" << intraOpThreads 
                 << ", total_cpu_tasks=" << totalActiveCpuTasks 
                 << ", system_cores=" << systemCores << std::endl;
    */
    return std::make_shared<Ort::Session>(_env, _modelData.data(), _modelSize, workerSessionOptions);
}
#endif

#else
CpuHandle::CpuHandle(void* data_, int64_t size_, string name_, size_t device_num_) : _name(name_), _device_num(device_num_){
    std::ignore = size_;
    std::ignore = data_;
    std::ignore = device_num_;
}
CpuHandle::~CpuHandle() {}
int CpuHandle::InferenceRequest(RequestPtr req) {
    std::ignore = req;
    return -1;
}
void CpuHandle::Run(RequestPtr req) {std::ignore = req;}
void CpuHandle::Terminate() {}
void CpuHandle::Start() {}

#endif

std::ostream& operator<<(std::ostream& os, const CpuHandle& c)
{
    using std::dec;
    for (int i = 0; i < c._numInputs; i++)
    {
        os << "            input [" << dec << i << "] " << c._inputNames[i] << ", "
          << c._inputDataTypes[i] << ", " << c._inputShapes[i] << endl;
    }
    for (int i = 0; i < c._numOutputs; i++)
    {
        os << "            output [" << dec << i << "] " << c._outputNames[i] << ", "
          << c._outputDataTypes[i] << ", " << c._outputShapes[i] << endl;
    }
    return os;
}
}/* namespace dxrt */
