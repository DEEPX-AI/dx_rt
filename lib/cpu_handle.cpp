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
#include <sys/time.h>
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

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

using namespace std;

namespace dxrt
{
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
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
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
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
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX64:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_COMPLEX128:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
        case ONNX_TENSOR_ELEMENT_DATA_TYPE_UNDEFINED:
        default: 
            return 0;
    }
}
CpuHandle::CpuHandle(void* data_, int64_t size_)
{
    _env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_VERBOSE, "dxrt--test");
    /* Graph Optimization Level
    ORT_DISABLE_ALL = 0,
    ORT_ENABLE_BASIC = 1,
    ORT_ENABLE_EXTENDED = 2,
    ORT_ENABLE_ALL = 99 */
    _sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);
    // _sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    DataDumpBin("tmp.onnx", data_, size_);
    _session = make_shared<Ort::Session>(_env, data_, size_, _sessionOptions);    
    Ort::AllocatorWithDefaultOptions allocator;
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
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
    for(int i=0; i<_numInputs; i++)
    {
        _inputNames.push_back( _session->GetInputNameAllocated(i, allocator).get() );
        _inputNamesChar.push_back( _inputNames.back().c_str());
        Ort::TypeInfo typeInfo = _session->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        if(i==0)
        {
            _inputDataType = convertDataType(dataType);
            _inputDataTypeOrg = (int)dataType;
        }
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
        _outputNames.push_back( _session->GetOutputNameAllocated(i, allocator).get() );
        _outputNamesChar.push_back( _outputNames.back().c_str());
        Ort::TypeInfo typeInfo = _session->GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        auto dataType = tensorInfo.GetElementType();
        if(i==0)
        {
            _outputDataType = convertDataType(dataType);
            _outputDataTypeOrg = (int)dataType;
        }
        _outputShape.push_back(tensorInfo.GetShape());
        auto size = dxrt::vectorProduct(_outputShape.back()) * convertElementSize(dataType);
        _outputSize += size;
        _outputSizes.push_back(size);
        if(i<_numOutputs-1)
        {
            _outputOffsets.push_back(_outputSize);
        }
    }
    _buffer = make_shared<Buffer>(4*(_inputSize + _outputSize));
}
int CpuHandle::InferenceRequest(RequestPtr req)
{
    auto future = async( launch::async, [ & ]() {
        Run(req);
    } );
    return 0;
}
void CpuHandle::Run(RequestPtr req)
{
    auto task = req->task();
    req->outputs() = task->outputs( _buffer->Get(task->output_size()) );
    // {
    //     LOG_VALUE(req->inputs().size());
    //     LOG_VALUE(req->outputs().size());
    //     for(auto &t:req->inputs()) cout << t << endl;
    //     for(auto &t:req->outputs()) cout << t << endl;
    //     // exit(0);
    // }
    vector<Ort::Value> inputTensors, outputTensors;
    Ort::MemoryInfo memoryInfo = 
        Ort::MemoryInfo::CreateCpu(
            OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
        );
    for(int i=0;i<_numInputs;i++)
    {
        inputTensors.emplace_back(
            move(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->inputs()[i].data(),
                    _inputSizes[i],
                    _inputShape[i].data(),
                    _inputShape[i].size(),
                    (ONNXTensorElementDataType)_inputDataTypeOrg
                )
            )
        );
    }
    for(int i=0;i<_numOutputs;i++)
    {
        outputTensors.emplace_back(
            move(
                Ort::Value::CreateTensor(
                    memoryInfo,
                    req->outputs()[i].data(),
                    _outputSizes[i],
                    _outputShape[i].data(),
                    _outputShape[i].size(),
                    (ONNXTensorElementDataType)_outputDataTypeOrg
                )
            )
        );
    }
    // cout << "session run start" << endl;
    // auto &profiler = Profiler::GetInstance();
    // profiler.Start("ort");
    // for(auto &x:_inputNamesChar) cout << string(x) << endl;
    // for(auto &x:_outputNamesChar) cout << string(x) << endl;
    _session->Run( Ort::RunOptions{nullptr}, 
        _inputNamesChar.data(), inputTensors.data(), inputTensors.size(),
        _outputNamesChar.data(), outputTensors.data(), outputTensors.size()
    );
    // profiler.End("ort");
    // DataDumpBin("ort.output.bin", (float*)req->outputs().front().data(), _outputSize);
    // DataDumpTxt("ort.output.txt", (float*)req->outputs().front().data(), 1, 25200, 85);
    // LOG_VALUE(profiler.Get("ort"));
    // cout << "session run end" << endl;
    req->task()->ProcessResponse(req, nullptr);
    cout << req->latency() << endl;
}
#else
CpuHandle::CpuHandle(void* data_, int64_t size_) {}
int CpuHandle::InferenceRequest(RequestPtr req) {}
#endif

ostream& operator<<(ostream& os, const CpuHandle& c)
{
    for(int i=0;i<c._numInputs;i++)
    {
        os << "            input [" << dec << i << "] " << c._inputNames[i] << ", " << c._inputDataType << ", " << c._inputShape[i] << endl;
    }
    for(int i=0;i<c._numOutputs;i++)
    {
        os << "            output [" << dec << i << "] " << c._outputNames[i] << ", " << c._outputDataType << ", " << c._outputShape[i] << endl;
    }
    return os;
}
}/* namespace dxrt */