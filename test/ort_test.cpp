#ifdef USE_ORT
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
#include <onnxruntime_cxx_api.h>
// #include <opencv2/opencv.hpp>
#include "gtest/gtest.h"
#include "dxrt/dxrt_api.h"
#include "dxrt_test.h"
// #include "dxrt/memory.h"

#define TEST_ONNX_FILE "test/data/cpu_t.onnx"
#define TEST_DATA_PATH "./"

using namespace std;

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

// class OrtTask
// {
// public:
//     OrtTask(void);
//     ~OrtTask(void);
//     OrtTask(std::string file);
//     static std::shared_ptr<OrtTask> Create(std::string file);
//     int Run();
//     void Show();
// private:
//     std::string onnxFile;
//     Ort::Env env;
//     Ort::SessionOptions sessionOptions;
//     Ort::Session *session;
//     size_t numInputs;
//     size_t numOutputs;
//     vector<std::string> inputNamesStr;
//     vector<std::string> outputNamesStr;
//     vector<const char*> inputNames;
//     vector<const char*> outputNames;
//     vector<Ort::Value> inputTensors;
//     vector<Ort::Value> outputTensors;
//     vector<void*> inputTensorBuffers;
//     vector<void*> outputTensorBuffers;
//     ONNXTensorElementDataType inputDataType;
//     ONNXTensorElementDataType outputDataType;
//     vector<int64_t> inputSize;
//     vector<int64_t> outputSize;
//     vector<vector<int64_t>> inputDims;
//     vector<vector<int64_t>> outputDims;
// };

// OrtTask::OrtTask(void) {}
// OrtTask::~OrtTask(void)
// {
//     // LOG_VALUE_HEX(session);
//     // dxrt::MemFree((void**)&session);
//     if(session!=nullptr)
//     {
//         delete session;
//     }
// }
// OrtTask::OrtTask(std::string file) : onnxFile(file)
// {    
//     env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_INFO, "dxrt-onnx");
//     session = new Ort::Session(env, onnxFile.c_str(), sessionOptions);
//     Ort::AllocatorWithDefaultOptions allocator;
//     // Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
//     numInputs = session->GetInputCount();
//     numOutputs = session->GetOutputCount();    
//     inputNamesStr = vector<string>(numInputs);
//     inputNames = vector<const char*>(numInputs);
//     inputTensorBuffers = vector<void*>(numInputs);
//     inputSize = vector<int64_t>(numInputs);
//     inputDims = vector<vector<int64_t>>(numInputs);
//     inputTensors.clear();
//     outputNamesStr = vector<string>(numOutputs);
//     outputNames = vector<const char*>(numOutputs);
//     outputTensorBuffers = vector<void*>(numOutputs);
//     outputSize = vector<int64_t>(numOutputs);
//     outputDims = vector<vector<int64_t>>(numOutputs);    
//     outputTensors.clear();
//     cout << "ORT Task from " << onnxFile << ": " << numInputs << " inputs, " << numOutputs << " outputs." << endl;
//     LOG_VALUE(inputNamesStr.size());
//     for(int i=0;i<numInputs;i++)
//     {
//         Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
//         inputNamesStr[i] = session->GetInputNameAllocated(i, allocator).get();
//         inputNames[i] = inputNamesStr[i].c_str();
//         Ort::TypeInfo typeInfo = session->GetInputTypeInfo(i);
//         auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
//         ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
//         if(i==0) inputDataType = dataType;
//         std::vector<int64_t> dims = tensorInfo.GetShape();
//         inputDims[i] = dims;
//         size_t size = dxrt::vectorProduct(dims);
//         inputSize[i] = size*sizeof(float);
//         void *buf = dxrt::MemAlloc(size*sizeof(float));
//         inputTensorBuffers[i] = buf;
//         inputTensors.emplace_back(
//             std::move(
//             Ort::Value::CreateTensor<float>(
//                 memoryInfo, (float*)inputTensorBuffers[i], size, dims.data(), dims.size()
//             ))
//         );
//         cout << "input [" << dec << i << "] " << inputNames[i] << ", " << dataType << ", " << dims << ", " << size << endl;
//     }
//     for(int i=0;i<numOutputs;i++)
//     {
//         Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
//         outputNamesStr[i] = session->GetOutputNameAllocated(i, allocator).get();
//         outputNames[i] = outputNamesStr[i].c_str();
//         Ort::TypeInfo typeInfo = session->GetOutputTypeInfo(i);
//         auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
//         ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
//         if(i==0) outputDataType = dataType;
//         std::vector<int64_t> dims = tensorInfo.GetShape();
//         outputDims[i] = dims;
//         size_t size = dxrt::vectorProduct(dims);
//         outputSize[i] = size*sizeof(float);
//         void *buf = dxrt::MemAlloc(size*sizeof(float));
//         outputTensorBuffers[i] = buf;
//         outputTensors.emplace_back(
//             std::move(
//             Ort::Value::CreateTensor<float>(
//                 memoryInfo, (float*)outputTensorBuffers[i], size, dims.data(), dims.size()
//             ))
//         );
//         cout << "output [" << dec << i << "] " << outputNames[i] << ", " << dataType << ", " << dims << ", " << size << endl;
//     }
//     /* Temp: Inject Test Data */
//     for(int i=0;i<numInputs;i++)
//     {
//         dxrt::DataFromFile(TEST_DATA_PATH "/output."+to_string(i)+".bin", (void*)inputTensorBuffers[i]);        
//     }

//     // cout << "created ort task" << endl;
// }
// std::shared_ptr<OrtTask> OrtTask::Create(std::string file)
// {
//     shared_ptr<OrtTask> ret = std::make_shared<OrtTask>(file);
//     return ret;
// }
// int OrtTask::Run()
// {
//     chrono::steady_clock::time_point tBegin = chrono::steady_clock::now();
//     session->Run(Ort::RunOptions{nullptr}, 
//                 inputNames.data(), inputTensors.data(), inputTensors.size(), 
//                 outputNames.data(), outputTensors.data(), outputTensors.size());
//     // auto outputs = session.Run(Ort::RunOptions{nullptr}, 
//     //             inputNames.data(), inputTensors.data(), inputTensors.size(), 
//     //             outputNames.data(), 1);
//     chrono::steady_clock::time_point tEnd = chrono::steady_clock::now();
//     auto t = chrono::duration_cast<chrono::microseconds>(tEnd - tBegin).count();
//     cout << "Inference Latency : " << t << "us" << endl;
//     /* Get Output */
//     dxrt::DataDumpBin(TEST_DATA_PATH "/orttask.output.0.bin", (void*)outputTensorBuffers[0], 16128*85*sizeof(float));
//     dxrt::DataDumpTxt(TEST_DATA_PATH "/orttask.output.0.txt", (float*)outputTensorBuffers[0], 1, 16128, 85);
//     return 0;
// }
// void OrtTask::Show()
// {
//     for(int i=0;i<numInputs;i++)
//     {        
//         cout << "input [" << dec << i << "] " << inputNames[i] << ", " << inputDataType << ", " << inputDims[i] << ", " << inputSize[i] << "bytes, " << endl;
//     }
//     for(int i=0;i<numOutputs;i++)
//     {
//         cout << "output [" << dec << i << "] " << outputNames[i] << ", " << outputDataType << ", " << outputDims[i] << ", " << outputSize[i] << "bytes, " << endl;
//     }
// }

// TEST(orttask, basic)
// {
//     auto ortTask = OrtTask::Create(TEST_ONNX_FILE);
//     ortTask->Show();
//     int ret = ortTask->Run();
// }
TEST(ort, basic)
{
    uint32_t tt, t;
    // string onnxFileName = TEST_ONNX_FILE;
    string onnxFileName = testModelPath;
    cout << "Test OnnxRuntime" << endl;
    /* Setup */
    Ort::Env env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING);
    Ort::SessionOptions sessionOptions;
    // sessionOptions.SetInterOpNumThreads(1);
    // /* Graph Optimization Level
    // ORT_DISABLE_ALL = 0,
    // ORT_ENABLE_BASIC = 1,
    // ORT_ENABLE_EXTENDED = 2,
    // ORT_ENABLE_ALL = 99 */
    sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_DISABLE_ALL);
    // sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    Ort::Session session(env, onnxFileName.c_str(), sessionOptions);
    Ort::AllocatorWithDefaultOptions allocator;
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
        OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
    );
    size_t numInputNodes = session.GetInputCount();
    size_t numOutputNodes = session.GetOutputCount();
    vector<string> inputNamesStr(numInputNodes);
    vector<string> outputNamesStr(numOutputNodes);
    vector<const char*> inputNames(numInputNodes);
    vector<const char*> outputNames(numOutputNodes);
    cout << numInputNodes << " inputs, " << numOutputNodes << " outputs "<< endl;
    /* Setup tensors */    
    vector<Ort::Value> inputTensors;
    vector<Ort::Value> outputTensors;
    vector<vector<float>> inputTensorBuffers;
    vector<vector<float>> outputTensorBuffers;
    for(int i=0;i<numInputNodes;i++)
    {
        inputNamesStr[i] = session.GetInputNameAllocated(i, allocator).get();
        inputNames[i] = inputNamesStr[i].c_str();
        Ort::TypeInfo typeInfo = session.GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
        size_t size = dxrt::vectorProduct(tensorInfo.GetShape());
        vector<float> buf(size);
        inputTensorBuffers.emplace_back(buf);
    }
    for(int i=0;i<numInputNodes;i++)
    {
        Ort::TypeInfo typeInfo = session.GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> dims = tensorInfo.GetShape();
        ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
        size_t tensorSize = dxrt::vectorProduct(dims);
        inputTensors.push_back(
            std::move(
            Ort::Value::CreateTensor<float>(
                memoryInfo, inputTensorBuffers[i].data(), tensorSize, dims.data(), dims.size()
            ))
        );
        // auto ptr = inputTensors[i].GetTensorData<float>();
        // cout << hex << ptr << dec << endl;
        cout << "input [" << dec << i << "] " << inputNames[i] << ", " << dataType << ", " << dims << ", " << tensorSize << endl;
    }
    for(int i=0;i<numOutputNodes;i++)
    {
        outputNamesStr[i] = session.GetOutputNameAllocated(i, allocator).get();
        outputNames[i] = outputNamesStr[i].c_str();
        Ort::TypeInfo typeInfo = session.GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
        size_t size = dxrt::vectorProduct(tensorInfo.GetShape());
        vector<float> buf(size);
        outputTensorBuffers.emplace_back(buf);
    }
    for(int i=0;i<numOutputNodes;i++)
    {
        // Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(
        //     OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault
        // );
        Ort::TypeInfo typeInfo = session.GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        std::vector<int64_t> dims = tensorInfo.GetShape();
        ONNXTensorElementDataType dataType = tensorInfo.GetElementType();
        size_t tensorSize = dxrt::vectorProduct(dims);
        outputTensors.push_back(
            std::move(
            Ort::Value::CreateTensor<float>(
                memoryInfo, outputTensorBuffers[i].data(), tensorSize, dims.data(), dims.size()
            ))
        );
        cout << "output [" << dec << i << "] " << outputNames[i] << ", " << dataType << ", " << dims << ", " << tensorSize << endl;
    }
    /* Temp: Inject Test Data */
    for(int i=0;i<numInputNodes;i++)
    {
        dxrt::DataFromFile(TEST_DATA_PATH "/output."+to_string(i)+".bin", (void*)inputTensorBuffers[i].data());        
    }
    // cout << inputTensorBuffers[numInputNodes-1] << endl;
    /* Run */
    int numRepeat = 1;
    for(int i=0;i<numRepeat;i++)
    {
        chrono::steady_clock::time_point tBegin = chrono::steady_clock::now();
        session.Run(Ort::RunOptions{nullptr}, 
                    inputNames.data(), inputTensors.data(), inputTensors.size(), 
                    outputNames.data(), outputTensors.data(), outputTensors.size());
        // auto outputs = session.Run(Ort::RunOptions{nullptr}, 
        //             inputNames.data(), inputTensors.data(), inputTensors.size(), 
        //             outputNames.data(), 1);
        chrono::steady_clock::time_point tEnd = chrono::steady_clock::now();
        auto t = chrono::duration_cast<chrono::microseconds>(tEnd - tBegin).count();
        cout << "Inference Latency : " << t << "us" << endl;
    }
    /* Get Output */
    dxrt::DataDumpBin(TEST_DATA_PATH "/ort.output.0.bin", (void*)outputTensorBuffers[0].data(), outputTensorBuffers[0].size()*sizeof(float));
    dxrt::DataDumpTxt(TEST_DATA_PATH "/ort.output.0.txt", (float*)outputTensorBuffers[0].data(), 1, 16128, 85);
    // cout << outputTensorBuffers[numOutputNodes-1] << endl;
}
#endif