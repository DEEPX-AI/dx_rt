/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/task_data.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/inference_engine.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/buffer.h"
#include <limits>
#include <cstdint>
#include <sstream>



namespace {

// Convert layout metadata into legacy format field expected by firmware (layout-1 mapping)
int8_t LayoutToLegacyFormat(deepx_rmapinfo::Layout layout)
{
    const int layoutValue = static_cast<int>(layout);
    return layoutValue > 0 ? static_cast<int8_t>(layoutValue - 1) : 0;
}

// Map compile-time PPU type flag to the corresponding runtime layout descriptor
deepx_rmapinfo::Layout LayoutFromPpuType(int ppuType)
{
    using deepx_rmapinfo::Layout;
    switch (ppuType)
    {
        case 0:
        case 1:
            return Layout::PPU_YOLO;
        case 3:
            return Layout::PPU_FD;
        case 2:
            return Layout::PPU_POSE;
        default:
            return Layout::LAYOUT_NONE;
    }
}

}

namespace dxrt {

TaskData::TaskData(int id_, std::string name_, rmapinfo info_)
: _id(id_), _name(name_), _info(info_)
{
}

void TaskData::set_from_npu(const std::vector<std::vector<uint8_t>>& data_, bool hasPpuBinary)
{
    using std::endl;
    using std::vector;

    // Fix: Initialize with max value to find minimum offset correctly
    int64_t last_output_lower_bound = std::numeric_limits<int64_t>::max();
    int64_t last_output_upper_bound = 0;
    bool has_valid_output = false;  // Track if we have any valid outputs
    
    _processor = Processor::NPU;
    
    _numInputs = _info.inputs().size();
    _numOutputs = _info.outputs().size();

    {
        uint64_t orginal_tensor_offset = 0;
        for (int i = 0; i < _numInputs; i++)
        {
            auto &tensor_info = _info.inputs()[i];
            vector<int64_t> shape;
            uint64_t orginal_tensor_size = 1;
            for (size_t j = 0; j < tensor_info.shape().size(); j++)
            {
                shape.emplace_back(tensor_info.shape()[j]);
                orginal_tensor_size*=tensor_info.shape()[j];
            }
            orginal_tensor_size*=tensor_info.elem_size();
            vector<int64_t> encoded_shape;
            for (size_t j = 0; j < tensor_info.shape_encoded().size(); j++)
            {
                encoded_shape.emplace_back(tensor_info.shape_encoded()[j]);
            }
            _inputShapes.emplace_back(shape);
            _inputOffsets.emplace_back(orginal_tensor_offset);
            orginal_tensor_offset += orginal_tensor_size;
            _encodedInputOffsets.emplace_back(tensor_info.memory().offset());
            _inputNames.emplace_back(tensor_info.name());
            _npuInputTensorInfos.emplace_back(tensor_info);

            _encodedInputNames.emplace_back(tensor_info.name_encoded());
            _encodedInputShapes.emplace_back(encoded_shape);
            _encodedInputSize += tensor_info.memory().size();
            _encodedInputSizes.emplace_back(tensor_info.memory().size());
        }
    }
    LOG_DXRT_DBG << "NPU Task: imported input shapes" << endl;

    {
        uint64_t orginal_tensor_offset = 0;
        for (int i = 0; i < _numOutputs; i++)
        {
            auto &tensor_info = _info.outputs()[i];
            int64_t tensor_offset = tensor_info.memory().offset();
            vector<int64_t> shape;
            uint64_t orginal_tensor_size = 1;
            for (size_t j = 0; j < tensor_info.shape().size(); j++)
            {
                shape.emplace_back(tensor_info.shape()[j]);
                orginal_tensor_size*=tensor_info.shape()[j];
            }
            orginal_tensor_size*=tensor_info.elem_size();
            vector<int64_t> encoded_shape;
            for (size_t j = 0; j < tensor_info.shape_encoded().size(); j++)
            {
                encoded_shape.emplace_back(tensor_info.shape_encoded()[j]);
            }

            has_valid_output = true;
            if (last_output_lower_bound > tensor_offset)
                last_output_lower_bound = tensor_offset;
            if (last_output_upper_bound < tensor_offset + tensor_info.memory().size())
                last_output_upper_bound = tensor_offset + tensor_info.memory().size();
            
            _npuOutputTensorInfos.emplace_back(tensor_info);

            _outputNames.emplace_back(tensor_info.name());
            _outputShapes.emplace_back(shape);

            _encodedOutputNames.emplace_back(tensor_info.name_encoded());
            _encodedOutputShapes.emplace_back(encoded_shape);
            _encodedOutputSizes.emplace_back(tensor_info.memory().size());

            _outputOffsets.emplace_back(orginal_tensor_offset);
            orginal_tensor_offset += orginal_tensor_size;

            if (_info.outputs()[i].memory().type() == deepx_rmapinfo::MemoryType::ARGMAX) {
                _encodedOutputOffsets.emplace_back(_info.model_memory().output().size()); // temporary offset
                continue;
            }
            else if (_info.outputs()[i].memory().type() == deepx_rmapinfo::MemoryType::PPU) {
                // PPU outputs have dynamic size (size=0 in rmap_info), skip bound calculation
                _encodedOutputOffsets.emplace_back(0); // PPU output offset managed separately
                continue;
            }
            else
            {
                _encodedOutputOffsets.emplace_back(tensor_offset);

                if (last_output_lower_bound > tensor_offset)
                    last_output_lower_bound = tensor_offset;
                if (last_output_upper_bound < tensor_offset + tensor_info.memory().size())
                    last_output_upper_bound = tensor_offset + tensor_info.memory().size();
            }
        }

        // If no valid outputs exist, set to 0
        if (!has_valid_output || last_output_lower_bound == std::numeric_limits<int64_t>::max()) {
            last_output_lower_bound = 0;
        }
        
        // Validate the calculated bounds
        if (last_output_lower_bound < 0 || last_output_upper_bound < last_output_lower_bound) {
            std::stringstream err_msg;
            err_msg << "Invalid output memory bounds calculated: lower=" << last_output_lower_bound 
                    << ", upper=" << last_output_upper_bound;
            LOG_DXRT_ERR(err_msg.str());
            throw std::runtime_error("Invalid output memory bounds");
        }

        // After last_output_lower_bound is determined, subtract it from all values in _encodedOutputOffsets.
        for(auto &offset : _encodedOutputOffsets)
        {
            
            
            // Sanity check: offset should not be negative after normalization (but offset is int64_t which can be negative)
            if (offset < static_cast<uint64_t>(last_output_lower_bound))
            {
                std::stringstream err_msg;
                err_msg << "Negative offset after normalization: " << offset;
                LOG_DXRT_ERR(err_msg.str());
                throw std::runtime_error("Invalid offset normalization");
            }
            else
            {
                offset -= last_output_lower_bound;
            }
        }
    }
    
    // Calculate encoded output size with validation
    _encodedOutputSize = last_output_upper_bound - last_output_lower_bound;
    
    // Check if any output is PPU type (dynamic size)
    bool hasPPUOutput = false;
    for (int i = 0; i < _numOutputs; i++) {
        if (_info.outputs()[i].memory().type() == deepx_rmapinfo::MemoryType::PPU) {
            hasPPUOutput = true;
            break;
        }
    }
    
    // Validate encoded output size (skip for PPU outputs which have dynamic size)
    if (has_valid_output && !hasPPUOutput && _encodedOutputSize <= 0) {
        std::stringstream err_msg;
        err_msg << "Invalid encoded output size: " << _encodedOutputSize;
        LOG_DXRT_ERR(err_msg.str());
        throw std::runtime_error("Invalid encoded output size");
    }

    LOG_DXRT_DBG << "NPU Task: imported output shapes"<< endl;
    
    if (_numInputs > 0)
    {
        for (int i = 0; i < _numInputs; i++){
            _inputDataTypes.push_back((DataType)_info.inputs()[i].dtype());
            _encodedInputDataTypes.push_back((DataType)_info.inputs()[i].dtype_encoded());
        }
    }
    else
    {
        _inputDataTypes.push_back(DataType::NONE_TYPE);
        _encodedInputDataTypes.push_back(DataType::NONE_TYPE);

    }
    if (_numOutputs > 0)
    {
        for (int i = 0; i < _numOutputs; i++){
            _outputDataTypes.push_back((DataType)_info.outputs()[i].dtype());
            _encodedOutputDataTypes.push_back((DataType)_info.outputs()[i].dtype_encoded());
        }
    }
    else
    {
        _outputDataTypes.push_back(DataType::NONE_TYPE);
        _encodedOutputDataTypes.push_back(DataType::NONE_TYPE);
    }
    LOG_DXRT_DBG << "NPU Task: imported data types" << endl;

    calculate_sizes();

    for (int i = 0; i < _numInputs; i++)
    {
        int memType = static_cast<int>(_info.inputs()[i].memory().type());
        _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShapes[i], _inputDataTypes[i], nullptr, memType));
    }
    for (int i = 0; i < _numOutputs; i++)
    {
        int memType = static_cast<int>(_info.outputs()[i].memory().type());
        _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShapes[i], _outputDataTypes[i], nullptr, memType));
    }
    LOG_DXRT_DBG << "NPU Task: imported tensors" << endl;
    
    // Use actual data size from loaded buffers, not from rmap_info metadata
    // (rmap_info may have incorrect or zero sizes for some models)
    auto rmapSize = data_.size() > 0 ? data_[0].size() : 0;
    auto weightSize = data_.size() > 1 ? data_[1].size() : 0;
    
    _npuModel.npu_id = 0;
    _npuModel.type = 0;
    _npuModel.cmds = static_cast<int32_t>(_info.counts().cmd());
    _npuModel.op_mode = _info.counts()._op_mode;
    for(int i = 0; i < MAX_CHECKPOINT_COUNT; i++)
        _npuModel.checkpoints[i] = _info.counts()._checkpoints[i];

    _npuModel.rmap.data = reinterpret_cast<uint64_t>(data_[0].data() );
    _npuModel.rmap.base = 0;  // decided in device
    _npuModel.rmap.offset = 0;  // defined in device
    _npuModel.rmap.size = static_cast<uint32_t>(rmapSize);
    _npuModel.weight.data = reinterpret_cast<uint64_t>(data_[1].data() );
    _npuModel.weight.base = 0;  // decided in device
    _npuModel.weight.offset = 0;  // defined in device
    _npuModel.weight.size = static_cast<uint32_t>(weightSize);
    _npuModel.input_all_offset = static_cast<uint32_t>(_info.model_memory().input().offset());
    _npuModel.input_all_size = static_cast<uint32_t>(_info.model_memory().input().size());
    _npuModel.output_all_offset = static_cast<uint32_t>(_info.model_memory().output().offset());
    _npuModel.output_all_size = static_cast<uint32_t>(_info.model_memory().output().size());
    
    // Validate before casting to uint32_t to prevent overflow
    int64_t last_offset_calc = _info.model_memory().output().offset() + last_output_lower_bound;
    if (last_offset_calc < 0 || last_offset_calc > UINT32_MAX) {
        std::stringstream err_msg;
        err_msg << "Invalid last_output_offset calculation: " << last_offset_calc 
                << " (output.offset=" << _info.model_memory().output().offset() 
                << ", lower_bound=" << last_output_lower_bound << ")";
        LOG_DXRT_ERR(err_msg.str());
        throw std::runtime_error("Invalid last_output_offset - possible overflow");
    }
    
    _npuModel.last_output_offset = static_cast<uint32_t>(last_offset_calc);
    _npuModel.last_output_size = _encodedOutputSize;
    
    _isPPU = false;
    _isPPCPU = false;

    // Check for PPCPU type (v8 with PPU binary) - HIGHEST PRIORITY
    if (hasPpuBinary)
    {
        _npuModel.type = 3;
        _isPPCPU = true;
        _isPPU = true;  // PPCPU is a type of PPU processing
        DataType dataType = static_cast<DataType>(_info.outputs()[0].dtype());
        int memType = static_cast<int>(_info.outputs()[0].memory().type());

        // v8: Set format and datatype based on PPU type from compile_config.json
        // ppu_type: 0/1 -> YOLO, 3 -> FD, 2 -> POSE (ordering preserved for backward compatibility)
        const int ppuType = _info.ppu_type();
        const auto ppuLayout = LayoutFromPpuType(ppuType);
        std::string ppu_tensor_name = "NONE";

        if (ppuLayout != deepx_rmapinfo::Layout::LAYOUT_NONE) {
            _npuModel.format = LayoutToLegacyFormat(ppuLayout);
            switch (ppuLayout) {
                case deepx_rmapinfo::Layout::PPU_YOLO:
                    dataType = DataType::BBOX;
                    ppu_tensor_name = "BBOX";
                    break;
                case deepx_rmapinfo::Layout::PPU_FD:
                    dataType = DataType::FACE;
                    ppu_tensor_name = "FACE";
                    break;
                case deepx_rmapinfo::Layout::PPU_POSE:
                    dataType = DataType::POSE;
                    ppu_tensor_name = "POSE";
                    break;
                default:
                    break;
            }
        } else {
            // Fallback to legacy behavior if ppu_type is not set or invalid
            const auto fallbackLayout = static_cast<deepx_rmapinfo::Layout>(_info.outputs()[0].layout());
            _npuModel.format = LayoutToLegacyFormat(fallbackLayout);
        }
        
        _outputTensors.clear();

        _outputTensors.emplace_back(
            //Tensor("PPU_OUTPUT", {1, _outputSize/GetDataSize_Datatype(dataType)}, dataType, nullptr)
            Tensor(ppu_tensor_name, _outputShapes[0], dataType, nullptr, memType)

        );
        _npuModel.output_all_size *= 2; // Allocate double size for combined output

        LOG_DXRT_DBG << "NPU Task: PPCPU type detected (v8 with PPU binary)" << std::endl;
    }
    // Check all outputs for ARGMAX type
    else
    {
        bool allAreArgmax = true;
        for (int i = 0; i < _numOutputs; i++) {
            if (_info.outputs()[i].memory().type() == deepx_rmapinfo::MemoryType::ARGMAX) {
                _npuModel.type = 1;
            } else {
                allAreArgmax = false;
            }
        }
        
        // Only set type=1 and special handling if ALL outputs are ARGMAX
        if (allAreArgmax)
        {
            _isArgMax = true;
            _npuModel.last_output_size = 2;
            _outputSize = 2;
            _encodedOutputSize = _outputSize;
        }
        else if (_info.outputs()[0].memory().type() == deepx_rmapinfo::MemoryType::PPU)
        {
            
            _npuModel.type = 2;
            
            // When updating from .dxnn v6 to v7, format was replaced with layout. Applying correction value to connect with existing m1 fw dataformat
            const auto layout = static_cast<deepx_rmapinfo::Layout>(_info.outputs()[0].layout());
            _npuModel.format = LayoutToLegacyFormat(layout);
            
            _outputTensors.clear();

            int dataType = _info.outputs()[0].dtype();
            int memType = static_cast<int>(_info.outputs()[0].memory().type());

            _outputTensors.emplace_back(
                Tensor(_outputNames[0], _outputShapes[0], static_cast<DataType>(dataType), nullptr, memType)
            );
            _npuModel.last_output_offset = _npuModel.output_all_size; 
            //inference acc output offset -> input offset + input size (or output all offset) + output all size
#if DXRT_USB_NETWORK_DRIVER == 0
            _npuModel.last_output_size = 128*1024;
            _npuModel.output_all_size += 128*1024;
            _outputSize = 128*1024;
#else
            _npuModel.last_output_size = 16*1024;
            _npuModel.output_all_size += 16*1024;
            _outputSize = 16*1024;
#endif
            _encodedOutputSize = _outputSize;
            _isPPU = true;
        }
    }

    if (_info.version().npu() == "M1_8K")
    {
        _npuModel.npu_id = 1;
    }
    else
    {
        _npuModel.npu_id = 0;
    }
    
    _outputMemSize = std::max(static_cast<uint32_t>(0), _npuModel.output_all_size);
    _memUsage = rmapSize + weightSize + (static_cast<uint64_t>(_encodedInputSize) * DXRT_TASK_MAX_LOAD) + (static_cast<uint64_t>(_outputMemSize) * DXRT_TASK_MAX_LOAD);
    LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
}

void TaskData::set_from_cpu(std::shared_ptr<CpuHandle> cpuHandle)
{
    _processor = Processor::CPU;
    // cout << *_cpuHandle << std::endl;
    _numInputs = cpuHandle->_numInputs;
    _numOutputs = cpuHandle->_numOutputs;
    _inputSize = cpuHandle->_inputSize;
    _outputSize = cpuHandle->_outputSize;
    _outputMemSize = _outputSize;
    _memUsage = (static_cast<uint64_t>(_inputSize) * DXRT_TASK_MAX_LOAD) + (static_cast<uint64_t>(_outputMemSize) * DXRT_TASK_MAX_LOAD);
    _inputDataTypes = cpuHandle->_inputDataTypes;
    _outputDataTypes = cpuHandle->_outputDataTypes;
    _inputNames = cpuHandle->_inputNames;
    _outputNames = cpuHandle->_outputNames;
    _inputShapes = cpuHandle->_inputShapes;
    _outputShapes = cpuHandle->_outputShapes;
    _inputOffsets = cpuHandle->_inputOffsets;
    _outputOffsets = cpuHandle->_outputOffsets;
    for (int i = 0; i < _numInputs; i++)
    {
        _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShapes[i], _inputDataTypes[i], nullptr)); // CPU uses DRAM (default)
    }
    for (int i = 0; i < _numOutputs; i++)
    {
        _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShapes[i], _outputDataTypes[i], nullptr)); // CPU uses DRAM (default)
    }
}

Tensors TaskData::inputs(void* ptr, uint64_t phyAddr)
{
    if (ptr == nullptr)
    {
        return _inputTensors;
    }
    else
    {
        Tensors ret(_inputTensors);
        int i = 0;
        for (auto &t : ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _inputOffsets[i]);
            t.phy_addr() = phyAddr + _inputOffsets[i];
            i++;
        }
        return ret;
    }
}

Tensors TaskData::outputs(void* ptr, uint64_t phyAddr)
{
    if (ptr == nullptr)
    {
        return _outputTensors;
    }
    else
    {
        Tensors ret(_outputTensors);
        int i = 0;
        for (auto &t : ret)
        {
            t.data() = static_cast<void*>(static_cast<uint8_t*>(ptr) + _outputOffsets[i]);
            t.phy_addr() = phyAddr + _outputOffsets[i];
            i++;
        }
        return ret;
    }
}

uint32_t TaskData::weightChecksum()
{
    uint32_t value = 0;
    uint32_t* ptr = reinterpret_cast<uint32_t*>(_npuModel.weight.data);
    uint32_t size = _npuModel.weight.size;
    size /= sizeof(uint32_t);
    for (uint32_t i = 0; i < size; i++)
    {
        value ^= ptr[i];
    }
    return value;
}



}  // namespace dxrt
