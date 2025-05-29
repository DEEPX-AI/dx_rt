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
#include <cmath>
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
#include "dxrt/cpu_handle.h"
#include "dxrt/buffer.h"
#include "dxrt/task.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/exception/exception.h"
#include "dxrt/configuration.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif


#define MINIMUM_ORT_VERSION "1.20.0"
using namespace std;

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


CpuHandle::CpuHandle(void* data_, int64_t size_, string name_)
: _name(name_)
{
    if (version_check() == false)
    {
        throw InvalidOperationException("NOT SUPPORTED ORT VERSION "+ Ort::GetVersionString());
    }
    // _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_VERBOSE, "dxrt cpu handle");
    // _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO, "dxrt cpu handle");
    _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO);
    /* Graph Optimization Level
    ORT_DISABLE_ALL = 0,
    ORT_ENABLE_BASIC = 1,
    ORT_ENABLE_EXTENDED = 2,
    ORT_ENABLE_ALL = 99 */
    _sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);
    // _sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    // _sessionOptions.SetExecutionMode(ORT_SEQUENTIAL);
    // _sessionOptions.SetExecutionMode(ORT_PARALLEL);
    // DataDumpBin("tmp.onnx", data_, size_);
    _session = make_shared<Ort::Session>(_env, data_, size_, _sessionOptions);
    Ort::AllocatorWithDefaultOptions allocator;
    // Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
    _numInputs = _session->GetInputCount();
    _numOutputs = _session->GetOutputCount();
    _inputNames.clear();
    _outputNames.clear();
    _inputNamesChar.clear();
    _outputNamesChar.clear();
    _inputShape.clear();
    _outputShape.clear();
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
    for(int i=0; i<_numInputs; i++)
    {
        _inputNames.push_back( move(string(_session->GetInputNameAllocated(i, allocator).get())) );
        _inputNamesChar.push_back( _inputNames[i].c_str());
    }
    _outputNames.reserve(_numOutputs);
    _outputNamesChar.reserve(_numOutputs);
    for(int i=0; i<_numOutputs; i++)
    {
        _outputNames.push_back( move(string(_session->GetOutputNameAllocated(i, allocator).get())) );
        _outputNamesChar.push_back( _outputNames[i].c_str());
    }
    for(int i=0; i<_numInputs; i++)
    {
        Ort::TypeInfo typeInfo = _session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        _inputDataTypes.push_back(convertDataType(dataType)); 
        _inputShape.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_inputShape.back()) * convertElementSize(dataType);
        _inputSize += size;
        _inputSizes.push_back(size);
        if(i<_numInputs-1)
        {
            _inputOffsets.push_back(_inputSize);
        }
    }    
    for(int i=0; i<_numOutputs; i++)
    {
        Ort::TypeInfo typeInfo = _session->GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        _outputDataTypes.push_back(convertDataType(dataType)); 
        _outputShape.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_outputShape.back()) * convertElementSize(dataType);
        _outputSize += size;
        _outputSizes.push_back(size);

        if(i<_numOutputs-1)
        {
            _outputOffsets.push_back(_outputSize);
        }
    }

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
    LOG_DXRT_DBG<<"Task "<<name_<<" is set to "<<to_string(_numThreads + _initDynamicThreads)<<" threads (total : "<<to_string(_totalNumThreads.load())<<")"<<endl;
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

    if(dynamic_cpu_thread_env||_dynamicCpuThread)
        _dynamicCpuThread=true;
    
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
#ifdef USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
    string processedPU = req->processed_pu();
    int processedId = req->processed_id();
    string profileInstanceName = processedPU + "_t" + to_string(processedId);
    profiler.Start(profileInstanceName);
#endif
    LOG_DXRT_DBG << "CpuHandleRun:" << req->id() << std::endl;
    auto task = req->task();
    
    if(!task->is_head())
    {
        //req->inputs() = task->inputs( _buffer->Get(task->input_size()) );
        task->inputs(req->inputs().front().data());
    }
    //req->outputs() = task->outputs( _buffer->Get(task->GetOutputSize()) );
    req->setOutputs(task->outputs(req->getData()->output_ptr));

    vector<Ort::Value> inputTensors, outputTensors;
    Ort::MemoryInfo memoryInfo = 
        Ort::MemoryInfo::CreateCpu(
            OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
        );

    LOG_DXRT_DBG << task->id() << " - _numInputs : " << to_string(_numInputs) << std::endl;
    if (!task->is_head()) 
    {
        int prevTaskId = task->prevs().front()->id();
        LOG_DXRT_DBG << std::to_string(prevTaskId) << std::endl;
        for (int i = 0; i < _numInputs; i++) 
        {
            LOG_DXRT_DBG << "CpuHandle : " << req->inputs()[i].name() << " / i : " << to_string(i) << std::endl;
            LOG_DXRT_DBG << "_inputShape" << std::endl;
            for (int64_t &element : _inputShape[i]) {
                LOG_DXRT_DBG << element << " ";
            }
            LOG_DXRT_DBG << "_inputSizes : " << to_string(_inputSizes[i]) << std::endl;
        }
        for (int i = 0; i < _numInputs; i++) 
        {
            LOG_DXRT_DBG<<req->inputs()[i]<<std::endl;
            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    convertONNXTensorElementDataType(_inputDataTypes[i])
                )
            );
        }
    }
    else 
    {
        LOG_DXRT_DBG << " task is head, _numInputs :" << _numInputs << std::endl;
        for (int i = 0; i < _numInputs; i++) 
        {
            LOG_DXRT_DBG << "CpuHandle : " << req->inputs()[i].name() << " / i : " << to_string(i) << std::endl;
            LOG_DXRT_DBG << "_inputShape" << std::endl;
            for (int64_t &element : _inputShape[i]) {
                LOG_DXRT_DBG << element << " ";
            }
            LOG_DXRT_DBG << "_inputSizes : " << to_string(_inputSizes[i]) << std::endl;
        }
        for (int i = 0; i < _numInputs; i++) {
            LOG_DXRT_DBG<<req->inputs()[i]<<std::endl;
            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    convertONNXTensorElementDataType(_inputDataTypes[i])
                )
            );
        }
    }

    for (int i = 0; i < _numOutputs; i++) {
        LOG_DXRT_DBG << "CpuHandle : " << req->outputs()[i].name() << " output Tensor processing dtype : " << _outputDataTypes[i] << " / i : " << std::to_string(i) << std::endl;
        LOG_DXRT_DBG << "_outputShape" << std::endl;
        for (int64_t &element : _outputShape[i]) {
            LOG_DXRT_DBG << element << " ";
        }
        LOG_DXRT_DBG << "_outputSizes : " << to_string(_outputSizes[i]) << std::endl;
        outputTensors.emplace_back(
            Ort::Value::CreateTensor(
                memoryInfo,
                req->outputs()[i].data(),
                _outputSizes[i],
                _outputShape[i].data(),
                _outputShape[i].size(),
                convertONNXTensorElementDataType(_outputDataTypes[i])
            )
        );
    }
    LOG_DXRT_DBG << "session run start : " << req->id() << std::endl;
    _session->Run(Ort::RunOptions{nullptr},
                  _inputNamesChar.data(), inputTensors.data(), inputTensors.size(),
                  _outputNamesChar.data(), outputTensors.data(), outputTensors.size());
    LOG_DXRT_DBG << "session run end : " << req->id() << std::endl;
#ifdef USE_PROFILER
    profiler.End(profileInstanceName);
#endif
    //ProcessResponse(req, nullptr);
}
void CpuHandle::Terminate()
{
    _worker->Stop();
}
void CpuHandle::Start()
{
    LOG_DXRT_DBG<<"CpuHandleWorer Start : "<<_numThreads<<endl;
    _worker = CpuHandleWorker::Create(_name, _numThreads, _initDynamicThreads, this);
}


#else
CpuHandle::CpuHandle(void* data_, int64_t size_, string name_) : _name(name_){
	std::ignore = size_;
	std::ignore = data_;
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

ostream& operator<<(ostream& os, const CpuHandle& c)
{
    for(int i=0;i<c._numInputs;i++)
    {
        os << "            input [" << dec << i << "] " << c._inputNames[i] << ", " << c._inputDataTypes[i] << ", " << c._inputShape[i] << endl;
    }
    for(int i=0;i<c._numOutputs;i++)
    {
        os << "            output [" << dec << i << "] " << c._outputNames[i] << ", " << c._outputDataTypes[i] << ", " << c._outputShape[i] << endl;
    }
    return os;
}
}/* namespace dxrt */
