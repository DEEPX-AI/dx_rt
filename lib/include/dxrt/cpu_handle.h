// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <vector>

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/request.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif

namespace dxrt {
class Buffer;
class Worker;
struct DXRT_API CpuHandle
{
    CpuHandle(void* data_, int64_t size_, std::string name_);
    ~CpuHandle();
#ifdef USE_ORT
    Ort::Env _env;
    Ort::SessionOptions _sessionOptions;
    std::shared_ptr<Ort::Session> _session;
#endif
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::vector<DataType> _inputDataTypes;  
    std::vector<DataType> _outputDataTypes; 
    std::vector<int> _inputDataTypesOrg;    
    std::vector<int> _outputDataTypesOrg;   
    int _numInputs = 1;
    int _numOutputs;
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
    std::shared_ptr<Buffer> _buffer;

    int InferenceRequest(RequestPtr req);
    void Start();
    void Run(RequestPtr req);
    void Terminate(void);
    std::shared_ptr<Worker> _worker=nullptr;
    friend DXRT_API std::ostream& operator<<(std::ostream&, const CpuHandle&);
};
} /* namespace dxrt */