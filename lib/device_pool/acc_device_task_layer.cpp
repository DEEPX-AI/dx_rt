/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


// Accelerator-specific Device Task Layer implementations separated from device_task_layer.cpp

#include <vector>
#include <cstring>
#include <chrono>
#include <thread>
#include "dxrt/device_task_layer.h"
#include "dxrt/task_data.h"
#include "dxrt/request_data.h"
#include "dxrt/request_response_class.h"
#include "dxrt/configuration.h"
#include "dxrt/device_struct_operators.h"
#include "dxrt/npu_format_handler.h"
#include "dxrt/objects_pool.h"
#include "dxrt/util.h"
#include "dxrt/datatype.h"
#include "dxrt/runtime_event_dispatcher.h"
#include "../resource/log_messages.h"

#include <memory>
#ifdef DXRT_USE_DEVICE_VALIDATION
#include "dxrt/task.h"
#endif

#include "../data/ppcpu.h"

// Macros duplicated from original implementation unit (can be refactored later)
#define RMAP_RECOVERY_DONE      (1)
#define WEIGHT_RECOVERY_DONE    (2)

namespace dxrt {

extern uint8_t DEBUG_DATA;
extern uint8_t SKIP_INFERENCE_IO;


AccDeviceTaskLayer::AccDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface)
: DeviceTaskLayer(dev, service_interface), _inputHandlerQueue(dev->name()+"_input", dev->GetReadChannel(),
    std::bind(&AccDeviceTaskLayer::InputHandler, this, std::placeholders::_1, std::placeholders::_2)),
    _outputHandlerQueue(dev->name()+"_output", dev->GetWriteChannel(),
    std::bind(&AccDeviceTaskLayer::OutputHandler, this, std::placeholders::_1, std::placeholders::_2))
{}


int AccDeviceTaskLayer::RegisterTask(TaskData* task)
{
    LOG_DXRT_DBG << "Device " << id() << " RegisterTask ACC" << std::endl;
    int ret = 0;
    const int tId = task->id();
    UniqueLock lock(_taskDataLock);

    dxrt_model_t model = task->_npuModel;

    _npuModel[tId] = model;

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    model.rmap.base = _core->info().mem_addr;
    model.weight.base = _core->info().mem_addr;

    // Allocate model param regions (simple forward allocation)
    model.weight.offset = _serviceLayer->BackwardAllocateForTask(id(), tId, model.weight.size);
    model.rmap.offset = _serviceLayer->BackwardAllocateForTask(id(), tId, model.rmap.size);
    if (model.rmap.offset > model.weight.offset)
    {
        uint32_t temp_addr = model.rmap.offset;
        model.rmap.offset = _serviceLayer->BackwardAllocateForTask(id(), tId, model.rmap.size);
        _serviceLayer->DeAllocate(id(), temp_addr);
    }

    dxrt_request_acc_t inf{};
    memset(static_cast<void *>(&inf), 0x00, sizeof(dxrt_request_acc_t));
    inf.task_id = tId;
    inf.req_id = 0;
    inf.input.data = 0;
    inf.input.base = model.rmap.base;
    inf.input.offset = 0;
    inf.input.size = task->encoded_input_size();
    inf.output.data = 0;
    inf.output.base = model.rmap.base;
    // V7 default (will be overwritten during runtime request as needed)
    inf.output.offset = model.last_output_offset;
    inf.output.size = model.last_output_size;

    inf.model_type = static_cast<uint32_t>(model.type);
    inf.model_format = static_cast<uint32_t>(model.format);
    inf.model_cmds = static_cast<uint32_t>(model.cmds);
    inf.cmd_offset = model.rmap.offset;
    inf.weight_offset = model.weight.offset;
    inf.op_mode = model.op_mode;
    for (int i = 0; i < MAX_CHECKPOINT_COUNT; ++i)
        inf.datas[i] = model.checkpoints[i];
    {
        std::unique_lock<std::mutex> lk(_npuInferenceLock);
        _npuInferenceAcc[tId] = inf;
    }

    // Write model params
    ret = _core->Write(model.rmap);
    DXRT_ASSERT(ret == 0, "failed to write model rmap parameters" + std::to_string(ret));
    ret = _core->Write(model.weight);
    DXRT_ASSERT(ret == 0, "failed to write model weight parameters" + std::to_string(ret));

    // v8 PPCPU: Write PPU binary if exists
    if (task->_isPPCPU && task->_data && task->_data->size() >= 3) {
        const auto& ppuBinary = (*task->_data)[2];  // index 2 is PPU binary
        if (!ppuBinary.empty()) {
            // Allocate PPU binary region right after weight
            dxrt_meminfo_t ppuMem;
            ppuMem.base = model.rmap.base;
            ppuMem.offset = _serviceLayer->BackwardAllocateForTask(id(), tId, ppuBinary.size());
            ppuMem.size = ppuBinary.size();
            ppuMem.data = reinterpret_cast<uint64_t>(ppuBinary.data());

            ret = _core->Write(ppuMem);
            DXRT_ASSERT(ret == 0, "failed to write PPU binary parameters" + std::to_string(ret));

            // Store PPU binary offset in task data for later use in inference request
            task->_ppuBinaryOffset = ppuMem.offset;

            LOG_DXRT_DBG << "Device " << id() << " wrote PPU binary: offset=0x" << std::hex << ppuMem.offset
                         << ", size=" << std::dec << ppuMem.size << " bytes" << std::endl;
        }
    }

    // Verify (skip if size is 0)
    {
        if (model.rmap.size > 0 && model.weight.size > 0) {
            std::vector<std::vector<uint8_t>> readData;
            readData.emplace_back(std::vector<uint8_t>(model.rmap.size));
            readData.emplace_back(std::vector<uint8_t>(model.weight.size));
            dxrt_meminfo_t cmd(model.rmap);
            dxrt_meminfo_t weight(model.weight);
            cmd.data = reinterpret_cast<uint64_t>(readData[0].data());
            weight.data = reinterpret_cast<uint64_t>(readData[1].data());
            if (cmd.size > 0 && _core->Read(cmd) == 0) {
                ret += memcmp(reinterpret_cast<void*>(model.rmap.data), readData[0].data(), cmd.size);
            }
            if (weight.size > 0 && _core->Read(weight) == 0) {
                ret += memcmp(reinterpret_cast<void*>(model.weight.data), readData[1].data(), weight.size);
            }
            DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters" + std::to_string(ret));
        } else {
            LOG_DXRT_DBG << "Device " << id() << " skipping verify (rmap.size=" << model.rmap.size
                         << ", weight.size=" << model.weight.size << ")" << std::endl;
        }
    }

    _inputTensorFormats[tId] = task->inputs(reinterpret_cast<void*>(inf.input.data));
    _outputTensorFormats[tId] = task->outputs(reinterpret_cast<void*>(inf.output.data));


    // ACC cache registration similar to Device
    const int64_t block_size = data_align(task->encoded_input_size(), 64)
                           + static_cast<int64_t>(task->_outputMemSize);

   //int npu_cache_count = DXRT_TASK_MAX_LOAD;
   int npu_cache_count = task->get_buffer_count();
    while (npu_cache_count > 0)
    {
        if (_npuMemoryCacheManager.registerMemoryCache(task->id(), block_size, npu_cache_count) == false)
        {
            npu_cache_count--;
        }
        else
        {
            break;
        }
    }
    if (npu_cache_count < 1)
    {
        LOG_DXRT_ERR("Failed to register memory cache for task " + std::to_string(task->id()));
        ret = -1;
    }
    return ret;
}

int AccDeviceTaskLayer::Release(TaskData* task)
{
    UniqueLock lock(_taskDataLock);
    int taskId = task->id();


    dxrt_request_acc_t npu_inference_acc;
    {
        std::unique_lock<std::mutex> lock(_npuInferenceLock);
        npu_inference_acc = _npuInferenceAcc[taskId];
        _npuInferenceAcc.erase(taskId);
        _npuModel.erase(taskId);
    }

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.unRegisterMemoryCache(taskId);
    }
    _serviceLayer->DeAllocate(id(), npu_inference_acc.cmd_offset);
    _serviceLayer->DeAllocate(id(), npu_inference_acc.weight_offset);

    return 0;
}


int AccDeviceTaskLayer::InferenceRequest(RequestData *req, npu_bound_op boundOp)
{
    return InferenceRequestACC(req, boundOp);
}

int AccDeviceTaskLayer::InferenceRequestACC(RequestData* req, npu_bound_op boundOp)
{
    LOG_DXRT_DBG << "Device " << id() << " inference request" << std::endl;
    int ret = 0;
    auto task = req->taskData;
    int taskId = task->id();

    void* reqInputPtr = nullptr;
    if (req->inputs.size() > 0)
        reqInputPtr = req->encoded_inputs_ptr;

    {
        SharedLock lock(_taskDataLock);
        /* accelerator device: runtime allocation */
        dxrt_request_acc_t npu_inference_acc;
        {
            std::unique_lock<std::mutex> lock(_npuInferenceLock);
            npu_inference_acc = _npuInferenceAcc[taskId];
        }
        auto model = task->_npuModel;

        npu_inference_acc.req_id = req->requestId;
        if (reqInputPtr == nullptr)
        {
            LOG_DXRT_ERR("Device::InferenceRequest_ACC - reqInputPtr is nullptr");
            reqInputPtr = reinterpret_cast<void*>(npu_inference_acc.input.data);
        }
        else
        {
            npu_inference_acc.input.data = reinterpret_cast<uint64_t>(reqInputPtr);
        }

        npu_inference_acc.input.offset = AllocateFromCache(
            data_align(task->_encodedInputSize, 64) + task->_outputMemSize, taskId);
        if (Configuration::_sNpuValidateOpt.load())
        {
            _load++;
        }
        npu_inference_acc.output.data = reinterpret_cast<uint64_t>(req->encoded_outputs_ptr);  // device buffer -> task buffer

        auto outputOffset = npu_inference_acc.input.offset;
        if (model.output_all_offset == 0)
            outputOffset += data_align(task->_encodedInputSize, 64);
        else outputOffset += model.output_all_offset;

        npu_inference_acc.output.offset = outputOffset + model.last_output_offset;
        // Set custom_offset to PPU binary offset for firmware to execute PPU
        if (task->_isPPCPU) {
            npu_inference_acc.custom_offset = task->_ppuBinaryOffset;
            LOG_DXRT_DBG << "Device " << id() << " PPCPU inference: custom_offset=0x" << std::hex
                         << task->_ppuBinaryOffset << std::dec << std::endl;
        } else {
            npu_inference_acc.custom_offset = 0;
        }
        npu_inference_acc.proc_id = getpid();
        npu_inference_acc.bound = boundOp;
        {
            ObjectsPool::GetInstance().GetRequestById(req->requestId)->setOutputs(
                task->outputs(reinterpret_cast<void*>(npu_inference_acc.output.data)));
        }
        req->outputs = task->outputs(reinterpret_cast<void*>(req->output_buffer_base));
        {
            std::unique_lock<std::mutex> lock(_npuInferenceLock);
            _ongoingRequests[req->requestId] = npu_inference_acc;
            if (Configuration::_sNpuValidateOpt.load())
            {
                Request::GetById(req->requestId)->setNpuInferenceAcc(npu_inference_acc);
                auto memInfo = dxrt_meminfo_t(npu_inference_acc.output);
                LOG_DXRT_DBG << "    data: 0x" << std::hex << memInfo.data << std::endl;
                LOG_DXRT_DBG << "    base: 0x" << std::hex << memInfo.base << std::endl;
                LOG_DXRT_DBG << "    offset: 0x" << std::hex << memInfo.offset << std::endl;
                LOG_DXRT_DBG << "    size: " << std::dec << memInfo.size << " bytes" << std::endl;
            }
        }
        LOG_DXRT_DBG << "Device " << id() << " Request : " << npu_inference_acc << "Bound:" << boundOp << std::endl;

        _inputHandlerQueue.PushWork(req->requestId);

        LOG_DXRT_DBG << "request to input worker returned " << ret << std::endl;
    }
    return 0;
}

dxrt_request_acc_t AccDeviceTaskLayer::peekInference(int id)
{
    std::unique_lock<std::mutex> lock(_npuInferenceLock);
    return _ongoingRequests[id];
}

int AccDeviceTaskLayer::InputHandler(const int& requestId, int ch)
{
    auto& profiler = Profiler::GetInstance();
    dxrt_request_acc_t inferenceAcc = peekInference(requestId);
    int channel = ch;

    inferenceAcc.dma_ch = channel;
    RequestPtr req = Request::GetById(requestId);
    if (SKIP_INFERENCE_IO != 1)
    {
        TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" write input, load: "+std::to_string(load));
#ifdef USE_PROFILER
        profiler.Start("PCIe Write[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(inferenceAcc.dma_ch)+")");
#endif
        int ret = core()->Write(inferenceAcc.input);
        if (ret < 0)
        {
            //LOG_DXRT_DBG << inferenceAcc.input << std::endl;
            //LOG_DXRT_DBG << "write failed: " << ret << std::endl;
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::WRITE_INPUT,
                LogMessages::RuntimeDispatch_FailToWriteInput(ret, requestId, ch)
            );
        }
#ifdef USE_PROFILER
        profiler.End("PCIe Write[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(inferenceAcc.dma_ch)+")");
#endif
    }

    if (dxrt::DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_encoder_input.bin", req->inputs());
        DataDumpBin(req->taskData()->name() + "_input.bin", req->encoded_inputs_ptr(), req->taskData()->encoded_input_size());
    }
    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" signal to service input");

    _serviceLayer->HandleInferenceAcc(inferenceAcc, id());
    return 0;
}

int AccDeviceTaskLayer::OutputHandler(const dxrt_response_t& response, int ch)
{
    if (response.proc_id == 0)
    {
        return 0;
    }
    if (response.proc_id != static_cast<uint32_t>(getpid()))
    {
        LOG_DXRT << "response from other process reqid: " << response.req_id
            << ", pid:" << response.proc_id << std::endl;
        return 0;
    }
    uint32_t reqId = response.req_id;
    dxrt_request_acc_t request_acc = peekInference(reqId);
    auto req = Request::GetById(reqId);
    if (req == nullptr)
    {
        DXRT_ASSERT(false, "req is nullptr "+std::to_string(reqId));
    }

    req->set_processed_unit("NPU_"+std::to_string(core()->id()), id(), response.dma_ch);
    dxrt_meminfo_t output = request_acc.output;
    if (SKIP_INFERENCE_IO != 1 || req->model_type() != 1)
    {
#ifdef USE_PROFILER
        auto& profiler = Profiler::GetInstance();

        // Record OutputHandler entry time (Framework Response Handling Delay)
        uint64_t output_handler_entry_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            ProfilerClock::now().time_since_epoch()).count();

        // Get response receive timestamp from OutputReceiverThread (before queueing)
        uint64_t response_recv_ns = 0;
        {
            std::lock_guard<std::mutex> lock(_responseTimestampLock);
            auto it = _responseReceiveTimestamps.find(reqId);
            if (it != _responseReceiveTimestamps.end()) {
                response_recv_ns = it->second;
                _responseReceiveTimestamps.erase(it);  // Cleanup after use
            }
        }

        // Measure Framework Response Handling Delay
        if (response_recv_ns > 0) {
            auto queue_delay_tp = std::make_shared<TimePoint>();
            queue_delay_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            queue_delay_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(output_handler_entry_ns));
            profiler.AddTimePoint("Framework Response Handling Delay[Job_" + std::to_string(req->job_id()) + "][" +
                req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch),
                queue_delay_tp);
        }

        // Calculate accurate NPU Core execution time using firmware timestamps
        if (response.wait_start_time > 0 && response.wait_end_time > response.wait_start_time) {
            uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;
            uint64_t wait_window = response.wait_end_time - response.wait_start_time;

            if (wait_window - inf_time_ns > 1000000000ULL) {
                uint64_t npu_start_ns = response.wait_end_time - inf_time_ns;
                uint64_t npu_end_ns   = response.wait_end_time;
                auto npu_tp = std::make_shared<TimePoint>();
                npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(npu_start_ns));
                npu_tp->end   = ProfilerClock::time_point(std::chrono::nanoseconds(npu_end_ns));
                profiler.AddTimePoint("NPU Core[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), npu_tp);
            } else {
                uint64_t center_ns = (response.wait_start_time + response.wait_end_time) / 2;
                uint64_t npu_start_ns = center_ns - (inf_time_ns / 2);
                uint64_t npu_end_ns   = center_ns + (inf_time_ns / 2);
                auto npu_tp = std::make_shared<TimePoint>();
                npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(npu_start_ns));
                npu_tp->end   = ProfilerClock::time_point(std::chrono::nanoseconds(npu_end_ns));
                profiler.AddTimePoint("NPU Core[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), npu_tp);
            }
        } else if (response_recv_ns > 0) { //case when service is off
            // Fallback: use response receive time to calculate NPU core time
            // This is more accurate than using OutputHandler entry time
            uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;
            auto npu_tp = std::make_shared<TimePoint>();
            npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns - inf_time_ns));
            profiler.AddTimePoint("NPU Core[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), npu_tp);
        }

        if (response.wait_timestamp > 0) {
            auto wait_tp = std::make_shared<TimePoint>();
            wait_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_start_time));
            wait_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_end_time));
            profiler.AddTimePoint("Service Process Wait[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), wait_tp);
        }

        profiler.Start("PCIe Read[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(ch)+")");
        // profiler.Start("PCIe Read(" + std::to_string(response.dma_ch)+")");

#endif
        int read_ch = ch;
        int ret2 = 0;
        bool ctrlCmd = true;
#if DXRT_USB_NETWORK_DRIVER
        ctrlCmd = false;
#endif

        // PPCPU (type=3) processes filtered output with dynamic shape
        if (req->model_type() != 3)
        {
            // memset(reinterpret_cast<void *>(output.data), 0, output.size);
            ret2 = core()->Read(output, read_ch, ctrlCmd);
        }
        else
        {
            LOG_DXRT_DBG << "PPCPU output processing, ppu_filter_num : " << response.ppu_filter_num << std::endl;
            RequestData* req_data = req->getData();

            if (!req_data->outputs.empty() && response.ppu_filter_num > 0)
            {
                // Validate ppu_filter_num against reasonable limits
                DataType dtype = req_data->outputs[0].type();
                size_t unit_size = GetDataSize_Datatype(dtype);
                size_t expected_max_boxes = req_data->taskData->output_size() / unit_size;

                uint32_t validated_filter_num = response.ppu_filter_num;

                if (response.ppu_filter_num > expected_max_boxes) {
                    LOG_DXRT_ERR("PPCPU: Invalid ppu_filter_num=" << response.ppu_filter_num
                                 << " exceeds maximum boxes=" << expected_max_boxes
                                 << " (dtype=" << static_cast<int>(dtype)
                                 << ", unit_size=" << unit_size << ")");
                    // Clamp to maximum to prevent buffer overflow
                    validated_filter_num = static_cast<uint32_t>(expected_max_boxes);
                }

                // Configure memory info for PPCPU filtered output
                dxrt_meminfo_t ppcpu_output = SetMemInfo_PPCPU(
                    output,
                    validated_filter_num,
                    dtype,
                    req_data->encoded_output_ptrs[0]  // Use output_buffer_base instead of encoded_output_ptrs
                );

                LOG_DXRT_DBG << "PPCPU Read - offset: 0x" << std::hex << ppcpu_output.offset
                             << ", size: " << std::dec << ppcpu_output.size
                             << " (ppu_filter_num: " << validated_filter_num << ")" << std::endl;

                // Read PPCPU filtered output from device memory
                ret2 = core()->Read(ppcpu_output, read_ch, ctrlCmd);
            }
        }

#ifdef DXRT_USE_DEVICE_VALIDATION
        if (req->is_validate_request())
        {
            ReadValidationOutput(req);
        }
#endif


#ifdef USE_PROFILER
        profiler.End("PCIe Read[Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(ch)+")");
        // profiler.End("PCIe Read(" + std::to_string(response.dma_ch)+")");
#endif
        //DXRT_ASSERT(ret2 == 0, "Failed to read output, errno="+ std::to_string(ret2) +
        //    ", reqId=" + std::to_string(reqId) + ",ch:" + std::to_string(id()));
        if ( ret2 != 0 )
        {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::READ_OUTPUT,
                LogMessages::RuntimeDispatch_FailToReadOutput(ret2, reqId, id())
            );
        }
    }
    CallBack();

    if (DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_output.bin",
            req->encoded_outputs_ptr(), req->taskData()->encoded_output_size());
    }

    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+std::to_string(_device->load()));

    Deallocate_npuBuf(request_acc.input.offset, req->taskData()->id());

    dxrt_response_t resp2 = response;
    _processResponseHandler(id(),req->id(), &resp2);
   // RequestResponse::ProcessByData(req->id(), resp2, id());

    {
        std::unique_lock<std::mutex> lock(_npuInferenceLock);
        _ongoingRequests.erase(req->id());
    }
    return 0;
}

void AccDeviceTaskLayer::OutputReceiverThread(int id)
{
    dxrt_response_t response;
    int ret;
    int deviceId = core()->id();
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
    std::shared_ptr<TimePoint> tp = nullptr;
    std::ignore = tp;
    LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Entry" << std::endl;

    int termination_count = 0;
    static constexpr int DXRT_DEVICE_TERMINATE_CONFIRM_COUNT = 5;

    while (_stop.load(std::memory_order_acquire) == false)
    {
        memset(static_cast<void*>(&response), 0x00, sizeof(dxrt_response_t));
        response.req_id = static_cast<uint32_t>(id);
        if (_stop.load(std::memory_order_acquire))
            break;
        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Waiting for response..." << std::endl;
#if DXRT_USB_NETWORK_DRIVER
        ret = core()->Process(cmd, &response, sizeof(response));
#else
        ret = core()->Process(cmd, &response);
#endif
        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Response : " << response << std::endl;
        if (ret == -1)
        {
            LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Terminate detected." << std::endl;
            termination_count++;
            if (termination_count >= DXRT_DEVICE_TERMINATE_CONFIRM_COUNT)
                break;
            else
                continue;
        }
        if (ret != 0)
        {
            std::cout << "ERROR RET: " << ret << std::endl;
            continue;
        }
        if (response.status != 0)
        {
            LOG_VALUE(response.status);
            std::string _dumpFile = "dxrt.dump.bin." + std::to_string(core()->id());
            LOG_DXRT << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << std::endl;
            LOG_DXRT << "    Device " << deviceId << " dump to file " << _dumpFile << std::endl;
            std::vector<uint32_t> dump(1000, 0);
            core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
            for (size_t i = 0; i < dump.size(); i+=2)
            {
                if (dump[i] == 0xFFFFFFFF) break;
            }
            DataDumpBin(_dumpFile, dump.data(), dump.size());
            DataDumpTxt(_dumpFile+".txt", dump.data(), 1, dump.size()/2, 2, true);
            _stop.store(true);
            DXRT_ASSERT(false, "");
        }
        if (_stop.load(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << core()->name() << " : requested to stop thread." << std::endl;
            break;
        }
#ifdef USE_PROFILER
        // Record timestamp when response is received from driver (before queueing)
        {
            std::lock_guard<std::mutex> lock(_responseTimestampLock);
            _responseReceiveTimestamps[response.req_id] =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    ProfilerClock::now().time_since_epoch()).count();
        }
#endif
        _outputHandlerQueue.PushWork(response);
    }

    LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": End" << std::endl;
    _outputDispatcherTerminateFlag[id].store(true, std::memory_order_release);
}

void AccDeviceTaskLayer::EventThread()
{
    _eventThreadStartFlag.store(true, std::memory_order_release);
    std::string threadName = core()->name();
    int loopCnt = 0;
    LOG_DXRT_DBG << threadName << " : Entry" << std::endl;
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
    while (_stop.load(std::memory_order_acquire) == false)
    {
        if (_stop.load(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << std::endl;
            break;
        }
        dxrt::dx_pcie_dev_event_t eventInfo;
        memset(&eventInfo, 0, sizeof(dxrt::dx_pcie_dev_event_t));
        core()->Process(cmd, &eventInfo); //Waiting in kernel. (device::terminate())
        if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_ERROR)
        {
            if (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code)!=dxrt::dxrt_error_t::ERR_NONE)
            {
                std::string err_code_str;
                switch (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code)) {
                    case dxrt::dxrt_error_t::ERR_NPU0_HANG: err_code_str = "NPU0_HANG"; break;
                    case dxrt::dxrt_error_t::ERR_NPU1_HANG: err_code_str = "NPU1_HANG"; break;
                    case dxrt::dxrt_error_t::ERR_NPU2_HANG: err_code_str = "NPU2_HANG"; break;
                    case dxrt::dxrt_error_t::ERR_NPU_BUS: err_code_str = "NPU_BUS"; break;
                    case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_FAIL: err_code_str = "PCIE_DMA_CH0_FAIL"; break;
                    case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH1_FAIL: err_code_str = "PCIE_DMA_CH1_FAIL"; break;
                    case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH2_FAIL: err_code_str = "PCIE_DMA_CH2_FAIL"; break;
                    case dxrt::dxrt_error_t::ERR_LPDDR_DED_WR: err_code_str = "LPDDR_DED_WR"; break;
                    case dxrt::dxrt_error_t::ERR_LPDDR_DED_RD: err_code_str = "LPDDR_DED_RD"; break;
                    case dxrt::dxrt_error_t::ERR_DEVICE_ERR: err_code_str = "DEVICE_ERR"; break;
                    default: err_code_str = "UNKNOWN"; break;
                }

                LOG_DXRT_ERR(eventInfo.dx_rt_err);
                core()->ShowPCIEDetails();
                RuntimeEventDispatcher::GetInstance().DispatchEvent(
                    RuntimeEventDispatcher::LEVEL::ERROR,
                    RuntimeEventDispatcher::TYPE::DEVICE_IO,
                    RuntimeEventDispatcher::CODE::DEVICE_EVENT,
                    LogMessages::RuntimeDispatch_DeviceEventError(id(), err_code_str));
                DXRT_ASSERT(false, LogMessages::Device_DeviceErrorEvent(static_cast<int>(eventInfo.dx_rt_err.err_code)));
                break;
            }
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_NOTIFY_THROT)
        {
            if ( Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_THROTTLING) )
                LOG_DXRT << eventInfo.dx_rt_ntfy_throt << std::endl;

            if ( eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_DOWN
                || eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_UP
                || eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_DOWN
                || eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_UP ) {

                std::string throt_code_str;
                switch (eventInfo.dx_rt_ntfy_throt.ntfy_code) {
                    case dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_DOWN:
                        throt_code_str = "FREQ_DOWN(MHz) "
                            + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_freq[0])
                            + " to " + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_freq[1]);
                        break;
                    case dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_UP: throt_code_str = "FREQ_UP(MHz) "
                            + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_freq[0])
                            + " to " + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_freq[1]);
                        break;
                    case dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_DOWN: throt_code_str = "VOLT_DOWN(mV) "
                            + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_voltage[0])
                            + " to " + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_voltage[1]);
                        break;
                    case dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_UP: throt_code_str = "VOLT_UP(mV) "
                            + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_voltage[0])
                            + " to " + std::to_string(eventInfo.dx_rt_ntfy_throt.throt_voltage[1]);
                        break;
                    default: throt_code_str = "UNKNOWN"; break;
                }

                auto level = RuntimeEventDispatcher::LEVEL::INFO;
                if ( eventInfo.dx_rt_ntfy_throt.throt_temper >= 95)
                {
                    level = RuntimeEventDispatcher::LEVEL::WARNING;
                }

                RuntimeEventDispatcher::GetInstance().DispatchEvent(
                    level,
                    RuntimeEventDispatcher::TYPE::DEVICE_STATUS,
                    RuntimeEventDispatcher::CODE::THROTTLING_NOTICE,
                    LogMessages::RuntimeDispatch_ThrottlingNotice(
                        id(),
                        eventInfo.dx_rt_ntfy_throt.npu_id,
                        throt_code_str,
                        eventInfo.dx_rt_ntfy_throt.throt_temper)
                );
            }
            else if ( eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_BLOCK
                || eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_RELEASE
                || eventInfo.dx_rt_ntfy_throt.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_WARN )
            {

                std::string emergency_code_str;
                switch (eventInfo.dx_rt_ntfy_throt.ntfy_code) {
                    case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_BLOCK: emergency_code_str = "EMERGENCY_BLOCK"; break;
                    case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_RELEASE: emergency_code_str = "EMERGENCY_RELEASE"; break;
                    case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_WARN: emergency_code_str = "EMERGENCY_WARN"; break;
                    default: emergency_code_str = "UNKNOWN"; break;
                }

                RuntimeEventDispatcher::GetInstance().DispatchEvent(
                    RuntimeEventDispatcher::LEVEL::CRITICAL,
                    RuntimeEventDispatcher::TYPE::DEVICE_STATUS,
                    RuntimeEventDispatcher::CODE::THROTTLING_EMERGENCY,
                    LogMessages::RuntimeDispatch_ThrottlingEmergency(
                        id(),
                        eventInfo.dx_rt_ntfy_throt.npu_id,
                        emergency_code_str)
                );
            }
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_RECOVERY)
        {
            std::string type = "Unknown";
            if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_RMAP)
            {
                auto model = _npuModel.begin()->second;
                DXRT_ASSERT(core()->Write(model.rmap, 3) == 0, "Recovery rmap failed to write model parameters(cmd)");
                LOG_DXRT_ERR("RMAP data has been recovered. This error can cause issues with NPU operation.")
                StartDev(RMAP_RECOVERY_DONE);
                type = "RMAP";
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_WEIGHT)
            {
                auto model = _npuModel.begin()->second;
                DXRT_ASSERT(core()->Write(model.weight, 3) == 0, "Recovery weight failed to write model parameters(weight)");
                LOG_DXRT_ERR("Weight data has been recovered. This error can cause wrong result value.")
                StartDev(WEIGHT_RECOVERY_DONE);
                type = "WEIGHT";
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_CPU)
            {
                LOG_DXRT << "Host received a message regarding a CPU abnormal case." << std::endl;
                type = "CPU";
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_DONE)
            {
                LOG_DXRT << "Device recovery is complete" << std::endl;
                type = "DONE";
            }
            else
            {
                LOG_DXRT_ERR("Unknown data is received from device " << std::hex << eventInfo.dx_rt_recv.action << "\n");
                core()->ShowPCIEDetails();
            }

            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::WARNING,
                RuntimeEventDispatcher::TYPE::DEVICE_CORE,
                RuntimeEventDispatcher::CODE::RECOVERY_OCCURRED,
                LogMessages::RuntimeDispatch_DeviceRecovery(id(), type)
            );
        }
        else
        {
            LOG_DXRT_DBG << "!! unknown event occured from device "<< eventInfo.event_type << std::endl;
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, LoopCount" << loopCnt << std::endl;
    _eventThreadTerminateFlag.store(true);
}

void AccDeviceTaskLayer::StartThread()
{
    core()->CheckVersion();

    // Initialize atomic flags with release semantics to prevent data races with spawned threads
    _eventThreadTerminateFlag.store(false, std::memory_order_release);

    _eventThread = std::thread(&AccDeviceTaskLayer::EventThread, this);
    if (_serviceLayer->isRunOnService() == false)
    {
        for (uint32_t i = 0; i < core()->info().num_dma_ch; i++)
        {
            _outputDispatcher.emplace_back(&AccDeviceTaskLayer::OutputReceiverThread, this, i);
            _outputDispatcherTerminateFlag[i].store(false, std::memory_order_release);
        }
        //Load PPCPU firmware if not running on service layer
        size_t fw_size = PPCPUDataLoader::GetDataSize();
        uint64_t mem_offset = _serviceLayer->Allocate(id(), static_cast<uint64_t>(fw_size));

        dxrt_meminfo_t fw_meminfo;
        fw_meminfo.base = core()->info().mem_addr;
        fw_meminfo.offset = mem_offset;
        fw_meminfo.size = static_cast<uint32_t>(fw_size);
        fw_meminfo.data = reinterpret_cast<uint64_t>(PPCPUDataLoader::GetData());

        int ret1 = core()->Write(fw_meminfo);
        DXRT_ASSERT(ret1 == 0, "Failed to load PPCPU firmware to device: ret=" + std::to_string(ret1));
        LOG_DXRT_DBG << "PPCPU firmware loaded to device " << id() << " , size: " << fw_size << " bytes" << std::endl;

        dxrt_req_meminfo_t meminfo_req;
        meminfo_req.base = fw_meminfo.base;
        meminfo_req.offset = fw_meminfo.offset;
        meminfo_req.size = fw_meminfo.size;
        meminfo_req.data = fw_meminfo.data;
        meminfo_req.ch = 0;

        core()->DoCustomCommand(&meminfo_req, dxrt::dxrt_custom_sub_cmt_t::DX_INIT_PPCPU,sizeof(dxrt_req_meminfo_t));

    }
    else
    {
        LOG_DXRT_DBG << "Service layer is running. Skipping PPCPU firmware load." << std::endl;
    }
    _inputHandlerQueue.Start();
    _outputHandlerQueue.Start();
}

AccDeviceTaskLayer::~AccDeviceTaskLayer()
{
    _stop.store(true);
    _inputHandlerQueue.Stop();
    _outputHandlerQueue.Stop();
    if (_eventThreadStartFlag.load(std::memory_order_acquire))
    {
#if __linux__
        while (_eventThreadTerminateFlag.load(std::memory_order_acquire) == false)
        {
            Terminate();
            if (_eventThreadTerminateFlag.load(std::memory_order_acquire) == true)
                break;
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
#else
        Terminate();
#endif
        _eventThread.join();
    }
    Terminate();
    size_t outputDispatcher_size = _outputDispatcher.size();
    for (size_t i = 0; i < outputDispatcher_size; i++)
    {
        dxrt_response_t data;
        while(_outputDispatcherTerminateFlag[i] == false)
        {
            data.req_id = i;
            int ret = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, &data);
            LOG_DXRT_DBG << "Terminate output dispatcher " << i << " returned " << ret << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        _outputDispatcher[i].join();
    }
    _outputDispatcher.clear();
}


void AccDeviceTaskLayer::ProcessResponseFromService(const dxrt::_dxrt_response_t& response)
{
#ifdef USE_PROFILER
        // Record timestamp when response is received from driver (before queueing)
        {
            std::lock_guard<std::mutex> lock(_responseTimestampLock);
            _responseReceiveTimestamps[response.req_id] =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    ProfilerClock::now().time_since_epoch()).count();
        }
#endif
    _outputHandlerQueue.PushWork(response);
}
#ifdef DXRT_USE_DEVICE_VALIDATION
void AccDeviceTaskLayer::ReadValidationOutput(std::shared_ptr<Request> req)
{
    auto task = req->task();
    auto inferenceAcc = peekInference(req->id());
    auto model = _npuModel[task->id()];
    auto memInfo = dxrt_meminfo_t(inferenceAcc.output);

    // Get validation tensor once to avoid multiple calls
    Tensor validateTensor = req->ValidateOutputTensor();
    void *ptr = validateTensor.data();

    LOG_DXRT_DBG << "  Model Info:" << std::endl;
    LOG_DXRT_DBG << "    model.output_all_size: " << std::dec << model.output_all_size << " bytes" << std::endl;
    LOG_DXRT_DBG << "    model.last_output_offset: 0x" << std::hex << model.last_output_offset << std::endl;
    LOG_DXRT_DBG << "    memInfo.offset: 0x" << std::hex << memInfo.offset << std::endl;
    LOG_DXRT_DBG << "  Validation Tensor: " << validateTensor << std::endl;

    memInfo.data = reinterpret_cast<uint64_t>(ptr);
    memInfo.offset -= model.last_output_offset;
    memInfo.size = model.output_all_size;

    DXRT_ASSERT(core()->Read(memInfo) == 0, "Fail to read device");
    LOG_DXRT_DBG << "  Output Memory Info:" << std::endl;
    LOG_DXRT_DBG << "    data: 0x" << std::hex << memInfo.data << std::endl;
    LOG_DXRT_DBG << "    base: 0x" << std::hex << memInfo.base << std::endl;
    LOG_DXRT_DBG << "    offset: 0x" << std::hex << memInfo.offset << std::endl;
    LOG_DXRT_DBG << "    size: " << std::dec << memInfo.size << " bytes" << std::endl;

    LOG_DXRT_DBG << "  Encoded Input Size: " << req->taskData()->encoded_input_size() << " bytes" << std::endl;
    LOG_DXRT_DBG << "  Encoded Output Size: " << req->taskData()->encoded_output_size() << " bytes" << std::endl;
    LOG_DXRT_DBG << "  Validate Buffer size: " <<  /*_outputValidateBuffers[task->id()].size() << */ " bytes" << std::endl;

    if (memInfo.size == 0) memInfo = inferenceAcc.output;  // temporary solution for zero size argmax model

    if (core()->Read(memInfo) != 0) {
        LOG_DXRT_DBG << "Validate output is empty." << std::endl;
    }
    //req->setOutputs(ret);
}
#endif

dxrt_meminfo_t AccDeviceTaskLayer::SetMemInfo_PPCPU(const dxrt_meminfo_t& rmap_output,
                                                      size_t ppu_filter_num,
                                                      DataType dtype,
                                                      void* output_ptr)
{
    // Calculate unit size from data type
    size_t unit_size = GetDataSize_Datatype(dtype);
    // Calculate PPCPU output size
    size_t ppcpu_output_size = unit_size * ppu_filter_num;
    // Configure memory info for PPCPU filtered output
    // The filtered output comes after the RMAP output in memory
    dxrt_meminfo_t ppcpu_output;
    ppcpu_output.base = rmap_output.base;
    ppcpu_output.offset = rmap_output.offset + rmap_output.size;  // After RMAP output
    ppcpu_output.size = static_cast<uint32_t>(ppcpu_output_size);
    ppcpu_output.data = reinterpret_cast<uint64_t>(output_ptr);

    return ppcpu_output;
}

} // namespace dxrt
