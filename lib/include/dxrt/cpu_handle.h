// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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
    CpuHandle(void* data_, int64_t size_, std::string name_);
    ~CpuHandle();
#ifdef USE_ORT
    Ort::Env _env;
    Ort::SessionOptions _sessionOptions;
    std::shared_ptr<Ort::Session> _session;
#endif

public:
    static std::atomic<int> _totalNumThreads;
    static bool _dynamicCpuThread;
    static void SetDynamicCpuThread();

public:
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::vector<DataType> _inputDataTypes;  
    std::vector<DataType> _outputDataTypes; 
    int _numInputs = 1;
    int _numOutputs;
    int _numThreads = 1;
    int _initDynamicThreads = 0;
    std::string _name;
    std::vector<std::string> _inputNames;
    std::vector<const char*> _inputNamesChar;
    std::vector<std::string> _outputNames;
    std::vector<const char*> _outputNamesChar;
    std::vector<std::vector<int64_t>> _inputShape;
    std::vector<std::vector<int64_t>> _outputShape;
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