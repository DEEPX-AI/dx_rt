/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


// StdDeviceTaskLayer implementations separated from device_task_layer.cpp

// Std path implementation
#include <vector>
#include <cstring>
#include "dxrt/common.h"
#include "dxrt/device_task_layer.h"
#include "dxrt/task_data.h"
#include "dxrt/request_data.h"
#include "dxrt/task.h"
#include "dxrt/profiler.h"
#include "dxrt/request_response_class.h"

namespace dxrt {

// Local constant (mirrors original macro usage)
#define DEVICE_NUM_BUF 2

int StdDeviceTaskLayer::RegisterTask(TaskData* task)
{
    UniqueLock lock(_taskDataLock);
    LOG_DXRT_DBG << "Device " << id() << " RegisterTask STD" << std::endl;
    int ret = 0;
    const int tId = task->id();
    _bufIdx[tId] = 0;

    dxrt_model_t model = task->_npuModel;
    _npuInference[tId].clear();

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    model.rmap.base = _core->info().mem_addr;
    model.weight.base = _core->info().mem_addr;

    {
        model.rmap.offset = static_cast<uint32_t>(Allocate(model.rmap.size));
        model.weight.offset = static_cast<uint32_t>(Allocate(model.weight.size));
        if (model.rmap.offset > model.weight.offset) {
            model.rmap.offset = static_cast<uint32_t>(Allocate(model.rmap.size));
        }
    }

    for (int j = 0; j < DEVICE_NUM_BUF; ++j) {
        uint32_t inference_offset = 0;
        const uint64_t aligned_in = ((static_cast<uint64_t>(task->input_size()) + 63ull) & ~63ull);
        const uint64_t input_block = (model.output_all_offset == 0) ? aligned_in
                                                                    : static_cast<uint64_t>(model.output_all_offset);
        inference_offset = static_cast<uint32_t>(Allocate(input_block));

        dxrt_request_t inf{};
        inf.req_id = 0;
        inf.input.data = 0;
        inf.input.base = model.rmap.base; // same base
        inf.input.offset = inference_offset;
        inf.input.size = task->input_size();
        inf.output.data = 0;
        inf.output.base = model.rmap.base;
        inf.output.offset = static_cast<uint32_t>(Allocate(model.output_all_size));
        inf.output.size = model.output_all_size;

        inf.model_type = static_cast<uint32_t>(model.type);
        inf.model_format = static_cast<uint32_t>(model.format);
        inf.model_cmds = static_cast<uint32_t>(model.cmds);
        inf.cmd_offset = model.rmap.offset;
        inf.weight_offset = model.weight.offset;
        inf.last_output_offset = model.last_output_offset;

        if (_memoryMapBuffer == 0)
        {
            std::vector<uint8_t> buf(model.output_all_size);
            _outputValidateBuffers[tId] = std::move(buf);
        }
        else
        {
            inf.input.data = _memoryMapBuffer + inf.input.offset;
            inf.output.data = _memoryMapBuffer + inf.output.offset + inf.last_output_offset;
            void *start = static_cast<void*>(reinterpret_cast<uint8_t*>(_memoryMapBuffer) + inf.output.offset);
            void *end = static_cast<void*>(static_cast<uint8_t*>(start) + model.output_all_size);
            // LOG_VALUE_HEX(start);
            // LOG_VALUE_HEX(end);
            // _outputValidateBuffers[id] = vector<uint8_t>((uint8_t*)(_memory->data()) + inference.output.offset, (uint8_t*)(_memory->data()) + model.output_all_size);
            if (model.output_all_size == 0) {
                LOG_DXRT_WARN("Task " << tId << " output_all_size is 0, allocating minimum buffer" << std::endl);
                _outputValidateBuffers[tId] = std::vector<uint8_t>(1);  // Prevent empty vector
            } else {
                _outputValidateBuffers[tId] = std::vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
            }
            // LOG_VALUE_HEX(inference
        }

        _npuInference[tId].emplace_back(inf);

        DXRT_ASSERT(_core->Write(model.rmap) == 0, "failed to write model parameters(rmap)");
        DXRT_ASSERT(_core->Write(model.weight) == 0, "failed to write model parameters(weight)");
    }

    {
        std::vector<std::vector<uint8_t>> readData;
        readData.emplace_back(std::vector<uint8_t>(model.rmap.size));
        readData.emplace_back(std::vector<uint8_t>(model.weight.size));
        dxrt_meminfo_t cmd(model.rmap);
        dxrt_meminfo_t weight(model.weight);
        cmd.data = reinterpret_cast<uint64_t>(readData[0].data());
        weight.data = reinterpret_cast<uint64_t>(readData[1].data());
        if (_core->Read(cmd) == 0) {
            ret += memcmp(reinterpret_cast<void*>(cmd.data), readData[0].data(), cmd.size);
        }
        if (_core->Read(weight) == 0) {
            ret += memcmp(reinterpret_cast<void*>(weight.data), readData[1].data(), weight.size);
        }
        DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters" + std::to_string(ret));
    }

    for (auto &inf : _npuInference[tId]) {
        _inputTensors[tId].emplace_back(task->inputs(reinterpret_cast<void*>(inf.input.data),
            inf.input.base + inf.input.offset));
        _outputTensors[tId].emplace_back(task->outputs(reinterpret_cast<void*>(inf.output.data),
            inf.output.base + inf.output.offset));
    }

    for (const auto &v : _inputTensors[tId])
        for (const auto &tensor : v)
            LOG_DXRT << tensor << std::endl;
    for (const auto &v : _outputTensors[tId])
        for (const auto &tensor : v)
            LOG_DXRT << tensor << std::endl;

    return ret;
}

void StdDeviceTaskLayer::StartThread()
{
    _memoryMapBuffer = reinterpret_cast<uint64_t>(core()->CreateMemoryMap());
    LOG_DXRT_DBG << "StartThread: Memory Map buffer " << std::hex << _memoryMapBuffer << std::dec << std::endl;
    _thread = std::thread(&StdDeviceTaskLayer::ThreadImpl, this);
}

void StdDeviceTaskLayer::ThreadImpl()
{
    int ret = 0;
    LOG_DXRT_DBG << "Device " << id() << " thread start. " << std::endl;
    while (true)
    {
        if (_stop.load()) break;
        dxrt_response_t response;
        response.req_id = 0;
        LOG_DXRT_DBG << "Device " << id() << " wait. " << std::endl;
#ifdef USE_PROFILER
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name_wait = "ThreadImpl Wait[device "+std::to_string(id())+"]";
        profiler.Start(profile_name_wait);
#endif
        ret = core()->Wait();

#ifdef USE_PROFILER
        profiler.End(profile_name_wait);
#endif
        if (_stop.load()) break;
        // LOG_VALUE(ret);
        ret = core()->ReadDriverData(&response, sizeof(dxrt_response_t));
        if (_stop.load()) break;
        LOG_DXRT_DBG << "Device " << id() << " got response " << response.req_id << std::endl;
        if ((ret == 0) & (response.req_id != 0xFFFFFFFF))  // 0xFFFFFFFF: clear value
        {
            // cout << "response " << response.req_id << ", inf time " << response.inf_time << ", load " << load() << endl;
            auto req = Request::GetById(response.req_id);
            // LOG_VALUE(req.use_count());
            if (req != nullptr)
            {
                // LOG_VALUE(req->model_type());
                if (req->model_type() == 1)
                {
                    // LOG_VALUE(resp.argmax);
                    *(static_cast<uint16_t *>(req->getData()->outputs.front().data())) = response.argmax;
                    // if (DEBUG_DATA > 0)
                    //    DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
                }
                else if (req->model_type() == 2)
                {
                    // LOG_VALUE(resp.ppu_filter_num);

                    std::vector<int64_t> shape{1, response.ppu_filter_num};
                    Tensors newOutput;
                    Tensors oldOutput = req->outputs();
                    auto fronts = oldOutput.front();
                    newOutput.emplace_back(fronts.name(), shape, fronts.type(), fronts.data());
                    for(size_t i = 1; i < oldOutput.size(); i++)
                    {
                        newOutput.push_back(oldOutput[i]);
                    }
                    req->setOutputs(newOutput);
                    DXRT_ASSERT(req->getData()->outputs.front().shape()[1] == response.ppu_filter_num, "PPU MODEL OUTPUT NOT VALID SET");

                    // req->task()->getData()->_outputSize = req->getData()->outputs.front().shape()[1]*32;//task->output_size() setting

                    //if (DEBUG_DATA > 0)
                    //    DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
                }

                RequestResponse::ProcessResponse(req, response, 1);
                CallBack();
            }
        }
    }
    LOG_DXRT_DBG << "Device " << id() << " thread end. ret:"<< ret << std::endl;
}

int StdDeviceTaskLayer::Release(TaskData* task)
{
    UniqueLock lock(_taskDataLock);
    int taskId = task->id();
    auto &model = _npuModel[taskId];
    _serviceLayer->DeAllocate(id(), model.rmap.offset);
    _serviceLayer->DeAllocate(id(), model.weight.offset);
    for (auto &inf : _npuInference[taskId])
    {
        _serviceLayer->DeAllocate(id(), inf.input.offset);
        _serviceLayer->DeAllocate(id(), inf.output.offset);
    }
    _npuModel.erase(taskId);
    return 0;
}

int StdDeviceTaskLayer::InferenceRequest(RequestData* req, npu_bound_op boundOp)
{
    SharedLock lock(_taskDataLock);
    std::ignore = boundOp;
    LOG_DXRT_DBG << "Device " << id() << " inference request" << std::endl;
    int ret = 0;
    int bufId = 0;
    auto task = req->taskData;
    int taskId = task->id();
    std::unique_lock<std::mutex> lk(_lock);
    bufId = _bufIdx[taskId];
    (++_bufIdx[taskId]) %= DEVICE_NUM_BUF;

    void* reqInputPtr = nullptr;
    if (req->inputs.size() > 0)
        reqInputPtr = req->inputs.front().data();

    {
        auto &inferences = _npuInference[taskId];
        int pick = -1;
        for (size_t i = 0; i < inferences.size(); i++)
        {
            if (reinterpret_cast<void*>(inferences[i].input.data) == reqInputPtr)
            {
                pick = static_cast<int>(i);
                req->outputs = _outputTensors[taskId][i];
                break;
            }
        }
        if (pick == -1)
        {
            pick = bufId;
            void *dest = reinterpret_cast<void*>(inferences[pick].input.data);
            if (reqInputPtr == nullptr)
            {
                reqInputPtr = dest;
            }
            else
            {
                LOG_DXRT_DBG << std::hex << "memcpy " << reqInputPtr << "-> " << dest << std::dec << "(pick " << pick << ")" << std::endl;
#ifdef USE_PROFILER
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name = "STD Memcpy[device "+std::to_string(id())+" pick" + std::to_string(pick) + "]";
                profiler.Start(profile_name);
#endif
                memcpy(dest, reqInputPtr, task->_encodedInputSize);
                //Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
#ifdef USE_PROFILER
                profiler.End(profile_name);
#endif
                _core->Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, reinterpret_cast<void*>(&inferences[pick].input));
            }
            req->outputs = _outputTensors[taskId][pick];
        }
        auto npu_inference = inferences[pick];
        npu_inference.req_id = req->requestId;
        {
            UniqueLock lk2(requestsLock);
            _ongoingRequestsStd[req->requestId] = npu_inference;
        }
        LOG_DXRT_DBG << "Device " << id() << " Request : " << inferences[pick] << std::endl;
#ifdef USE_PROFILER

        // Start profiling for overall NPU task (input preprocess + PCIe + NPU execution + output postprocess)
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name_write = "STD Write[device "+std::to_string(id())+" pick" + std::to_string(pick) + "]";
        profiler.Start(profile_name_write);
#endif

#ifdef __linux__
        ret = _core->WriteData(&npu_inference, sizeof(dxrt_request_t));
#elif _WIN32
        ret = _core->WriteData(&npu_inference, sizeof(dxrt_request_t));
#endif
        LOG_DXRT_DBG << "written " << ret << std::endl;
#ifdef USE_PROFILER
        profiler.End(profile_name_write);
#endif
    }
    return 0;
}

void StdDeviceTaskLayer::ProcessResponseFromService(const dxrt::_dxrt_response_t& response)
{
    std::ignore = response;
    DXRT_ASSERT(false, "UNIMPLEMENTED StdDeviceTaskLayer::ProcessResponseFromService");
}

StdDeviceTaskLayer::~StdDeviceTaskLayer()
{
    _stop = true;
    Terminate();
    if (_thread.joinable())
        _thread.join();
}

} // namespace dxrt
