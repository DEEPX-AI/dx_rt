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

#pragma once

#include <vector>
#include <atomic>

#include "dxrt/common.h"
#include "dxrt/datatype.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

namespace dxrt {
class Buffer;
class CpuHandleWorker;
class Request;
using RequestPtr = std::shared_ptr<Request>;

class DXRT_API CpuHandle
{
public:
    CpuHandle(void* data_, int64_t size_, std::string name_, size_t device_num_);
    ~CpuHandle();
#ifdef USE_ORT
    Ort::Env _env;
    Ort::SessionOptions _sessionOptions;
    std::shared_ptr<Ort::Session> _session;
    
    // For DYNAMIC THREAD: store model data for worker session creation
    std::vector<uint8_t> _modelData;
    int64_t _modelSize;
    
    // Create individual session for worker (DYNAMIC THREAD mode)
    std::shared_ptr<Ort::Session> CreateWorkerSession();
    void RunWithSession(RequestPtr req, std::shared_ptr<Ort::Session> session);
#endif

public:
    static std::atomic<int> _totalNumThreads;
    static bool _dynamicCpuThread;
    static void SetDynamicCpuThread();

public:
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::string _name;
    size_t _device_num = 1;
    std::vector<DataType> _inputDataTypes;  
    std::vector<DataType> _outputDataTypes; 
    int _numInputs = 1;
    int _numOutputs;
    int _numThreads = 1;
    int _initDynamicThreads = 0;
    std::vector<std::string> _inputNames;
    std::vector<const char*> _inputNamesChar;
    std::vector<std::string> _outputNames;
    std::vector<const char*> _outputNamesChar;
    std::vector<std::vector<int64_t>> _inputShapes;
    std::vector<std::vector<int64_t>> _outputShapes;
    std::vector<uint64_t> _inputOffsets = {0};
    std::vector<uint64_t> _outputOffsets;
    std::vector<uint64_t> _inputSizes;
    std::vector<uint64_t> _outputSizes;
    //std::shared_ptr<Buffer> _buffer;
    void* _cpuTaskOutputBufferPtr;
    std::shared_ptr<CpuHandleWorker> _worker=nullptr;

public:
    int InferenceRequest(RequestPtr req);
    void Start();
    void Run(RequestPtr req);
    void Terminate(void);
    friend DXRT_API std::ostream& operator<<(std::ostream&, const CpuHandle&);
};
} /* namespace dxrt */