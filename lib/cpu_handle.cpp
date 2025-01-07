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

#ifdef USE_ORT
#ifdef __linux__
#include <onnxruntime_cxx_api.h>
#endif
#endif

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
CpuHandle::CpuHandle(void* data_, int64_t size_, string name_)
: _name(name_)
{
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
        _inputDataTypesOrg.push_back((int)dataType);          
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
        _outputDataTypesOrg.push_back((int)dataType);          
        _outputShape.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_outputShape.back()) * convertElementSize(dataType);
        _outputSize += size;
        _outputSizes.push_back(size);

        if(i<_numOutputs-1)
        {
            _outputOffsets.push_back(_outputSize);
        }
    }
 
}
CpuHandle::~CpuHandle()
{
    LOG_DXRT_DBG << endl;
    if (_worker != nullptr)
        _worker->Stop();
}
int CpuHandle::InferenceRequest(RequestPtr req)
{
    return _worker->request(req);
}
void CpuHandle::Run(RequestPtr req)
{
    LOG_DXRT_DBG << "CpuHandleRun:" << req->id() << std::endl;
    auto task = req->task();
    TASK_FLOW_START("["+to_string(req->job_id())+"]"+task->name());
    
    if(req->inputs().empty())
    {
        //req->inputs() = task->inputs( _buffer->Get(task->input_size()) );
        req->inputs() = task->inputs(task->GetInputBuffer());
    }
    //req->outputs() = task->outputs( _buffer->Get(task->output_size()) );
    req->outputs() = task->outputs(req->getData()->output_ptr);

    vector<Ort::Value> inputTensors, outputTensors;
    Ort::MemoryInfo memoryInfo = 
        Ort::MemoryInfo::CreateCpu(
            OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
        );

    LOG_DXRT_DBG << task->id() << " - _numInputs : " << std::to_string(_numInputs) << std::endl;
    if (!task->is_head() && task->prevs().size() == 1) 
    {
        int prevTaskId = task->prevs().front()->id();
        LOG_DXRT_DBG << std::to_string(prevTaskId) << std::endl;
        for (int i = 0; i < _numInputs; i++) 
        {
            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    (ONNXTensorElementDataType)_inputDataTypesOrg[i]
                )
            );
        }
    } 
    else if (!task->is_head() && task->prevs().size() > 1) 
    {
        LOG_DXRT_DBG << "prev task size : " << task->prevs().size() << std::endl;
        for (int i = 0; i < _numInputs; i++) 
        {
            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    (ONNXTensorElementDataType)_inputDataTypesOrg[i]
                )
            );
        }
    } 
    else 
    {
        LOG_DXRT_DBG << " task is head, _numInputs :" << _numInputs << std::endl;
        for (int i = 0; i < _numInputs; i++) 
        {
            LOG_DXRT_DBG << "CpuHandle : " << req->inputs()[i].name() << " / i : " << std::to_string(i) << std::endl;
            LOG_DXRT_DBG << "_inputShape" << std::endl;
            int acc = 4;
            for (int64_t &element : _inputShape[i]) {
                LOG_DXRT_DBG << element << " ";
                acc *= element;
            }
            LOG_DXRT_DBG << "_inputSizes : " << std::to_string(_inputSizes[i]) << std::endl;
            LOG_DXRT_DBG << "_inputDataTypesOrg : " << std::to_string(_inputDataTypesOrg[i]) << std::endl;
        }
        for (int i = 0; i < _numInputs; i++) {
            inputTensors.emplace_back(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    (ONNXTensorElementDataType)_inputDataTypesOrg[i]
                )
            );
        }
    }

    for (int i = 0; i < _numOutputs; i++) {
        LOG_DXRT_DBG << "CpuHandle : " << req->outputs()[i].name() << " output Tensor processing dtype : " << std::to_string(_outputDataTypesOrg[i]) << " / i : " << std::to_string(i) << std::endl;
        outputTensors.emplace_back(
            Ort::Value::CreateTensor(
                memoryInfo,
                req->outputs()[i].data(),
                _outputSizes[i],
                _outputShape[i].data(),
                _outputShape[i].size(),
                (ONNXTensorElementDataType)_outputDataTypesOrg[i]
            )
        );
    }
    LOG_DXRT_DBG << "session run start : " << req->id() << std::endl;
    _session->Run(Ort::RunOptions{nullptr},
                  _inputNamesChar.data(), inputTensors.data(), inputTensors.size(),
                  _outputNamesChar.data(), outputTensors.data(), outputTensors.size());
    LOG_DXRT_DBG << "session run end : " << req->id() << std::endl;

    task->ProcessResponse(req, nullptr);
}
void CpuHandle::Terminate()
{
    _worker->Stop();
}
void CpuHandle::Start()
{
    //_buffer = make_shared<Buffer>((_inputSize + _outputSize)*DXRT_ASYNC_LOAD_THRE*2);
    _worker = CpuHandleWorker::Create(_name, 1, this);
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
