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
#include <algorithm>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include "dxrt/common.h"
#include "dxrt/device_task_layer.h"
#include "inference_context.h"
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
#include "dxrt/safe_cast.h"

#include <memory>
#ifdef DXRT_USE_DEVICE_VALIDATION
#include "dxrt/task.h"
#endif

#ifdef USE_VNPU
    #include "rk_mpi_mb.h"
    #include "rk_mpi_sys.h"
    #include "rk_mpi_mmz.h"
    #include "dxrt/fixed_size_buffer.h"
#endif  // USE_VNPU

#include "../data/ppcpu.h"

// Macros duplicated from original implementation unit (can be refactored later)
#define RMAP_RECOVERY_DONE      (1)
#define WEIGHT_RECOVERY_DONE    (2)

namespace dxrt {

constexpr int THROTTLING_WARNING_TEMPERATURE = 95;
constexpr int NO_TASK_ID = -1;

namespace {

// ---------------------------------------------------------------------------
// Platform I/O address and cache-coherency helpers (VNPU vs PCIe).
// These centralise every #ifdef USE_VNPU in the hot inference path.
// ---------------------------------------------------------------------------

// note: static not required for internal linkage due to anonymous namespace, but added for clarity

uint64_t resolveInputHostAddr(RequestData* req, void* virtualPtr, int devId)
{
#ifndef USE_VNPU
    (void)req; (void)devId;
    return SafeCast::PointerToInteger<void*>(virtualPtr);
#else
    (void)virtualPtr;
    if (req->encoded_inputs_phy != 0)
    {
        LOG_DXRT_DBG << "Device " << devId << " Using CMA input physical address: 0x"
                     << std::hex << req->encoded_inputs_phy << std::dec << std::endl;
        return req->encoded_inputs_phy;
    }
    LOG_DXRT_ERR("Device " + std::to_string(devId)
        + " Error: input physical address is zero, falling back to virtual address");
    return 0;
#endif
}

uint64_t resolveOutputHostAddr(RequestData* req, int devId)
{
#ifndef USE_VNPU
    (void)devId;
    return SafeCast::PointerToInteger<void*>(req->encoded_outputs_ptr);
#else
    if (req->encoded_outputs_phy != 0)
    {
        LOG_DXRT_DBG << "Device " << devId << " Using CMA output physical address: 0x"
                     << std::hex << req->encoded_outputs_phy << std::dec << std::endl;
        return req->encoded_outputs_phy;
    }
    LOG_DXRT_ERR("Device " + std::to_string(devId)
        + " Error: output physical address is zero, falling back to virtual address");
    return 0;
#endif
}

void flushInputBeforeDma(RequestPtr req, uint32_t size, int reqId, int devId)
{
#ifdef USE_VNPU
    if (req->encoded_inputs_ptr() != nullptr && size > 0)
    {
        RK_MPI_MMZ_FlushCacheVaddrEnd(req->encoded_inputs_ptr(), size, RK_MMZ_SYNC_RW);
        LOG_DXRT_DBG << "Device " << devId << " Flushed input cache before DMA Write for request "
                     << reqId << ", ptr=" << req->encoded_inputs_ptr()
                     << ", size=" << size << std::endl;
    }
#else
    (void)req; (void)size; (void)reqId; (void)devId;
#endif
}

[[maybe_unused]]
void invalidateOutputAfterDma(RequestPtr req, uint32_t size, int ret)
{
#ifdef USE_VNPU
    if (ret == 0 && req->encoded_outputs_ptr() != nullptr)
    {
        RK_MPI_MMZ_FlushCacheVaddrStart(req->encoded_outputs_ptr(), size, RK_MMZ_SYNC_RW);
    }
#else
    (void)req; (void)size; (void)ret;
#endif
}

void* getPpcpuOutputPtr(RequestData* reqData, uint64_t outputData)
{
#ifndef USE_VNPU
    (void)outputData;
    return reqData->encoded_output_ptrs[0];
#else
    (void)reqData;
    return SafeCast::IntegerToPointer<void*>(outputData);
#endif
}

void assertAndInvalidatePpcpuOutput(RequestPtr req, int reqId, uint32_t size, int ret, int devId)
{
#ifdef USE_VNPU
    DXRT_ASSERT(ret == 0,
        "Failed to read PPCPU output, errno=" + std::to_string(ret)
        + ", reqId=" + std::to_string(reqId));

    if (ret == 0 && req->encoded_outputs_ptr() != nullptr)
    {
        RK_MPI_MMZ_FlushCacheVaddrStart(req->encoded_outputs_ptr(), size, RK_MMZ_SYNC_RW);
        LOG_DXRT_DBG << "Device " << devId << " Invalidated PPCPU output cache for request "
                     << reqId << ", ptr=" << req->encoded_outputs_ptr()
                     << ", size=" << size << std::endl;
    }
#else
    (void)req; (void)reqId; (void)size; (void)ret; (void)devId;
#endif
}

}  // namespace

AccDeviceTaskLayer::AccDeviceTaskLayer(
    std::shared_ptr<DeviceCore> dev,
    std::shared_ptr<ServiceLayerInterface> service_interface)
    : DeviceTaskLayer(dev, service_interface),
      _inputHandlerQueue(dev->name() + "_input", dev->GetReadChannel(),
          std::bind(&AccDeviceTaskLayer::InputHandler, this, std::placeholders::_1, std::placeholders::_2)),
      _outputHandlerQueue(dev->name() + "_output", dev->GetWriteChannel(),
          std::bind(&AccDeviceTaskLayer::OutputHandler, this, std::placeholders::_1, std::placeholders::_2))
{
}

int AccDeviceTaskLayer::Release(TaskData* task)
{
    UniqueLock lock(_taskDataLock);
    int taskId = task->id();


    uint32_t fallback_cmd_offset = 0;
    uint32_t fallback_weight_offset = 0;
    {
        std::unique_lock<std::mutex> inference_lock(npuInferenceLock());
        auto cfgIt = taskStaticConfigs().find(taskId);
        if (cfgIt != taskStaticConfigs().end())
        {
            fallback_cmd_offset    = cfgIt->second.cmd_offset;
            fallback_weight_offset = cfgIt->second.weight_offset;
        }
        npuModelMap().erase(taskId);
        taskStaticConfigs().erase(taskId);
    }

    if (memoryCacheManager().canGetCache(taskId))
    {
        memoryCacheManager().unRegisterMemoryCache(taskId);
    }

    // Deallocate memory using stored memory info
    auto memInfosIt = modelMemoryInfos().find(taskId);
    if (memInfosIt != modelMemoryInfos().end())
    {
        const auto& memInfos = memInfosIt->second;

        // Deallocate RMAP memory
        if (memInfos.rmapMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocate(id(), memInfos.rmapMemInfo.block_id);
        }

        // Deallocate Weight memory
        if (memInfos.weightMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocate(id(), memInfos.weightMemInfo.block_id);
        }

        // Deallocate PPU memory if it exists
        if (memInfos.ppuMemInfo.size > 0 && memInfos.ppuMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocate(id(), memInfos.ppuMemInfo.block_id);
        }

        modelMemoryInfos().erase(memInfosIt);
    }
    else
    {
        // Fallback to old method for backward compatibility
        serviceLayer()->DeAllocate(id(), fallback_cmd_offset);
        serviceLayer()->DeAllocate(id(), fallback_weight_offset);
    }

    // Cleanup device-specific PPU binary storage
    _ppuBinaryData.erase(taskId);
    auto ppu_offset_it = _ppuBinaryOffsets.find(taskId);
    if (ppu_offset_it != _ppuBinaryOffsets.end())
    {
        auto ppu_offset = ppu_offset_it->second;
        _ppuBinaryOffsets.erase(ppu_offset_it);
        if (ppu_offset != 0 && ppu_offset != static_cast<uint32_t>(-1))
        {
            serviceLayer()->DeAllocate(id(), ppu_offset);
        }
    }

    return 0;
}


int AccDeviceTaskLayer::InferenceRequest(RequestData *req, npu_bound_op boundOp)
{
    auto dmaPass = _dmaStopGate.WaitIfStopped();
    int retval = InferenceRequestACC(req, boundOp);
    return retval;
}

int AccDeviceTaskLayer::InferenceRequestACC(RequestData* req, npu_bound_op boundOp)
{
    LOG_DXRT_DBG << "Device " << id() << " inference request" << std::endl;
    int ret = 0;
    auto task = req->taskData;
    int taskId = task->id();
    int reqId = req->requestId;

    void* req_input_ptr = nullptr;
    if (req->inputs.size() > 0)
    {
        req_input_ptr = req->encoded_inputs_ptr;
    }

    {
        SharedLock lock(_taskDataLock);

        // ---------------------------------------------------------------
        // Layer 3: build slim request and call BuildDriverRequest()
        // ---------------------------------------------------------------
        TaskStaticConfig cfg;
        {
            std::unique_lock<std::mutex> inference_lock(npuInferenceLock());
            auto it = taskStaticConfigs().find(taskId);
            DXRT_ASSERT(it != taskStaticConfigs().end(),
                "TaskStaticConfig missing for taskId=" + std::to_string(taskId));
            cfg = it->second;
        }

        LOG_DXRT_DBG << "Device " << id() << " InferenceRequestACC: taskId=" << taskId
                 << ", model_type=" << static_cast<int>(cfg.model_type)
                 << ", task->_isPPCPU=" << task->_isPPCPU
                 << ", custom_offset=0x" << std::hex << cfg.custom_offset << std::dec
                 << std::endl;

        // Build slim request with per-request runtime values
        InferenceSlimRequest slim{};
        slim.req_id  = req->requestId;
        slim.task_id = static_cast<uint32_t>(taskId);
        slim.bound   = static_cast<uint32_t>(boundOp);

        if (req_input_ptr == nullptr)
        {
            LOG_DXRT_ERR("Device::InferenceRequest_ACC - req_input_ptr is nullptr");
        }
        else
        {
            slim.input_host_addr = resolveInputHostAddr(req, req_input_ptr, id());
        }

        slim.output_host_addr = resolveOutputHostAddr(req, id());

        const uint64_t alignedInputBytes = data_align(task->_encodedInputSize, 64);
        const uint64_t outputDelta = (cfg.output_all_offset != 0)
            ? static_cast<uint64_t>(cfg.output_all_offset)
            : alignedInputBytes;
        const uint64_t requiredSliceBytes = outputDelta
            + static_cast<uint64_t>(cfg.last_output_offset)
            + static_cast<uint64_t>(cfg.output_size);
        const uint64_t cacheSliceBytes = (std::max)(
            alignedInputBytes + static_cast<uint64_t>(task->_outputMemSize),
            requiredSliceBytes);

        NpuMemoryCacheSlice cacheSlice;
        if (req->_ioSliceValid)
        {
            // Slice was pre-acquired in PrepareZeroCopyIo and already wired as the encoded I/O
            // host storage (zero-copy). Reuse it here; release still flows through OutputHandler.
            cacheSlice = req->_ioSlice;
            req->_ioSliceValid = false;
        }
        else
        {
            cacheSlice = AllocateFromCache(cacheSliceBytes, taskId);
        }
        DXRT_ASSERT(cacheSlice.isValid(), "Failed to allocate NPU memory cache slice");
        slim.input_device_offset = cacheSlice.deviceAddress();

        if (Configuration::_sNpuValidateOpt.load())
        {
            loadCounter()++;
        }

        // Assemble full dxrt_request_acc_t via Layer 3
        dxrt_request_acc_t npu_inference_acc = BuildDriverRequest(cfg, slim);

        if (cfg.output_all_offset == 0)
        {
            LOG_DXRT_DBG << "Device " << id()
                      << " output_all_offset is 0, output.offset=0x"
                      << std::hex << npu_inference_acc.output.offset << std::dec << std::endl;
        }
        else
        {
            LOG_DXRT_ERR(
                "Device " + std::to_string(id())
                + " using model output_all_offset, output.offset=0x"
                + ToHexString(npu_inference_acc.output.offset));
        }

        if (task->_isPPCPU)
        {
            if (npu_inference_acc.custom_offset != 0)
            {
                LOG_DXRT_DBG << "Device " << id() << " PPCPU inference: custom_offset=0x" << std::hex
                             << npu_inference_acc.custom_offset << ", model_type=" << std::dec
                             << static_cast<int>(npu_inference_acc.model_type) << std::endl;
            }
            else
            {
                LOG_DXRT_ERR(
                    "Device " + std::to_string(id()) + " PPCPU task "
                    + std::to_string(taskId) + " missing PPU offset");
            }
        }
        else
        {
            npu_inference_acc.custom_offset = 0;
        }

        {
            ObjectsPool::GetInstance().GetRequestById(reqId)->setOutputs(
                task->outputs(SafeCast::IntegerToPointer<void*>(npu_inference_acc.output.data)));
        }
        req->outputs = task->outputs(req->output_buffer_base);
        {
            std::unique_lock<std::mutex> npu_inference_lock(npuInferenceLock());
            _ongoingRequests[reqId] = npu_inference_acc;
            _ongoingInputAllocations[reqId] = OngoingCacheAllocation{cacheSlice, taskId};
            if (Configuration::_sNpuValidateOpt.load())
            {
                Request::GetById(reqId)->setNpuInferenceAcc(npu_inference_acc);
                auto memInfo = dxrt_meminfo_t(npu_inference_acc.output);
                LOG_DXRT_DBG << "    data: 0x" << std::hex << memInfo.data << std::endl;
                LOG_DXRT_DBG << "    base: 0x" << std::hex << memInfo.base << std::endl;
                LOG_DXRT_DBG << "    offset: 0x" << std::hex << memInfo.offset << std::endl;
                LOG_DXRT_DBG << "    size: " << std::dec << memInfo.size << " bytes" << std::endl;
            }
        }
        LOG_DXRT_DBG << "Device " << id() << " Request : " << npu_inference_acc << "Bound:" << boundOp << std::endl;
        LOG_DXRT_DBG << "Device " << id() << " Pushing request " << reqId
                 << " to InputHandlerQueue" << std::endl;
        _inputHandlerQueue.PushWork(reqId);

        LOG_DXRT_DBG << "request to input worker returned " << ret << std::endl;
    }
    return 0;
}

void AccDeviceTaskLayer::PrepareZeroCopyIo(RequestData* req)
{
#ifndef USE_VNPU
    // The staging memcpy only exists on the service (PCIe) path, where encoded I/O currently
    // lives in heap buffers separate from the DMA'able SHM slice. Elsewhere: keep existing path.
    if (req == nullptr || !serviceLayer()->isRunOnService())
    {
        return;
    }

    // Only normal models have a static, contiguous output region that maps 1:1 onto the slice.
    // ARGMAX/PPU/PPCPU relocate or dynamically size their output and keep the existing path.
    if (static_cast<ModelType>(req->taskData->_npuModel.type) != ModelType::MODEL_TYPE_NORMAL)
    {
        return;
    }

    const int taskId = req->taskData->id();
    if (!memoryCacheManager().canGetCache(taskId))
    {
        return;  // no pooled SHM slice for this task -> keep heap fallback
    }

    TaskStaticConfig cfg;
    {
        std::unique_lock<std::mutex> lock(npuInferenceLock());
        auto it = taskStaticConfigs().find(taskId);
        if (it == taskStaticConfigs().end())
        {
            return;
        }
        cfg = it->second;
    }

    // Host offset of the output region within the slice; must match InferenceRequestACC /
    // OutputHandler so that encoded_outputs_ptr coincides with the DMA'd output location.
    const uint64_t outputDelta = (cfg.output_all_offset != 0)
        ? static_cast<uint64_t>(cfg.output_all_offset)
        : data_align(cfg.input_size, 64);
    const uint64_t outputOffsetInSlice = outputDelta + static_cast<uint64_t>(cfg.last_output_offset);

    NpuMemoryCacheSlice slice = memoryCacheManager().getNpuMemoryCache(taskId);
    if (!slice.isValid() || slice.hostPtr() == nullptr)
    {
        if (slice.isValid())
        {
            memoryCacheManager().returnNpuMemoryCache(taskId, slice);
        }
        return;  // could not obtain a host-mapped SHM slice -> keep heap fallback
    }

    auto* host = static_cast<uint8_t*>(slice.hostPtr());
    req->encoded_inputs_ptr  = host;
    req->encoded_outputs_ptr = host + outputOffsetInSlice;
    req->_ioSlice      = slice;
    req->_ioSliceValid = true;
#else
    (void)req;
#endif  // USE_VNPU
}

dxrt_request_acc_t AccDeviceTaskLayer::peekInference(int id)
{
    std::unique_lock<std::mutex> lock(npuInferenceLock());
    return _ongoingRequests[id];
}

int AccDeviceTaskLayer::InputHandler(const int& reqId, int ch)
{
    _dmaStopGate.WaitIfStopped();
    LOG_DXRT_DBG << "Device " << id() << " InputHandler START for request " << reqId << std::endl;
    auto& profiler = Profiler::GetInstance();
    dxrt_request_acc_t inferenceAcc = peekInference(reqId);
    int channel = ch;

    const auto numResponseChannels = static_cast<int>(core()->info().num_dma_ch);
    if (numResponseChannels > 0 && (channel < 0 || channel >= numResponseChannels))
    {
        const int normalizedChannel = ((channel % numResponseChannels) + numResponseChannels) % numResponseChannels;
        LOG_DXRT_DBG << "Device " << id() << " InputHandler normalized NPU dma_ch for request "
                     << reqId << ": worker_ch=" << channel
                     << ", num_dma_ch=" << numResponseChannels
                     << ", dma_ch=" << normalizedChannel << std::endl;
        channel = normalizedChannel;
    }

    inferenceAcc.dma_ch = channel;
    RequestPtr req = Request::GetById(reqId);
#ifdef USE_PROFILER
    const std::string profileTagBase =
        "[Device_" + std::to_string(id())
        + "][Job_" + std::to_string(req->job_id())
        + "][" + req->taskData()->name()
        + "][Req_" + std::to_string(req->id()) + "]";
#endif

    // Debug: Log input DMA parameters
    LOG_DXRT_DBG << "Device " << id() << " InputHandler req=" << reqId
             << ": input.data(phy)=0x" << std::hex << inferenceAcc.input.data
             << ", input.base=0x" << inferenceAcc.input.base
             << ", input.offset=0x" << inferenceAcc.input.offset
             << ", input.size=" << std::dec << inferenceAcc.input.size << std::endl;

    if (SKIP_INFERENCE_IO != 1)
    {
        TASK_FLOW(
            "[" + std::to_string(req->job_id()) + "]"
            + req->taskData()->name() + " write input, load: " + std::to_string(load));
        flushInputBeforeDma(req, inferenceAcc.input.size, reqId, id());
#ifdef USE_PROFILER
        profiler.Start(Profiler::EventType::H2D, req->task()->name(), id(), req->job_id());
#endif

        int ret;
        if (serviceLayer()->isRunOnService())
        {
            NpuMemoryCacheSlice inputSlice;
            {
                std::unique_lock<std::mutex> npu_lock(npuInferenceLock());
                auto it = _ongoingInputAllocations.find(reqId);
                DXRT_ASSERT(it != _ongoingInputAllocations.end(),
                    "InputHandler missing cache allocation for reqId=" + std::to_string(reqId));
                inputSlice = it->second.slice;
            }
            // Zero-copy: when the encoded input already lives in this SHM slice
            // (PrepareZeroCopyIo), src == dst and the staging copy is skipped.
            if (inputSlice.view.hostPtr() != nullptr && inferenceAcc.input.size > 0
                && inputSlice.view.hostPtr() != reinterpret_cast<void*>(inferenceAcc.input.data))
            {
                std::memcpy(inputSlice.view.hostPtr(),
                            reinterpret_cast<void*>(inferenceAcc.input.data),
                            inferenceAcc.input.size);
            }
            SharedMemoryView inputView;
            inputView.info   = inputSlice.view.info;
            inputView.offset = inputSlice.view.offset;
            inputView.size   = inferenceAcc.input.size;
            ret = serviceLayer()->DMAWrite(inputView);
        }
        else
        {
            ret = serviceLayer()->DMAWrite(id(), inferenceAcc.input.data,
                inferenceAcc.input.base + inferenceAcc.input.offset, inferenceAcc.input.size);
        }
        if (ret < 0)
        {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::WRITE_INPUT,
                LogMessages::RuntimeDispatch_FailToWriteInput(ret, reqId, ch)
            );

            // Write failure means the DMA channel is in error state (e.g. CS=2 stuck).
            // Block the device to stop new requests — do NOT submit NPU_RUN for
            // an input that was never written.
            block();

            if (serviceLayer()->isRunOnService())
            {
                // In service mode, recovery is handled by the service (dxrtd).
                // The service's WaitThread will detect the error via NPU_RUN_RESP
                // and perform DXRT_CMD_RECOVERY, then broadcast to all clients.
                // Terminate this client immediately so the service can proceed.
                LOG_DXRT_ERR(
                    "DMA write failed (errno=" + std::to_string(ret)
                    + ") in service mode on device " + std::to_string(id())
                    + ". Terminating for service-side recovery.");
                std::_Exit(EXIT_FAILURE);
            }

            // Library mode: EventThread will handle recovery.
            return ret;
        }
#ifdef USE_PROFILER
        profiler.End(Profiler::EventType::H2D, req->task()->name(), id(), req->job_id());
        req->setDispatchTimestampNs(std::chrono::duration_cast<std::chrono::nanoseconds>(
            ProfilerClock::now().time_since_epoch()).count());
#endif
    }

    if (dxrt::DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_encoder_input.bin", req->inputs());
        DataDumpBin(
            req->taskData()->name() + "_input.bin",
            req->encoded_inputs_ptr(),
            req->taskData()->encoded_input_size());
    }
    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" signal to service input");

    serviceLayer()->HandleInferenceAcc(inferenceAcc, id());

    return 0;
}

int AccDeviceTaskLayer::OutputHandler(const dxrt_response_t& response, int ch)
{
    std::ignore = ch;
    if (response.proc_id == 0)
    {
        return 0;
    }
    if (response.proc_id != static_cast<uint32_t>(getpid()))
    {
        LOG_DXRT_DBG << "response from other process reqId: " << response.req_id
            << ", pid:" << response.proc_id << std::endl;
        return 0;
    }
    _dmaStopGate.WaitIfStopped();
    uint32_t reqId = response.req_id;
    dxrt_request_acc_t request_acc = peekInference(reqId);
    auto req = Request::GetById(reqId);
    if (req == nullptr)
    {
        DXRT_ASSERT(false, "req is nullptr "+std::to_string(reqId));
    }

    req->set_processed_unit("NPU_"+std::to_string(core()->id()), id(), response.dma_ch);
    dxrt_meminfo_t output = request_acc.output;
#ifdef USE_PROFILER
    const std::string profileTagBase =
        "[Device_" + std::to_string(id())
        + "][Job_" + std::to_string(req->job_id())
        + "][" + req->taskData()->name()
        + "][Req_" + std::to_string(req->id()) + "]";
#endif
    if (SKIP_INFERENCE_IO != 1 || req->modelType() != ModelType::MODEL_TYPE_ARGMAX)
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
            if (it != _responseReceiveTimestamps.end())
            {
                response_recv_ns = it->second;
                _responseReceiveTimestamps.erase(it);  // Cleanup after use
            }
        }

        // Measure Framework Response Handling Delay
        if (response_recv_ns > 0)
        {
            auto queue_delay_tp = std::make_shared<TimePoint>();
            queue_delay_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            queue_delay_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(output_handler_entry_ns));
            profiler.AddTimePoint(Profiler::EventType::FRAMEWORK_OVERHEAD, req->task()->name(), id(), req->job_id(), queue_delay_tp);
        }

        // Calculate NPU inference time
        // Strategy: Use response receive time as inference end, calculate backwards.
        // The previous approach (H2D completion = inference start) assumed inference
        // starts immediately after H2D, but NPU cores queue requests — so the actual
        // inference start is later than H2D completion when the core is busy.
        uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;

        if (response_recv_ns > 0)
        {
            auto npu_tp = std::make_shared<TimePoint>();
            npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns - inf_time_ns));
            profiler.AddTimePoint(Profiler::InferenceCoreEventType(response.dma_ch), req->task()->name(), id(), req->job_id(), npu_tp);
        }

        if (response.wait_timestamp > 0)
        {
            auto wait_tp = std::make_shared<TimePoint>();
            wait_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_start_time));
            wait_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_end_time));
            profiler.AddTimePoint(Profiler::EventType::SERVICE_PROCESS_WAIT, req->task()->name(), id(), req->job_id(), wait_tp);
        }

        // ponytail: compute NPU queue wait = (H2D end → response receive) - inf_time
        // This is the idle time spent in NPU's internal queue before core starts processing.
        if (response_recv_ns > 0 && req->dispatchTimestampNs() > 0 && response_recv_ns > req->dispatchTimestampNs())
        {
            int64_t h2d_to_response_us = static_cast<int64_t>(response_recv_ns - req->dispatchTimestampNs()) / 1000;
            int64_t queue_wait_us = h2d_to_response_us - static_cast<int64_t>(response.inf_time);
            req->setQueueWaitTime(queue_wait_us > 0 ? queue_wait_us : 0);
        }

        profiler.Start(Profiler::EventType::D2H, req->task()->name(), id(), req->job_id());

#endif
    #ifndef USE_PROFILER
        std::ignore = ch;
    #endif

        int ret2 = 0;

        // Get output memory info and config once for both normal and PPCPU cases
        NpuMemoryCacheSlice outputSlice;
        TaskStaticConfig cfg{};
        {
            std::unique_lock<std::mutex> npu_lock(npuInferenceLock());
            auto it = _ongoingInputAllocations.find(reqId);
            DXRT_ASSERT(it != _ongoingInputAllocations.end(),
                "OutputHandler missing cache allocation for reqId=" + std::to_string(reqId));
            outputSlice = it->second.slice;

            const int taskId = req->task()->id();
            const auto cfgIt = taskStaticConfigs().find(taskId);
            DXRT_ASSERT(cfgIt != taskStaticConfigs().end(),
                "TaskStaticConfig missing for taskId=" + std::to_string(taskId));
            cfg = cfgIt->second;
        }
        const uint64_t outputDelta = (cfg.output_all_offset != 0)
            ? static_cast<uint64_t>(cfg.output_all_offset)
            : data_align(req->taskData()->_encodedInputSize, 64);
        const uint64_t outputOffsetInSlice = outputDelta + static_cast<uint64_t>(cfg.last_output_offset);

        // PPCPU (type=3) processes filtered output with dynamic shape
        if (req->modelType() != ModelType::MODEL_TYPE_PPCPU)
        {
            // Skip memset - output.data is CMA physical address (not CPU-accessible)
            // DMA Read will overwrite the buffer anyway
            // sometimes it is useful to memset for initialization of pages in CMA buffer, but usually not needed
#if 0
            memset(SafeCast::BytePtrToPtr<void*>(output.data), 0, output.size);
#endif

            // Fault injection: corrupt the output DMA source address on the Nth read.
            // Activate:  export DXRT_FAULT_INJECT_OUTPUT=1000
            // Deactivate: unset DXRT_FAULT_INJECT_OUTPUT  (or don't set it)
            //
            // The env var lives in the RT client process. The decision (which
            // read to corrupt) is made here; the actual address corruption is
            // applied in the active DMA path below:
            //   - library mode:  corrupt output.base directly (in-process).
            //   - service mode:  flag the DMARead so dxrtd corrupts the device
            //                     address it resolves (client output.base is 0).
            bool injectFault = false;
            {
                static int s_faultAt = []() {
                    const char* env = getenv("DXRT_FAULT_INJECT_OUTPUT");
                    return env ? std::atoi(env) : 0;
                }();
                if (s_faultAt > 0)
                {
                    static std::atomic<int> s_outputReadCount{0};
                    int count = s_outputReadCount.fetch_add(1) + 1;
                    if (count == s_faultAt)
                    {
                        injectFault = true;
                    }
                }
            }

            SharedMemoryView outputView;
            outputView.info   = outputSlice.view.info;
            outputView.offset = outputSlice.view.offset + outputOffsetInSlice;
            outputView.size   = output.size;
            if (injectFault)
            {
                // In service mode the device address is resolved server-side
                // (dxrtd) from block_id + offset; the client's phys_addr_* are
                // never transmitted. Flag this read so the service corrupts the
                // device address it hands to the driver. Log here so the
                // [FAULT_INJECT] evidence appears in the run_model log.
                LOG_DXRT_ERR(
                    "[FAULT_INJECT] Output Read: flagging service-mode DMARead "
                    "for device-address corruption (reqId="
                    + std::to_string(reqId) + ", block_id="
                    + std::to_string(outputView.info.block_id) + ")");
                ret2 = serviceLayer()->DMAReadWithFaultInjection(outputView);
            }
            else
            {
                if (serviceLayer()->isRunOnService())
                {
                    ret2 = serviceLayer()->DMARead(outputView);
                }
                else
                {
                    ret2 = serviceLayer()->DMARead(
                        id(), outputView.deviceAddress(), output.data, output.size);
                }
            }
            // Zero-copy: when the encoded output already lives in this SHM slice
            // (PrepareZeroCopyIo), dst == src and the staging copy is skipped.
            if (serviceLayer()->isRunOnService() && ret2 == 0 && outputView.hostPtr() != nullptr
                && reinterpret_cast<void*>(output.data) != outputView.hostPtr())
            {
                std::memcpy(reinterpret_cast<void*>(output.data),
                            outputView.hostPtr(),
                            output.size);
            }
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
                    LOG_DXRT_ERR(
                        "PPCPU: Invalid ppu_filter_num=" + std::to_string(response.ppu_filter_num)
                        + " exceeds maximum boxes=" + std::to_string(expected_max_boxes)
                        + " (dtype=" + std::to_string(static_cast<int>(dtype))
                        + ", unit_size=" + std::to_string(unit_size) + ")");
                    // Clamp to maximum to prevent buffer overflow
                    validated_filter_num = static_cast<uint32_t>(expected_max_boxes);
                }

                // Configure memory info for PPCPU filtered output
                dxrt_meminfo_t ppcpu_output = SetMemInfo_PPCPU(
                    output, validated_filter_num, dtype,
                    getPpcpuOutputPtr(req_data, output.data));
                LOG_DXRT_DBG << "PPCPU Read - base=0x" << std::hex << ppcpu_output.base
                             << ", offset=0x" << ppcpu_output.offset
                             << ", data=0x" << ppcpu_output.data
                             << ", size=" << std::dec << ppcpu_output.size
                             << " (ppu_filter_num: " << validated_filter_num << ")" << std::endl;
                // Read PPCPU filtered output from device memory using SharedMemoryView
                SharedMemoryView ppcpuOutputView;
                ppcpuOutputView.info = outputSlice.view.info;
                ppcpuOutputView.offset = outputSlice.view.offset + outputOffsetInSlice + output.size;
                ppcpuOutputView.size = ppcpu_output.size;
                if (serviceLayer()->isRunOnService())
                {
                    ret2 = serviceLayer()->DMARead(ppcpuOutputView);
                }
                else
                {
                    ret2 = serviceLayer()->DMARead(
                        id(), ppcpuOutputView.deviceAddress(), ppcpu_output.data, ppcpu_output.size);
                }
                if (serviceLayer()->isRunOnService() && ret2 == 0 && ppcpuOutputView.hostPtr() != nullptr)
                {
                    std::memcpy(reinterpret_cast<void*>(ppcpu_output.data),
                                ppcpuOutputView.hostPtr(),
                                ppcpu_output.size);
                }
                assertAndInvalidatePpcpuOutput(req, reqId, ppcpu_output.size, ret2, id());
            }
        }

#ifdef DXRT_USE_DEVICE_VALIDATION
        if (req->is_validate_request())
        {
            ReadValidationOutput(req);
        }
#endif


#ifdef USE_PROFILER
        profiler.End(Profiler::EventType::D2H, req->task()->name(), id(), req->job_id());
#endif
        if ( ret2 != 0 )
        {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::READ_OUTPUT,
                LogMessages::RuntimeDispatch_FailToReadOutput(ret2, reqId, id())
            );

            // DMA Read failure: block the device so no more requests are submitted.
            // In service mode the DMA abort event goes to this client's fd (per-fd
            // event queue), NOT to the service's EventThread (which uses service's fd).
            // The service will never receive this event directly.
            //
            // To ensure recovery happens:
            //   - Block the device to stop new DMA submissions.
            //   - Exit immediately so the service's die_check_thread detects us as
            //     dead and handle_process_die() can trigger cleanup.
            //   - The service can then issue DXRT_CMD_RECOVERY after client cleanup.
            block();

            if (serviceLayer()->isRunOnService())
            {
                LOG_DXRT_ERR(
                    "DMA Read failed (errno=" + std::to_string(ret2)
                    + ") in service mode on device " + std::to_string(id())
                    + ". Terminating for service-side recovery.");
                std::_Exit(EXIT_FAILURE);
            }

            // Library mode: EventThread will handle recovery via the driver event queue.
            return 0;
        }
    }
    CallBack();

    if (DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_output.bin",
            req->encoded_outputs_ptr(), req->taskData()->encoded_output_size());
    }

    TASK_FLOW(
        "[" + std::to_string(req->job_id()) + "]"
        + req->taskData()->name() + " output is ready, load :"
        + std::to_string(_device->load()));

    // NOTE: The NPU memory-cache slice for this request is intentionally NOT released
    // here. The encoded output produced by the DMA read above still lives in that slice,
    // and NFH output decoding (processResponseHandler -> NFHLayer::handleOutput ->
    // DecodeOutputs) consumes it on a separate worker thread. Returning the slice to the
    // (LIFO, blocking) pool now would let a waiting request immediately reuse and overwrite
    // it before decode reads it -> torn output / bitmatch failure (repro: async + single
    // bound device once in-flight count exceeds buffer_count). The slice is released in
    // NFHLayer::handleOutput via ReleaseInferenceCache() after decode has consumed it.
    dxrt_response_t resp2 = response;
    processResponseHandler()(id(), reqId, &resp2);


    {
        std::unique_lock<std::mutex> lock(npuInferenceLock());
        _ongoingRequests.erase(reqId);
    }
    return 0;
}

void AccDeviceTaskLayer::ReleaseInferenceCache(int reqId)
{
    // Called by NFHLayer::handleOutput AFTER the encoded output has been decoded, so the
    // slice can now be safely returned to the pool for reuse. Counterpart to the release
    // that previously lived (too early) in OutputHandler. The taskId is stored alongside the
    // slice at allocation time, so release does NOT depend on the Request/Task still being
    // alive (this runs on an NFH worker thread, potentially after request teardown).
    OngoingCacheAllocation alloc;
    bool found = false;
    {
        std::unique_lock<std::mutex> lock(npuInferenceLock());
        auto it = _ongoingInputAllocations.find(reqId);
        if (it != _ongoingInputAllocations.end())
        {
            alloc = it->second;
            _ongoingInputAllocations.erase(it);
            found = true;
        }
    }

    if (!found)
    {
        return;  // already released, or request never acquired a cache slice
    }

    Deallocate_npuBuf(alloc.slice, alloc.taskId);
}

void AccDeviceTaskLayer::OutputReceiverThread(int id)
{
    dxrt_response_t response;
    int ret;
    int deviceId = core()->id();
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP;
#endif
    std::shared_ptr<TimePoint> tp = nullptr;
    std::ignore = tp;
    LOG_DXRT_DBG << core()->name() << " OutputReceiverThread " << id << ": Entry" << std::endl;

    int termination_count = 0;
    static constexpr int DXRT_DEVICE_TERMINATE_CONFIRM_COUNT = 5;
    bool shouldExit = false;

    while (!shouldExit && (isStopFlag(std::memory_order_acquire) == false))
    {
        memset(static_cast<void*>(&response), 0x00, sizeof(dxrt_response_t));
        response.req_id = static_cast<uint32_t>(id);
        if (isStopFlag(std::memory_order_acquire))
        {
            shouldExit = true;
            continue;
        }
        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread " << id << ": Waiting for response..." << std::endl;

        LOG_DXRT_DBG << "Device " << core()->name() << " OutputReceiverThread "
                 << id << ": Calling Process for response..." << std::endl;
#ifdef USE_PROFILER
        auto processStart = std::chrono::high_resolution_clock::now();
#endif
#if DXRT_USB_NETWORK_DRIVER
        ret = core()->Process(cmd, &response, sizeof(response));
#else
        ret = core()->Process(cmd, &response);
#endif
#ifdef USE_PROFILER
        auto processEnd = std::chrono::high_resolution_clock::now();
        auto processDuration = std::chrono::duration_cast<std::chrono::milliseconds>(processEnd - processStart);
        LOG_DXRT_DBG << "Device " << core()->name() << " OutputReceiverThread " << id
             << ": Process returned " << ret << " elapsed_ms=" << processDuration.count() << std::endl;
#else
        LOG_DXRT_DBG << "Device " << core()->name() << " OutputReceiverThread " << id
             << ": Process returned " << ret << std::endl;
#endif

        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread " << id << ": Response : " << response
                     << ", Device Load: " << load() << std::endl;
        if (ret == -1)
        {
            LOG_DXRT_DBG << core()->name() << " OutputReceiverThread " << id << ": Terminate detected." << std::endl;
            termination_count++;
            if (termination_count >= DXRT_DEVICE_TERMINATE_CONFIRM_COUNT)
            {
                shouldExit = true;
                continue;
            }
            else
                continue;
        }
#ifdef __linux__
        if (ret == -ECANCELED)
        {
            break;
        }
        if (ret == -ENODATA)
        {
            // No data available, can occur during shutdown or if the device is blocked.
            // Sleep briefly to avoid busy loop, then check stop flag again.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
#endif
        if (ret != 0)
        {
            std::cout << "ERROR RET: " << ret << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        if (response.status != 0)
        {
            uint32_t errCode = static_cast<uint32_t>(response.status);  // NOSONAR
            LOG_VALUE(response.status);

            // Check if this is a recoverable error that EventThread will handle
            // via DXRT_CMD_EVENT → DXRT_CMD_RECOVERY.
            //   100-103: DMA timeout + soft reset failure
            //   300:     FW timeout
            //   400-403: DMA HW abort (Abort MSI)
            bool isRecoverable = (errCode >= 100 && errCode < 200)
                              || (errCode == 300)
                              || (errCode >= 400 && errCode < 500);

            if (isRecoverable)
            {
                LOG_DXRT_ERR(
                    "[OutputReceiverThread " + std::to_string(id)
                    + "] Recoverable error (code=" + std::to_string(errCode)
                    + ") on device " + std::to_string(deviceId)
                    + ". Deferring to EventThread for DXRT_CMD_RECOVERY.");
                block();
                // Do NOT setStopFlag or DXRT_ASSERT here.
                // EventThread receives the driver event, performs recovery,
                // then the process can exit cleanly.
                shouldExit = true;
                continue;
            }

            // Non-recoverable error — fatal path (existing behavior)
            std::string _dumpFile = "dxrt.dump.bin." + std::to_string(core()->id());
            LOG_DXRT << "Error Detected: " + ErrTable(static_cast<dxrt_error_t>(response.status)) << std::endl;
            LOG_DXRT << "    Device " << deviceId << " dump to file " << _dumpFile << std::endl;
            std::vector<uint32_t> dump(1000, 0);
            core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
            std::ignore = std::find_if(
                dump.begin(),
                dump.end(),
                [](uint32_t value) { return value == 0xFFFFFFFF; });
            DataDumpBin(_dumpFile, dump.data(), static_cast<uint32_t>(dump.size()));
            DataDumpTxt(_dumpFile+".txt", dump.data(), 1, static_cast<uint32_t>(dump.size())/2, 2, true);
            setStopFlag(true);
            DXRT_ASSERT(false, "");
        }
        if (isStopFlag(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << core()->name() << " : requested to stop thread." << std::endl;
            shouldExit = true;
            continue;
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
        // Update usage timer for successful NPU response (Service OFF mode)
        if (!serviceLayer()->isRunOnService())
        {
            auto no_serviceLayer = std::dynamic_pointer_cast<NoServiceLayer>(serviceLayer());
            if (no_serviceLayer)
            {
                no_serviceLayer->addUsage(this->id(), response.dma_ch, static_cast<double>(response.inf_time));
                LOG_DXRT_DBG << "Device " << this->id() << " added usage for request " << response.req_id
                         << ", dma_ch=" << response.dma_ch
                         << ", inf_time=" << response.inf_time << " ms" << std::endl;
            }
        }

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
    bool shouldExit = false;
    LOG_DXRT_DBG << threadName << " : Entry" << std::endl;
#ifdef __linux__
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT_V2;
#elif _WIN32
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
#endif
    while ((shouldExit == false) && (isStopFlag(std::memory_order_acquire) == false))
    {
        if (isStopFlag(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << std::endl;
            shouldExit = true;
            continue;
        }
        dxrt::dx_pcie_dev_event_t eventInfo;
        if (!CatchEvent(cmd, &eventInfo))
        {
            shouldExit = true;
            continue;
        }

        shouldExit = HandleCaughtEvent(eventInfo);
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, LoopCount" << loopCnt << std::endl;
    _eventThreadTerminateFlag.store(true);
}

bool AccDeviceTaskLayer::CatchEvent(dxrt_cmd_t cmd, dxrt::dx_pcie_dev_event_t* eventInfo)
{
    memset(eventInfo, 0, sizeof(dxrt::dx_pcie_dev_event_t));
    int ret = core()->Process(cmd, eventInfo); // Waiting in kernel. (device::terminate())

#ifdef __linux__
    if (ret == -ECANCELED)
    {
        return false;
    }
#endif

    return true;
}

bool AccDeviceTaskLayer::HandleCaughtEvent(const dxrt::dx_pcie_dev_event_t& eventInfo)
{
    if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type) == dxrt::dxrt_event_t::DXRT_EVENT_ERROR)
    {
        if (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code) != dxrt::dxrt_error_t::ERR_NONE)
        {
            uint32_t err_code = eventInfo.dx_rt_err.err_code;
            std::string err_code_str;
            switch (static_cast<dxrt::dxrt_error_t>(err_code)) {
                case dxrt::dxrt_error_t::ERR_NPU0_HANG: err_code_str = "NPU0_HANG"; break;
                case dxrt::dxrt_error_t::ERR_NPU1_HANG: err_code_str = "NPU1_HANG"; break;
                case dxrt::dxrt_error_t::ERR_NPU2_HANG: err_code_str = "NPU2_HANG"; break;
                case dxrt::dxrt_error_t::ERR_NPU_BUS: err_code_str = "NPU_BUS"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_FAIL: err_code_str = "PCIE_DMA_CH0_FAIL"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH1_FAIL: err_code_str = "PCIE_DMA_CH1_FAIL"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH2_FAIL: err_code_str = "PCIE_DMA_CH2_FAIL"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH3_FAIL: err_code_str = "PCIE_DMA_CH3_FAIL"; break;
                case dxrt::dxrt_error_t::ERR_LPDDR_DED_WR: err_code_str = "LPDDR_DED_WR"; break;
                case dxrt::dxrt_error_t::ERR_LPDDR_DED_RD: err_code_str = "LPDDR_DED_RD"; break;
                case dxrt::dxrt_error_t::ERR_FW_TIMEOUT: err_code_str = "FW_TIMEOUT"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_ABORT: err_code_str = "PCIE_DMA_CH0_ABORT"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH1_ABORT: err_code_str = "PCIE_DMA_CH1_ABORT"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH2_ABORT: err_code_str = "PCIE_DMA_CH2_ABORT"; break;
                case dxrt::dxrt_error_t::ERR_PCIE_DMA_CH3_ABORT: err_code_str = "PCIE_DMA_CH3_ABORT"; break;
                case dxrt::dxrt_error_t::ERR_DEVICE_ERR: err_code_str = "DEVICE_ERR"; break;
                default: err_code_str = "UNKNOWN(" + std::to_string(err_code) + ")"; break;
            }

#ifdef USE_VNPU
            // Capture error details as string for LogMessage
            std::ostringstream error_details;
            error_details << eventInfo.dx_rt_err << "\n";
            core()->ShowPCIEDetails(error_details);

            LOG_DXRT_ERR(error_details.str());
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::ERROR,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::DEVICE_EVENT,
                LogMessages::RuntimeDispatch_DeviceEventError_VNPU(id(), err_code_str, error_details.str()));
#else
            LOG_DXRT_ERR(eventInfo.dx_rt_err);
            core()->ShowPCIEDetails();
            std::cout << "************************************************************************" << std::endl;
            std::cout << " * Error occurred! Please follow the steps below to recover the device." << std::endl;
            std::cout << " * Refer to the user guide if additional help is needed." << std::endl;
            std::cout << std::endl;
            std::cout << " Step 1: Reset the device using dxrt-cli" << std::endl;
            std::cout << "         > dxrt-cli -r 0" << std::endl;
            std::cout << " Step 2: Retry the inference using run_model" << std::endl;
            std::cout << "         > run_model -m [model.dxnn]" << std::endl;
            std::cout << " ** If the error persists, please contact DeepX support for assistance." << std::endl;
            std::cout << "************************************************************************" << std::endl;
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::ERROR,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::DEVICE_EVENT,
                LogMessages::RuntimeDispatch_DeviceEventError(id(), err_code_str));

            // recovery signal
            core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
#endif

#ifndef USE_VNPU

            // Classify error and handle accordingly
            //   100-103: DMA timeout + soft reset failure (driver engine_en cycle failed)
            //   200-201: LPDDR ECC error
            //   300:     FW timeout
            //   400-403: DMA HW abort (Abort MSI from HW)
            if (err_code >= 400 && err_code < 500)
            {
                // DMA HW Abort (Abort MSI) — recoverable via DXRT_CMD_RECOVERY
                TriggerRecovery(err_code);
            }
            else if (err_code >= 100 && err_code < 200)
            {
                // DMA timeout + soft reset failure — driver's engine_en cycle
                // could not clear CS=2. Full recovery (possibly PCIe SBR) needed.
                TriggerRecovery(err_code);
            }
            else if (err_code == 300)
            {
                // FW Timeout — recoverable
                TriggerRecovery(err_code);
            }
            else
            {
                // Non-recoverable errors (NPU hang, LPDDR ECC, etc.) — fatal
                DXRT_ASSERT(false, LogMessages::Device_DeviceErrorEvent(static_cast<int>(err_code)));
            }
#endif
            return true;
        }
    }
    else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type) == dxrt::dxrt_event_t::DXRT_EVENT_NOTIFY_THROT)
    {
        HandleThrottlingEvent(eventInfo.dx_rt_ntfy_throt);
    }
    else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_RECOVERY)
    {
        std::string type = "Unknown";
        if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_RMAP)
        {
            auto model = npuModelMap().begin()->second;
            DXRT_ASSERT(
                serviceLayer()->DMAWrite(id(), model.rmap.data, model.rmap.base + model.rmap.offset, model.rmap.size) == 0,
                "Recovery rmap failed to write model parameters(cmd)");
            LOG_DXRT_ERR("RMAP data has been recovered. This error can cause issues with NPU operation.");
            StartDev(RMAP_RECOVERY_DONE);
            type = "RMAP";
        }
        else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_WEIGHT)
        {
            auto model = npuModelMap().begin()->second;
            DXRT_ASSERT(
                serviceLayer()->DMAWrite(id(), model.weight.data, model.weight.base + model.weight.offset, model.weight.size) == 0,
                "Recovery weight failed to write model parameters(weight)");
            LOG_DXRT_ERR("Weight data has been recovered. This error can cause wrong result value.");
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
            LOG_DXRT_ERR(
                "Unknown data is received from device 0x"
                + ToHexString(eventInfo.dx_rt_recv.action) + "\n");
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

    return false;
}

void AccDeviceTaskLayer::HandleThrottlingEvent(const dxrt::dx_pcie_dev_ntfy_throt_t& throtInfo) const
{
    if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_THROTTLING))
        LOG_DXRT << throtInfo << std::endl;

    if (throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_DOWN
        || throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_UP
        || throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_DOWN
        || throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_UP) {

        std::string throt_code_str;
        switch (throtInfo.ntfy_code) {
            case dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_DOWN:
                throt_code_str = "FREQ_DOWN(MHz) "
                    + std::to_string(throtInfo.throt_freq[0])
                    + " to " + std::to_string(throtInfo.throt_freq[1]);
                break;
            case dxrt::dxrt_notify_throt_t::NTFY_THROT_FREQ_UP:
                throt_code_str = "FREQ_UP(MHz) "
                    + std::to_string(throtInfo.throt_freq[0])
                    + " to " + std::to_string(throtInfo.throt_freq[1]);
                break;
            case dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_DOWN:
                throt_code_str = "VOLT_DOWN(mV) "
                    + std::to_string(throtInfo.throt_voltage[0])
                    + " to " + std::to_string(throtInfo.throt_voltage[1]);
                break;
            case dxrt::dxrt_notify_throt_t::NTFY_THROT_VOLT_UP:
                throt_code_str = "VOLT_UP(mV) "
                    + std::to_string(throtInfo.throt_voltage[0])
                    + " to " + std::to_string(throtInfo.throt_voltage[1]);
                break;
            default:
                throt_code_str = "UNKNOWN";
                break;
        }

        auto level = RuntimeEventDispatcher::LEVEL::INFO;
        if (throtInfo.throt_temper >= THROTTLING_WARNING_TEMPERATURE)
        {
            level = RuntimeEventDispatcher::LEVEL::WARNING;
        }

        RuntimeEventDispatcher::GetInstance().DispatchEvent(
            level,
            RuntimeEventDispatcher::TYPE::DEVICE_STATUS,
            RuntimeEventDispatcher::CODE::THROTTLING_NOTICE,
            LogMessages::RuntimeDispatch_ThrottlingNotice(
                id(),
                throtInfo.npu_id,
                throt_code_str,
                throtInfo.throt_temper)
        );
    }
    else if (throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_BLOCK
        || throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_RELEASE
        || throtInfo.ntfy_code == dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_WARN)
    {
        std::string emergency_code_str;
        switch (throtInfo.ntfy_code) {
            case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_BLOCK:
                emergency_code_str = "EMERGENCY_BLOCK";
                break;
            case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_RELEASE:
                emergency_code_str = "EMERGENCY_RELEASE";
                break;
            case dxrt::dxrt_notify_throt_t::NTFY_EMERGENCY_WARN:
                emergency_code_str = "EMERGENCY_WARN";
                break;
            default:
                emergency_code_str = "UNKNOWN";
                break;
        }

        RuntimeEventDispatcher::GetInstance().DispatchEvent(
            RuntimeEventDispatcher::LEVEL::CRITICAL,
            RuntimeEventDispatcher::TYPE::DEVICE_STATUS,
            RuntimeEventDispatcher::CODE::THROTTLING_EMERGENCY,
            LogMessages::RuntimeDispatch_ThrottlingEmergency(
                id(),
                throtInfo.npu_id,
                emergency_code_str)
        );
    }
}

void AccDeviceTaskLayer::LogAbortDiagnostics(int channel, const dx_pcie_dev_err_t *err)   // NOSONAR
{
    std::cout << "[DMA ABORT] Channel " << channel << std::endl;
    std::cout << "  err_status=0x" << std::hex << std::setfill('0')
              << std::setw(8) << err->dma_err << std::dec << std::endl;
    std::cout << "  WR ch status: ["
        << err->dma_wr_ch_sts[0] << ", "
        << err->dma_wr_ch_sts[1] << ", "
        << err->dma_wr_ch_sts[2] << ", "
        << err->dma_wr_ch_sts[3] << "]" << std::endl;
    std::cout << "  RD ch status: ["
        << err->dma_rd_ch_sts[0] << ", "
        << err->dma_rd_ch_sts[1] << ", "
        << err->dma_rd_ch_sts[2] << ", "
        << err->dma_rd_ch_sts[3] << "]" << std::endl;
    std::cout << "  PCIe BDF: "
        << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(err->bus) << ":"
        << std::setw(2) << static_cast<int>(err->dev) << "."
        << static_cast<int>(err->func)
        << std::dec << std::endl;
    std::cout << "  RT driver version: " << err->rt_driver_version << std::endl;
    std::cout << "  PCIe driver version: " << err->pcie_driver_version << std::endl;
}

void AccDeviceTaskLayer::LogDmaFailDiagnostics(int channel, const dx_pcie_dev_err_t *err)   // NOSONAR
{
    std::cout << "[DMA FAIL] Channel " << channel << std::endl;
    std::cout << "  err_status=0x" << std::hex << std::setfill('0')
              << std::setw(8) << err->dma_err << std::dec << std::endl;
    std::cout << "  WR ch status: ["
        << err->dma_wr_ch_sts[0] << ", "
        << err->dma_wr_ch_sts[1] << ", "
        << err->dma_wr_ch_sts[2] << ", "
        << err->dma_wr_ch_sts[3] << "]" << std::endl;
    std::cout << "  RD ch status: ["
        << err->dma_rd_ch_sts[0] << ", "
        << err->dma_rd_ch_sts[1] << ", "
        << err->dma_rd_ch_sts[2] << ", "
        << err->dma_rd_ch_sts[3] << "]" << std::endl;
    std::cout << "  PCIe BDF: "
        << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(err->bus) << ":"
        << std::setw(2) << static_cast<int>(err->dev) << "."
        << static_cast<int>(err->func)
        << std::dec << std::endl;
}

void AccDeviceTaskLayer::LogFwTimeoutDiagnostics(const dx_pcie_dev_err_t *err)   // NOSONAR
{
    std::cout << "[FW TIMEOUT]" << std::endl;
    std::cout << "  err_code=" << err->err_code << std::endl;
    std::cout << "  FW version: " << err->fw_ver << std::endl;
    std::cout << "  NPU ID: " << err->npu_id << std::endl;
    std::cout << "  busy=" << err->busy << ", abnormal_cnt=" << err->abnormal_cnt << std::endl;
    std::cout << "  PCIe BDF: "
        << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(err->bus) << ":"
        << std::setw(2) << static_cast<int>(err->dev) << "."
        << static_cast<int>(err->func)
        << std::dec << std::endl;
}

void AccDeviceTaskLayer::TriggerRecovery(uint32_t errCode)
{
    LOG_DXRT_INFO("TriggerRecovery initiated for device " << id() << " with err_code=" << errCode);

    // Log diagnostic information based on error type
    if (errCode >= 400 && errCode < 500)
    {
        int abort_ch = static_cast<int>(errCode) - 400;
        // This diagnostic logging will be done in DmaAbortRecoveryThread
        LOG_DXRT_ERR("DMA HW Abort on channel " << abort_ch);
    }
    else if (errCode >= 100 && errCode < 200)
    {
        int fail_ch = static_cast<int>(errCode) - 100;
        LOG_DXRT_ERR("DMA timeout + soft reset failure on channel " << fail_ch);
    }
    else if (errCode == 300)
    {
        LOG_DXRT_ERR("FW Timeout on device " << id());
    }

    // Dispatch recovery on a separate thread to avoid blocking the event listener
    // (matching the pattern of HandleDmaAbortError)
    if (!_recoveryPending.exchange(true, std::memory_order_acq_rel))
    {
        if (_recoveryThread.joinable())
            _recoveryThread.join();
        _recoveryThread = std::thread(&AccDeviceTaskLayer::DmaAbortRecoveryThread, this);
    }
    else
    {
        LOG_DXRT_INFO("Recovery already pending for device " << id() << ", skipping duplicate dispatch");
    }
}

void AccDeviceTaskLayer::HandleDmaAbortError(const dx_pcie_dev_err_t *err)
{
    int abort_ch = static_cast<int>(err->err_code) - 400;
    LogAbortDiagnostics(abort_ch, err);

    RuntimeEventDispatcher::GetInstance().DispatchEvent(
        RuntimeEventDispatcher::LEVEL::ERROR,
        RuntimeEventDispatcher::TYPE::DEVICE_IO,
        RuntimeEventDispatcher::CODE::DEVICE_EVENT,
        LogMessages::RuntimeDispatch_DmaAbort(id(), abort_ch, err->dma_err));

    // Dispatch recovery on a separate thread to avoid blocking the event listener
    if (!_recoveryPending.exchange(true, std::memory_order_acq_rel))
    {
        if (_recoveryThread.joinable())
            _recoveryThread.join();
        _recoveryThread = std::thread(&AccDeviceTaskLayer::DmaAbortRecoveryThread, this);
    }
    else
    {
        LOG_DXRT_INFO("DMA Abort recovery already pending, skipping duplicate dispatch");
    }
}

void AccDeviceTaskLayer::HandleDmaFailError(const dx_pcie_dev_err_t *err)
{
    int fail_ch = static_cast<int>(err->err_code) - 100;
    LogDmaFailDiagnostics(fail_ch, err);

    RuntimeEventDispatcher::GetInstance().DispatchEvent(
        RuntimeEventDispatcher::LEVEL::ERROR,
        RuntimeEventDispatcher::TYPE::DEVICE_IO,
        RuntimeEventDispatcher::CODE::DEVICE_EVENT,
        LogMessages::RuntimeDispatch_DmaFail(id(), fail_ch, err->dma_err));

    // DMA completion failure also needs recovery
    if (!_recoveryPending.exchange(true, std::memory_order_acq_rel))
    {
        if (_recoveryThread.joinable())
            _recoveryThread.join();
        _recoveryThread = std::thread(&AccDeviceTaskLayer::DmaAbortRecoveryThread, this);
    }
}

void AccDeviceTaskLayer::HandleFwTimeoutError(const dx_pcie_dev_err_t *err)
{
    LogFwTimeoutDiagnostics(err);

    RuntimeEventDispatcher::GetInstance().DispatchEvent(
        RuntimeEventDispatcher::LEVEL::ERROR,
        RuntimeEventDispatcher::TYPE::DEVICE_IO,
        RuntimeEventDispatcher::CODE::DEVICE_EVENT,
        LogMessages::RuntimeDispatch_FwTimeout(id()));

    // FW timeout also needs recovery
    if (!_recoveryPending.exchange(true, std::memory_order_acq_rel))
    {
        if (_recoveryThread.joinable())
            _recoveryThread.join();
        _recoveryThread = std::thread(&AccDeviceTaskLayer::DmaAbortRecoveryThread, this);
    }
}

void AccDeviceTaskLayer::PauseDmaRequests()
{
    core()->PauseDMA();
}

void AccDeviceTaskLayer::ResumeDmaRequests()
{
    core()->ResumeDMA();
}

void AccDeviceTaskLayer::DmaAbortRecoveryThread()
{
    LOG_DXRT_INFO("DmaAbortRecoveryThread: Starting recovery for device " << id());

    RuntimeEventDispatcher::GetInstance().DispatchEvent(
        RuntimeEventDispatcher::LEVEL::WARNING,
        RuntimeEventDispatcher::TYPE::DEVICE_CORE,
        RuntimeEventDispatcher::CODE::RECOVERY_OCCURRED,
        LogMessages::RuntimeDispatch_RecoveryStarted(id()));

    int ret = triggerRecovery();

    if (ret == 0)
    {
        RuntimeEventDispatcher::GetInstance().DispatchEvent(
            RuntimeEventDispatcher::LEVEL::WARNING,
            RuntimeEventDispatcher::TYPE::DEVICE_CORE,
            RuntimeEventDispatcher::CODE::RECOVERY_OCCURRED,
            LogMessages::RuntimeDispatch_RecoveryCompleted(id()));

        // Recovery succeeded: DMA requests have been unblocked by
        // ResumeAfterRecovery (called inside triggerRecovery).
        LOG_DXRT_INFO("DMA Recovery completed for device " << id() << ". Resuming normal operation.");
    }
    else
    {
        // triggerRecovery() already called OnRecoveryFailed (which aborts),
        // so this branch should never be reached.
        LOG_DXRT_ERR(
            "DmaAbortRecoveryThread: Recovery failed for device " + std::to_string(id())
            + ", ret=" + std::to_string(ret));
        std::abort();
    }

    _recoveryPending.store(false, std::memory_order_release);
}
void AccDeviceTaskLayer::StartThread()
{
    core()->CheckVersion();

    // In service mode, the service (dxrtd) owns the EventThread and handles
    // driver error events + DXRT_CMD_RECOVERY.  The client should NOT run its
    // own EventThread because:
    //   (a) If the driver event queue is per-device (shared), the client would
    //       consume events that the service needs to process.
    //   (b) If per-fd, both would try DXRT_CMD_RECOVERY simultaneously.
    // Instead, the client receives error notifications via IPC broadcast from
    // the service (ERROR_REPORT → ProcessErrorFromService → _Exit).
    if (serviceLayer()->isRunOnService() == false)
    {
        // Wire PauseForRecovery / ResumeAfterRecovery to this task layer's
        // _dmaStopGate so that DeviceTaskLayer::triggerRecovery() can stop
        // new DMA requests and drain in-flight ones before the ioctl.
        auto no_service = std::dynamic_pointer_cast<NoServiceLayer>(serviceLayer());
        if (no_service)
        {
            static constexpr uint32_t RECOVERY_DMA_DRAIN_TIMEOUT_MS = 5000;
            no_service->RegisterRecoveryCallbacks(
                id(),
                [this]() {
                    // Step 1+2: block new requests and drain in-flight DMA.
                    _dmaStopGate.SetStop(true);
                    waitForInflightDmaCompletion(RECOVERY_DMA_DRAIN_TIMEOUT_MS);
                },
                [this]() {
                    // Step 4: unblock new requests after successful recovery.
                    _dmaStopGate.SetStop(false);
                });
        }

        _eventThread = std::thread(&AccDeviceTaskLayer::EventThread, this);
    }

    if (serviceLayer()->isRunOnService() == false)
    {
        for (uint32_t i = 0; i < core()->info().num_dma_ch; i++)
        {
            _outputDispatcher.emplace_back(&AccDeviceTaskLayer::OutputReceiverThread, this, i);
            _outputDispatcherTerminateFlag[i].store(false, std::memory_order_release);
        }
        //Load PPCPU firmware if not running on service layer
        size_t fw_size = PPCPUDataLoader::GetDataSize();
        uint64_t mem_offset = serviceLayer()->Allocate(id(), NO_TASK_ID, MemoryType::Normal, fw_size);
        core()->InitPPCPU(mem_offset);

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
    setStopFlag(true);

    // Join recovery thread if running
    if (_recoveryThread.joinable())
        _recoveryThread.join();

    _inputHandlerQueue.Stop();
    _outputHandlerQueue.Stop();

    Terminate();
    if (_eventThreadStartFlag.load(std::memory_order_acquire))
    {
        _eventThread.join();
    }
    //GCOVR_EXCL_START
    size_t outputDispatcher_size = _outputDispatcher.size();
    for (size_t i = 0; i < outputDispatcher_size; i++)
    {
        _outputDispatcher[i].join();
    }
    _outputDispatcher.clear();
    //GCOVR_EXCL_STOP
}

void AccDeviceTaskLayer::ProcessThrottleFromService(const dxrt::dx_pcie_dev_ntfy_throt_t &throtInfo)
{
    HandleThrottlingEvent(throtInfo);
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
    auto model = npuModelMap()[task->id()];
    auto memInfo = dxrt_meminfo_t(inferenceAcc.output);

    // Get validation tensor once to avoid multiple calls
    Tensor validateTensor = req->ValidateOutputTensor();
    void *ptr = validateTensor.data();

    LOG_DXRT_DBG << "  Model Info:" << std::endl;
    LOG_DXRT_DBG << "    model.output_all_size: " << std::dec << model.output_all_size << " bytes" << std::endl;
    LOG_DXRT_DBG << "    model.last_output_offset: 0x" << std::hex << model.last_output_offset << std::endl;
    LOG_DXRT_DBG << "    memInfo.offset: 0x" << std::hex << memInfo.offset << std::endl;
    LOG_DXRT_DBG << "  Validation Tensor: " << validateTensor << std::endl;

    memInfo.data = SafeCast::PointerToInteger<void*>(ptr);
    memInfo.offset -= model.last_output_offset;
    memInfo.size = model.output_all_size;

    DXRT_ASSERT(serviceLayer()->DMARead(id(), memInfo.base + memInfo.offset, memInfo.data, memInfo.size) == 0, "Fail to read device");
    LOG_DXRT_DBG << "  Output Memory Info:" << std::endl;
    LOG_DXRT_DBG << "    data: 0x" << std::hex << memInfo.data << std::endl;
    LOG_DXRT_DBG << "    base: 0x" << std::hex << memInfo.base << std::endl;
    LOG_DXRT_DBG << "    offset: 0x" << std::hex << memInfo.offset << std::endl;
    LOG_DXRT_DBG << "    size: " << std::dec << memInfo.size << " bytes" << std::endl;

    LOG_DXRT_DBG << "  Encoded Input Size: " << req->taskData()->encoded_input_size() << " bytes" << std::endl;
    LOG_DXRT_DBG << "  Encoded Output Size: " << req->taskData()->encoded_output_size() << " bytes" << std::endl;

    if (memInfo.size == 0) memInfo = inferenceAcc.output;  // temporary solution for zero size argmax model

    if (serviceLayer()->DMARead(id(), memInfo.base + memInfo.offset, memInfo.data, memInfo.size) != 0) {
        LOG_DXRT_DBG << "Validate output is empty." << std::endl;
    }

}
#endif

dxrt_meminfo_t AccDeviceTaskLayer::SetMemInfo_PPCPU(const dxrt_meminfo_t& rmap_output,
                                                      size_t ppu_filter_num,
                                                      DataType dtype,
                                                      void* output_ptr) const
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
    ppcpu_output.data = SafeCast::PointerToInteger<void*>(output_ptr);

    return ppcpu_output;
}

} // namespace dxrt
