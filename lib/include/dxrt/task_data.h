/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/model.h"
#include "dxrt/driver.h"
#include "dxrt/util.h"

namespace dxrt {

using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;

class CpuHandle;
class TaskData
{
 public:
    TaskData(int id_, std::string name_, rmapinfo info_);

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

    uint32_t weightChecksum();

    int encoded_input_size() const {return _encodedInputSize;}
    int encoded_output_size() const {return _encodedOutputSize;}

private:

   void calculate_sizes() {
      _inputSize = calculate_total_size(_inputDataTypes, _inputShapes);
      _outputSize = calculate_total_size(_outputDataTypes, _outputShapes);
   }

   uint32_t calculate_total_size(const std::vector<DataType>& dataTypes, const std::vector<std::vector<int64_t>>& shapes) {
      uint32_t totalSize = 0;
      if (dataTypes.size() != shapes.size()) {
         throw std::runtime_error("DataTypes and Shapes vectors must have the same size.");
      }
      for (size_t i = 0; i < dataTypes.size(); ++i) {
            uint32_t elementCount = std::accumulate(shapes[i].begin(), shapes[i].end(), 1LL, std::multiplies<int64_t>());
            totalSize += elementCount * GetDataSize_Datatype(dataTypes[i]);
      }
      return totalSize;
   }

 public:  // TODO(ykpark): make private
    int _id;
    std::string _name = "EMPTY";
    Processor _processor = Processor::NONE_PROCESSOR;

    rmapinfo _info;

    dxrt_model_t _npuModel;

    uint64_t _memUsage = 0;
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    std::vector<DataType> _inputDataTypes;
    std::vector<DataType> _outputDataTypes;
    int _numInputs;
    int _numOutputs;
    std::vector<std::string> _inputNames;
    std::vector<std::string> _outputNames;
    std::vector<std::vector<int64_t>> _inputShapes;
    std::vector<std::vector<int64_t>> _outputShapes;
    std::vector<uint64_t> _inputOffsets;
    std::vector<uint64_t> _encodedInputOffsets;
    std::vector<uint64_t> _outputOffsets;
    std::vector<uint64_t> _encodedOutputOffsets;
    //std::shared_ptr<Buffer> _taskOutputBuffer=nullptr;
    
    uint32_t _encodedInputSize = 0;
    uint32_t _encodedOutputSize = 0;
    std::vector<uint32_t> _encodedInputSizes;
    std::vector<uint32_t> _encodedOutputSizes;
    std::vector<DataType> _encodedInputDataTypes;
    std::vector<DataType> _encodedOutputDataTypes;
    std::vector<std::string> _encodedInputNames;
    std::vector<std::string> _encodedOutputNames;
    std::vector<std::vector<int64_t>> _encodedInputShapes;
    std::vector<std::vector<int64_t>> _encodedOutputShapes;

    Tensors _inputTensors;
    Tensors _outputTensors;

    std::vector<deepx_rmapinfo::TensorInfo> _npuInputTensorInfos;
    std::vector<deepx_rmapinfo::TensorInfo> _npuOutputTensorInfos;

    bool _isArgMax = false;
    bool _isPPU = false;
};

}  // namespace dxrt