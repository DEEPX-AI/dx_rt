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
#include "dxrt/model_type.h"
#include "dxrt/safe_cast.h"

namespace dxrt {

// Local constant (mirrors original macro usage)
static constexpr int DEVICE_NUM_BUF = 2;

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

    model.rmap.base = core()->info().mem_addr;
    model.weight.base = core()->info().mem_addr;

    {
        model.rmap.offset = static_cast<uint32_t>(Allocate(model.rmap.size));
        model.weight.offset = static_cast<uint32_t>(Allocate(model.weight.size));
        if (model.rmap.offset > model.weight.offset) {
            model.rmap.offset = static_cast<uint32_t>(Allocate(model.rmap.size));
        }
    }

    for (int j = 0; j < DEVICE_NUM_BUF; ++j) {
        uint32_t inference_offset = 0;
        const uint64_t aligned_in = ((static_cast<uint64_t>(task->input_size()) + 63ULL) & ~63ULL);
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
            auto start = static_cast<void*>(SafeCast::IntegerToPointer<uint8_t*>(_memoryMapBuffer) + inf.output.offset);
            auto end = static_cast<void*>(static_cast<uint8_t*>(start) + model.output_all_size);



            if (model.output_all_size == 0) {
                LOG_DXRT_WARN("Task " << tId << " output_all_size is 0, allocating minimum buffer" << std::endl);
                _outputValidateBuffers[tId] = std::vector<uint8_t>(1);  // Prevent empty vector
            } else {
                _outputValidateBuffers[tId] = std::vector<uint8_t>(static_cast<uint8_t*>(start), static_cast<uint8_t*>(end));
            }
        }


        _npuInference[tId].emplace_back(inf);

        DXRT_ASSERT(core()->Write(model.rmap) == 0, "failed to write model parameters(rmap)");
        DXRT_ASSERT(core()->Write(model.weight) == 0, "failed to write model parameters(weight)");
    }

    {
        std::vector<std::vector<uint8_t>> readData;
        readData.emplace_back(model.rmap.size);
        readData.emplace_back(model.weight.size);
        dxrt_meminfo_t cmd(model.rmap);
        dxrt_meminfo_t weight(model.weight);
        cmd.data = SafeCast::PointerToInteger<uint8_t*>(readData[0].data());
        weight.data = SafeCast::PointerToInteger<uint8_t*>(readData[1].data());
        if (core()->Read(cmd) == 0) {
            ret += memcmp(SafeCast::IntegerToPointer<void*>(cmd.data), readData[0].data(), cmd.size);
        }
        if (core()->Read(weight) == 0) {
            ret += memcmp(SafeCast::IntegerToPointer<void*>(weight.data), readData[1].data(), weight.size);
        }
        DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters" + std::to_string(ret));
    }

    for (const auto &inf : _npuInference[tId]) {
        _inputTensors[tId].emplace_back(task->inputs(SafeCast::IntegerToPointer<void*>(inf.input.data),
            inf.input.base + inf.input.offset));
        _outputTensors[tId].emplace_back(task->outputs(SafeCast::IntegerToPointer<void*>(inf.output.data),
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
    _memoryMapBuffer = SafeCast::PointerToInteger<void*>(core()->CreateMemoryMap());
    LOG_DXRT_DBG << "StartThread: Memory Map buffer " << std::hex << _memoryMapBuffer << std::dec << std::endl;
    _thread = std::thread(&StdDeviceTaskLayer::ThreadImpl, this);
}

void StdDeviceTaskLayer::ThreadImpl()
{
    int ret = 0;
    bool shouldExit = false;
    LOG_DXRT_DBG << "Device " << id() << " thread start. " << std::endl;
    while (!shouldExit)
    {
        if (isStopFlag())
        {
            shouldExit = true;
            continue;
        }
        dxrt_response_t response;
        response.req_id = 0;
        LOG_DXRT_DBG << "Device " << id() << " wait. " << std::endl;
#ifdef USE_PROFILER
        auto& profiler = dxrt::Profiler::GetInstance();
        std::string profile_name_wait = "ThreadImpl Wait[device "+std::to_string(id())+"]";
        profiler.Start(profile_name_wait);
#endif
        std::ignore = core()->Wait();

#ifdef USE_PROFILER
        profiler.End(profile_name_wait);
#endif
        if (isStopFlag())
        {
            shouldExit = true;
            continue;
        }

        ret = core()->ReadDriverData(&response, sizeof(dxrt_response_t));
        if (isStopFlag())
        {
            shouldExit = true;
            continue;
        }
        LOG_DXRT_DBG << "Device " << id() << " got response " << response.req_id << std::endl;
        if ((ret == 0) && (response.req_id != 0xFFFFFFFF))  // 0xFFFFFFFF: clear value
        {

            auto req = Request::GetById(response.req_id);

            if (req != nullptr)
            {

                if (req->modelType() == ModelType::MODEL_TYPE_ARGMAX)
                {

                    *(static_cast<uint16_t *>(req->getData()->outputs.front().data())) = response.argmax;
                }
                else if (req->modelType() == ModelType::MODEL_TYPE_PPU)
                {
                    std::vector<int64_t> shape{1, response.ppu_filter_num};
                    Tensors newOutput;
                    Tensors oldOutput = req->outputs();
                    auto fronts = oldOutput.front();
                    newOutput.emplace_back(fronts.name(), shape, fronts.type(), fronts.data());
                    for (size_t i = 1; i < oldOutput.size(); i++)
                    {
                        newOutput.push_back(oldOutput[i]);
                    }
                    req->setOutputs(newOutput);
                    DXRT_ASSERT(req->getData()->outputs.front().shape()[1] == response.ppu_filter_num, "PPU MODEL OUTPUT NOT VALID SET");
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
    const auto& model = npuModelMap()[taskId];
    serviceLayer()->DeAllocate(id(), model.rmap.offset);
    serviceLayer()->DeAllocate(id(), model.weight.offset);
    for (const auto &inf : _npuInference[taskId])
    {
        serviceLayer()->DeAllocate(id(), inf.input.offset);
        serviceLayer()->DeAllocate(id(), inf.output.offset);
    }
    npuModelMap().erase(taskId);
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
    std::unique_lock<std::mutex> lk(stateLock());
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
            if (SafeCast::IntegerToPointer<void*>(inferences[i].input.data) == reqInputPtr)
            {
                pick = static_cast<int>(i);
                req->outputs = _outputTensors[taskId][i];
                break;
            }
        }
        if (pick == -1)
        {
            pick = bufId;
            void *dest = SafeCast::IntegerToPointer<void*>(inferences[pick].input.data);
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

#ifdef USE_PROFILER
                profiler.End(profile_name);
#endif
                core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_CPU_CACHE_FLUSH, static_cast<void*>(&inferences[pick].input));
            }
            req->outputs = _outputTensors[taskId][pick];
        }
        std::ignore = ret;
        std::ignore = reqInputPtr;
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
        ret = core()->WriteData(&npu_inference, sizeof(dxrt_request_t));
#elif _WIN32
        ret = core()->WriteData(&npu_inference, sizeof(dxrt_request_t));
#endif
        std::ignore = ret;
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
    setStopFlag(true);
    Terminate();
    if (_thread.joinable())
        _thread.join();
}

} // namespace dxrt
