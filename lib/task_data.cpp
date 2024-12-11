// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task_data.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/inference_engine.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/buffer.h"

using namespace std;

namespace dxrt {

TaskData::TaskData(int id_, std::string name_, std::vector<rmapinfo> info_)
: _id(id_), _name(name_), _infos(info_)
{
}

void TaskData::set_from_npu(const std::vector<std::vector<uint8_t>>& data_)
{
    _processor = Processor::NPU;
    for (auto &info : _infos)
    {
        _memUsage += info.size();
    }
    auto &info = _infos.front();
    _numOutputs = info.outputs().outputlist().output_size();
    {
        vector<int64_t> shapes;
        for (int i = 0; i < info.input().shapes().shape_size(); i++)
        {
            shapes.emplace_back(info.input().shapes().shape(i));
        }
        _inputShape.emplace_back(shapes);
        _inputNames.emplace_back(info.input().memory().name());
    }
    LOG_DXRT_DBG << "NPU Task: imported input shapes" << endl;
    {
        for (int i = 0; i < info.outputs().outputlist().output_size(); i++)
        {
            auto &node = info.outputs().outputlist().output(i);
            vector<int64_t> shapes;
            for (int j = 0; j < node.shapes().shape_size(); j++)
            {
                shapes.emplace_back(node.shapes().shape(j));
            }
            if (shapes.size() == 4)
            {
                shapes[3] = GetAlign(shapes[3]);
            }
            else if (shapes.size()>4) 
                throw std::out_of_range("Invalid tensor shape ( > 4D)");
            _outputShape.emplace_back(shapes);
            _outputOffsets.emplace_back(info.outputs().outputlist().output(i).memory().offset()
                - info.outputs().outputlist().output(0).memory().offset());
            _outputSize += info.outputs().outputlist().output(i).memory().size();
            _outputNames.emplace_back(node.name());
        }
    }
    LOG_DXRT_DBG << "NPU Task: imported output shapes"<< endl;
    _inputSize = info.input().memory().size();
    _inputDataTypes.push_back((DataType)info.input().type());
    if (info.outputs().outputlist().output_size() > 0)
    {
        for (int i = 0; i < _numOutputs; i++){
            _outputDataTypes.push_back((DataType)info.outputs().outputlist().output(0).type());
        }
    }
    else
    {
        _outputDataTypes.push_back(DataType::NONE_TYPE);
    }
    LOG_DXRT_DBG << "NPU Task: imported data types" << endl;

    for (int i = 0; i < _numInputs; i++)
    {
        // JG: npu task only has single input in current version
        _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShape[i], _inputDataTypes[0], nullptr));
    }
    for (int i = 0; i < _numOutputs; i++)
    {
       _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShape[i], _outputDataTypes[i], nullptr));
    }
    LOG_DXRT_DBG << "NPU Task: imported tensors" << endl;
    for (size_t i = 0; i < _infos.size(); i++)
    {
        auto &eachInfo = _infos[i];
        auto cmdSize = eachInfo.memorys().memory(0).size();
        auto weightSize = eachInfo.memorys().memory(1).size();
        dxrt_model_t model;
        model.npu_id = 0;
        model.type = 0;
        model.cmds = static_cast<int32_t>(eachInfo.counts().cmd());
        model.cmd.data = reinterpret_cast<uint64_t>(data_[ 2*i + 0 ].data() );
        model.cmd.base = 0;  // decided in device
        model.cmd.offset = 0;  // defined in device
        model.cmd.size = static_cast<uint32_t>(cmdSize);
        model.weight.data = reinterpret_cast<uint64_t>(data_[ 2*i + 1 ].data() );
        model.weight.base = 0;  // decided in device
        model.weight.offset = 0;  // defined in device
        model.weight.size = static_cast<uint32_t>(weightSize);
        model.output_all_offset = static_cast<uint32_t>(eachInfo.outputs().memory().offset());
        model.output_all_size = static_cast<uint32_t>(eachInfo.outputs().memory().size());
        model.last_output_offset = static_cast<uint32_t>(
            eachInfo.outputs().outputlist().output(0).memory().offset() - eachInfo.outputs().memory().offset());
        model.last_output_size = _outputSize;

        if (eachInfo.outputs().outputlist().output(0).memory().type() == deepx_rmapinfo::MemoryType::ARGMAX)
        {
            model.type = 1;
            // model.last_output_offset = model.output_all_size;
            // model.last_output_size = 64;
            // model.output_all_size += 64;
            // _outputSize = 64;
            model.last_output_size = 2;
            _outputSize = 2;
            _isArgMax = true;
        }
        else if (eachInfo.outputs().outputlist().output(0).memory().type() == deepx_rmapinfo::MemoryType::PPU)
        {
            if (eachInfo.outputs().outputlist().output(0).format() >= deepx_rmapinfo::DataFormat::PPU_YOLO
                && eachInfo.outputs().outputlist().output(0).format() <= deepx_rmapinfo::DataFormat::PPU_POSE )
            {
                model.type = 2;
                model.format = info.outputs().outputlist().output(0).format();
                _outputTensors.clear();


                int dataType = DataType::BBOX;
                dataType += info.outputs().outputlist().output(0).format();
                dataType -= deepx_rmapinfo::DataFormat::PPU_YOLO;
                _outputTensors.emplace_back(
                    Tensor(_outputNames[i], {128*1024/GetDataSize_Datatype(static_cast<DataType>(dataType))}, static_cast<DataType>(dataType), nullptr)
                    //Tensor(_outputNames[i], _outputShape[i], static_cast<DataType>(dataType), nullptr)
                );
                model.last_output_offset = model.output_all_size;
                model.last_output_size = 128*1024;
                model.output_all_size += 128*1024;
                _outputSize = 128*1024;
            }
            _isPPU = true;
        }
        if (_infos.size() > 1)
        {
            model.npu_id = -1;
        }
        else
        {
            if (info.version().npu() == "M1_8K")
            {
                model.npu_id = 1;
            }
            else
            {
                model.npu_id = 0;
            }
        }
        _npuModel.emplace_back(model);
    }
    _outputMemSize = 0;
    for (auto &model : _npuModel)
    {
        _outputMemSize = max(_outputMemSize, model.output_all_size);
    }
    LOG_DXRT_DBG << "NPU Task: imported npu parameters" << endl;
}
void TaskData::set_from_cpu(std::shared_ptr<CpuHandle> cpuHandle)
{
    _processor = Processor::CPU;
    // cout << *_cpuHandle << endl;
    _numInputs = cpuHandle->_numInputs;
    _numOutputs = cpuHandle->_numOutputs;
    _inputSize = cpuHandle->_inputSize;
    _outputSize = cpuHandle->_outputSize;
    _outputMemSize = _outputSize;
    _memUsage = _inputSize + _outputSize;
    _inputDataTypes = cpuHandle->_inputDataTypes;
    _outputDataTypes = cpuHandle->_outputDataTypes;
    _inputNames = cpuHandle->_inputNames;
    _outputNames = cpuHandle->_outputNames;
    _inputShape = cpuHandle->_inputShape;
    _outputShape = cpuHandle->_outputShape;
    _inputOffsets = cpuHandle->_inputOffsets;
    _outputOffsets = cpuHandle->_outputOffsets;
    for (int i = 0; i < _numInputs; i++)
    {
        _inputTensors.emplace_back(Tensor(_inputNames[i], _inputShape[i], _inputDataTypes[i], nullptr));
    }
    for (int i = 0; i < _numOutputs; i++)
    {
        _outputTensors.emplace_back(Tensor(_outputNames[i], _outputShape[i], _outputDataTypes[i], nullptr));
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
    return _inputTensors;
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
    return _outputTensors;
}
}  // namespace dxrt
