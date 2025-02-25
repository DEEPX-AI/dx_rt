// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/model.h"
#include "dxrt/driver.h"

namespace dxrt {

using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;

class CpuHandle;
class TaskData
{
 public:
    TaskData(int id_, std::string name_, std::vector<rmapinfo> info_);

    int id() const{return _id;}
    std::string name() const {return _name;}
    Processor processor() const {return _processor;}

    const Tensors& input_tensors() const {return _inputTensors;}
    const Tensors& output_tensors() const {return _outputTensors;}
    

    void set_from_npu(const std::vector<std::vector<uint8_t>>& data_);
    void set_from_cpu(std::shared_ptr<CpuHandle> cpuHandle);

    Tensors inputs(void* ptr, uint64_t phyAddr = 0);
    Tensors outputs(void* ptr, uint64_t phyAddr = 0);

    int input_size() const {return _inputSize;}
    int output_size() const {return _outputSize;}

 public:  // TODO(ykpark): make private
    int _id;
    std::string _name = "EMPTY";
    Processor _processor = Processor::NONE_PROCESSOR;

    std::vector<rmapinfo> _infos;

    std::vector<dxrt_model_t> _npuModel;

    uint64_t _memUsage = 0;
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::vector<DataType> _inputDataTypes;
    std::vector<DataType> _outputDataTypes;
    int _numInputs = 1;
    int _numOutputs;
    std::vector<std::string> _inputNames;
    std::vector<std::string> _outputNames;
    std::vector<std::vector<int64_t>> _inputShape;
    std::vector<std::vector<int64_t>> _outputShape;
    std::vector<uint64_t> _inputOffsets = {0};
    std::vector<uint64_t> _outputOffsets;
    //std::shared_ptr<Buffer> _taskOutputBuffer=nullptr;
    
    Tensors _inputTensors;
    Tensors _outputTensors;

    bool _isArgMax = false;
    bool _isPPU = false;
};

}  // namespace dxrt
