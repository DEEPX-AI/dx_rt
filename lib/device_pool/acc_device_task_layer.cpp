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
#endif // USE_VNPU

#include "../data/ppcpu.h"

// Macros duplicated from original implementation unit (can be refactored later)
#define RMAP_RECOVERY_DONE      (1)
#define WEIGHT_RECOVERY_DONE    (2)

namespace dxrt {

constexpr int THROTTLING_WARNING_TEMPERATURE = 95;

AccDeviceTaskLayer::AccDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface)
: DeviceTaskLayer(dev, service_interface), _inputHandlerQueue(dev->name()+"_input", dev->GetReadChannel(),
    std::bind(&AccDeviceTaskLayer::InputHandler, this, std::placeholders::_1, std::placeholders::_2)),
    _outputHandlerQueue(dev->name()+"_output", dev->GetWriteChannel(),
    std::bind(&AccDeviceTaskLayer::OutputHandler, this, std::placeholders::_1, std::placeholders::_2))
{}

#ifndef USE_VNPU
int AccDeviceTaskLayer::RegisterTask(TaskData* task)
{
    int ret = 0;
    const int tId = task->id();
    UniqueLock lock(_taskDataLock);

    dxrt_model_t model = task->_npuModel;

    npuModelMap()[tId] = model;

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    model.rmap.base = core()->info().mem_addr;
    model.weight.base = core()->info().mem_addr;

    // Allocate model param regions (simple forward allocation)


    uint64_t weight_offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.weight.size);
    uint64_t rmap_offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.rmap.size);
    if (rmap_offset > weight_offset)
    {
        auto temp_addr = rmap_offset;
        rmap_offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.rmap.size);
        serviceLayer()->DeAllocate(id(), temp_addr);
    }
    constexpr uint64_t ERROR_ALLOC = std::numeric_limits<uint64_t>::max();
    if (rmap_offset == ERROR_ALLOC)
    {
        LOG_DXRT_ERR("Failed to allocate rmap memory in NPU memory");
        return -1;
    }
    if (weight_offset == ERROR_ALLOC)
    {
        LOG_DXRT_ERR("Failed to allocate weight memory in NPU memory");
        return -1;
    }

    model.weight.offset = static_cast<uint32_t>(weight_offset);
    model.rmap.offset = static_cast<uint32_t>(rmap_offset);

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

    inf.model_type = model.type;
    inf.model_format = model.format;
    inf.model_cmds = static_cast<uint32_t>(model.cmds);
    inf.cmd_offset = model.rmap.offset;
    inf.weight_offset = model.weight.offset;
    inf.op_mode = model.op_mode;
    for (int i = 0; i < MAX_CHECKPOINT_COUNT; ++i)
    {
        inf.datas[i] = model.checkpoints[i];
    }

    {
        std::unique_lock<std::mutex> lk(npuInferenceLock());
        _npuInferenceAcc[tId] = inf;
    }

    // Write model params
    ret = core()->Write(model.rmap);
    DXRT_ASSERT(ret == 0, "failed to write model rmap parameters" + std::to_string(ret));
    ret = core()->Write(model.weight);
    DXRT_ASSERT(ret == 0, "failed to write model weight parameters" + std::to_string(ret));

    // v8 PPCPU: Write PPU binary if exists
    if (task->_isPPCPU && task->_data && task->_data->size() >= 3)
    {
        const auto& ppu_binary = (*task->_data)[2];  // index 2 is PPU binary
        if (!ppu_binary.empty())
        {
            // Copy PPU binary to device-specific storage to prevent multi-device DMA conflicts
            _ppuBinaryData[tId] = ppu_binary;  // Deep copy
            const auto& ppu_binary_copy = _ppuBinaryData[tId];

            // Allocate PPU binary region in device memory
            dxrt_meminfo_t ppu_mem;
            ppu_mem.base = model.rmap.base;

            uint64_t ppu_offset = serviceLayer()->BackwardAllocateForTask(id(), tId, ppu_binary_copy.size());
            if (ppu_offset == ERROR_ALLOC)
            {
                LOG_DXRT_ERR("Failed to allocate ppuMem memory in NPU memory");
                _ppuBinaryData.erase(tId);
                return -1;
            }

            ppu_mem.offset = static_cast<uint32_t>(ppu_offset);
            ppu_mem.size = static_cast<uint32_t>(ppu_binary_copy.size());
            ppu_mem.data = SafeCast::PointerToInteger<const uint8_t*>(ppu_binary_copy.data());
            ret = core()->Write(ppu_mem);
            DXRT_ASSERT(ret == 0, "failed to write PPU binary parameters" + std::to_string(ret));

            // Store PPU binary offset in device-specific map (not in TaskData to avoid conflicts)
            _ppuBinaryOffsets[tId] = ppu_mem.offset;

            LOG_DXRT_DBG << "Device " << id() << " wrote PPU binary (device-specific copy): offset=0x" << std::hex << ppu_mem.offset
                         << ", size=" << std::dec << ppu_mem.size << " bytes" << std::endl;
        }
    }

    // Verify (skip if size is 0)
    if (model.rmap.size > 0 && model.weight.size > 0)
    {
        auto verify = [&](const dxrt_meminfo_t& info, const std::string& name) {
            if (info.size == 0)
            {
                return 0;
            }

            std::vector<uint8_t> read_buf(info.size);
            dxrt_meminfo_t read_cmd = info;
            read_cmd.data = SafeCast::PointerToInteger<uint8_t*>(read_buf.data());

            if (core()->Read(read_cmd) == 0)
            {
                return std::memcmp(SafeCast::IntegerToPointer<void*>(info.data),
                    read_buf.data(), info.size) == 0 ? 0: 1;
            }
            else
            {
                LOG_DXRT_ERR("Failed to read back " + name + " for verification");
            }
            return 1;
        };

        int fail_count = 0;
        fail_count += verify(model.rmap, "rmap");
        fail_count += verify(model.weight, "weight");

        DXRT_ASSERT(fail_count == 0, "failed to verify model parameters, fail count: " + std::to_string(fail_count));
    }
    else
    {
        LOG_DXRT_DBG << "Device " << id() << " skipping verify (rmap.size=" << model.rmap.size
                        << ", weight.size=" << model.weight.size << ")" << std::endl;
    }


    _inputTensorFormats[tId] = task->inputs(SafeCast::IntegerToPointer<void*>(inf.input.data));
    _outputTensorFormats[tId] = task->outputs(SafeCast::IntegerToPointer<void*>(inf.output.data));


    // ACC cache registration similar to Device
    const int64_t block_size = data_align(task->encoded_input_size(), 64)
                           + static_cast<int64_t>(task->_outputMemSize);

   // int npu_cache_count equals to DXRT_TASK_MAX_LOAD
   int npu_cache_count = task->get_buffer_count();
    while (npu_cache_count > 0)
    {
        if (memoryCacheManager().registerMemoryCache(task->id(), block_size, npu_cache_count) == false)
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

#else

int AccDeviceTaskLayer::RegisterTask(TaskData* task)
{
    LOG_DXRT_DBG << "Device " << id() << " RegisterTask ACC" << std::endl;
    int ret = 0;
    const int tId = task->id();
    UniqueLock lock(_taskDataLock);

    dxrt_model_t model = task->_npuModel;

    npuModelMap()[tId] = model;

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    model.rmap.base = core()->info().mem_addr;
    model.weight.base = core()->info().mem_addr;

    // Allocate model param regions (simple forward allocation)
    model.weight.offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.weight.size);
    model.rmap.offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.rmap.size);
    if (model.rmap.offset > model.weight.offset)
    {
        uint32_t temp_addr = model.rmap.offset;
        model.rmap.offset = serviceLayer()->BackwardAllocateForTask(id(), tId, model.rmap.size);
        serviceLayer()->DeAllocate(id(), temp_addr);
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
        std::unique_lock<std::mutex> lk(npuInferenceLock());
        _npuInferenceAcc[tId] = inf;
    }

    // Write model params using temporary CMA buffers for zero-copy DMA
    // Allocate CMA buffers for DMA transmission
    std::unique_ptr<FixedSizeBuffer> rmap_dma_buffer;
    std::unique_ptr<FixedSizeBuffer> weight_dma_buffer;
    void* rmap_vaddr = nullptr;
    uint64_t rmap_paddr = 0;
    void* weight_vaddr = nullptr;
    uint64_t weight_paddr = 0;

    // Allocate and use CMA buffer for RMAP DMA transmission
    if (model.rmap.size > 0)
    {
        rmap_dma_buffer = std::make_unique<FixedSizeBuffer>(
            model.rmap.size, 1, BufferAllocType::CMA_DMA);
        rmap_vaddr = rmap_dma_buffer->getBuffer();
        rmap_paddr = rmap_dma_buffer->getPhysicalAddress(rmap_vaddr);

        if (rmap_vaddr && rmap_paddr)
        {
            // Copy model rmap data to CMA buffer (CPU uses virtual address)
            memcpy(rmap_vaddr, reinterpret_cast<const void*>(model.rmap.data), model.rmap.size);

            // Flush CPU cache to RAM before DMA Write (CPU -> RAM -> Device)
            rmap_dma_buffer->flushCache(rmap_vaddr, model.rmap.size, false);

            // Use physical address for DMA
            dxrt_meminfo_t rmap_dma = model.rmap;
            rmap_dma.data = rmap_paddr;

            LOG_DXRT << "Device " << id() << " Writing rmap: vaddr=0x" << std::hex << rmap_vaddr
                     << ", paddr=0x" << rmap_paddr
                     << ", base=0x" << rmap_dma.base << ", offset=0x" << rmap_dma.offset
                     << ", size=" << std::dec << rmap_dma.size << std::endl;

            // // Save original RMAP data to file
            // DataDumpBin("debug_rmap_original.bin", rmap_vaddr, model.rmap.size);
            LOG_DXRT_DBG << "Saved RMAP original data to debug_rmap_original.bin" << std::endl;

            ret = core()->Write(rmap_dma);
            DXRT_ASSERT(ret == 0, "failed to write model rmap parameters" + std::to_string(ret));
        }
    }

    if (model.weight.size > 0)
    {
        weight_dma_buffer = std::make_unique<FixedSizeBuffer>(
            model.weight.size, 1, BufferAllocType::CMA_DMA);
        weight_vaddr = weight_dma_buffer->getBuffer();
        weight_paddr = weight_dma_buffer->getPhysicalAddress(weight_vaddr);

        if (weight_vaddr && weight_paddr)
        {
            // Copy model weight data to CMA buffer (CPU uses virtual address)
            memcpy(weight_vaddr, reinterpret_cast<const void*>(model.weight.data), model.weight.size);

            // Flush CPU cache to RAM before DMA Write (CPU -> RAM -> Device)
            weight_dma_buffer->flushCache(weight_vaddr, model.weight.size, false);

            // Use physical address for DMA
            dxrt_meminfo_t weight_dma = model.weight;
            weight_dma.data = weight_paddr;

            LOG_DXRT << "Device " << id() << " Writing weight: vaddr=0x" << std::hex << weight_vaddr
                     << ", paddr=0x" << weight_paddr
                     << ", base=0x" << weight_dma.base << ", offset=0x" << weight_dma.offset
                     << ", size=" << std::dec << weight_dma.size << std::endl;

            // // Save original Weight data to file
            // DataDumpBin("debug_weight_original.bin", weight_vaddr, model.weight.size);
            LOG_DXRT_DBG << "Saved Weight original data to debug_weight_original.bin" << std::endl;

            ret = core()->Write(weight_dma);
            DXRT_ASSERT(ret == 0, "failed to write model weight parameters" + std::to_string(ret));
        }
    }

    // v8 PPCPU: Write PPU binary if exists (using CMA buffer like RMAP/Weight)
    if (task->_isPPCPU && task->_data && task->_data->size() >= 3)
    {
        const auto& ppuBinary = (*task->_data)[2];  // index 2 is PPU binary
        if (!ppuBinary.empty())
        {
            // Allocate CMA buffer for PPU binary DMA transmission
            std::unique_ptr<FixedSizeBuffer> ppu_dma_buffer = std::make_unique<FixedSizeBuffer>(
                ppuBinary.size(), 1, BufferAllocType::CMA_DMA);

            void* ppu_vaddr = ppu_dma_buffer->getBuffer();
            uint64_t ppu_paddr = ppu_dma_buffer->getPhysicalAddress(ppu_vaddr);

            if (ppu_vaddr && ppu_paddr)
            {
                // Copy PPU binary to CMA buffer (CPU uses virtual address)
                memcpy(ppu_vaddr, SafeCast::BytePtrToPtr<const void*>(ppuBinary.data()), ppuBinary.size());

                // Flush CPU cache to RAM before DMA Write (CPU -> RAM -> Device)
                ppu_dma_buffer->flushCache(ppu_vaddr, ppuBinary.size(), false);

                // Allocate PPU binary region in device memory
                dxrt_meminfo_t ppuMem;
                ppuMem.base = model.rmap.base;
                ppuMem.offset = serviceLayer()->BackwardAllocateForTask(id(), tId, ppuBinary.size());
                ppuMem.size = ppuBinary.size();
                ppuMem.data = ppu_paddr;  // Use physical address for DMA

                LOG_DXRT << "Device " << id() << " Writing PPU binary: vaddr=0x" << std::hex << ppu_vaddr
                         << ", paddr=0x" << ppu_paddr
                         << ", base=0x" << ppuMem.base << ", offset=0x" << ppuMem.offset
                         << ", size=" << std::dec << ppuMem.size << std::endl;

                ret = core()->Write(ppuMem);
                DXRT_ASSERT(ret == 0, "failed to write PPU binary parameters" + std::to_string(ret));

                // Store PPU binary offset in task data for later use in inference request
                task->_ppuBinaryOffset = ppuMem.offset;

                LOG_DXRT_DBG << "Device " << id() << " wrote PPU binary: offset=0x" << std::hex << ppuMem.offset
                             << ", size=" << std::dec << ppuMem.size << " bytes" << std::endl;
            }
            // CMA buffer automatically released via RAII when unique_ptr goes out of scope
        }
    }

    // Verify using CMA buffers (reuse temp buffers allocated for Write)
    if (model.rmap.size > 0 && model.weight.size > 0)
    {
        // Reuse the CMA buffers we just wrote to for verification Read
        dxrt_meminfo_t cmd_read(model.rmap);
        dxrt_meminfo_t weight_read(model.weight);

        // Clear CMA buffers before Read
        if (rmap_vaddr && rmap_paddr)
        {
            // memset(rmap_vaddr, 0, model.rmap.size);
            cmd_read.data = rmap_paddr;  // Use physical address for DMA

            LOG_DXRT << "Device " << id() << " Reading rmap for verification: "
                     << "base=0x" << std::hex << cmd_read.base << ", offset=0x" << cmd_read.offset
                     << ", data(paddr)=0x" << cmd_read.data << ", vaddr=0x" << rmap_vaddr
                     << ", size=" << std::dec << cmd_read.size << std::endl;

            if (core()->Read(cmd_read) == 0) {
                // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
                rmap_dma_buffer->flushCache(rmap_vaddr, model.rmap.size, true);

                // // Save readback RMAP data to file
                // DataDumpBin("debug_rmap_readback.bin", rmap_vaddr, cmd_read.size);
                LOG_DXRT_DBG << "Saved RMAP readback data to debug_rmap_readback.bin" << std::endl;

                // Compare using virtual address
                ret += memcmp(reinterpret_cast<const void*>(model.rmap.data), rmap_vaddr, cmd_read.size);
                if (ret != 0) {
                    LOG_DXRT << "[WARNING] RMAP verification mismatch" << std::endl;
                    // Show first few bytes
                    for (size_t i = 0; i < std::min(static_cast<size_t>(cmd_read.size), static_cast<size_t>(64)); ++i) {
                        uint8_t wrote = reinterpret_cast<const uint8_t*>(model.rmap.data)[i];
                        uint8_t read = static_cast<uint8_t*>(rmap_vaddr)[i];
                        if (wrote != read) {
                            LOG_DXRT << "  RMAP mismatch at byte " << i << ": wrote=0x"
                                     << std::hex << (int)wrote << ", read=0x" << (int)read << std::dec << std::endl;
                            break;
                        }
                    }
                }
            }
        }

        if (weight_vaddr && weight_paddr) {
            // memset(weight_vaddr, 0, model.weight.size);
            weight_read.data = weight_paddr;  // Use physical address for DMA

            LOG_DXRT << "Device " << id() << " Reading weight for verification: "
                     << "base=0x" << std::hex << weight_read.base << ", offset=0x" << weight_read.offset
                     << ", data(paddr)=0x" << weight_read.data << ", vaddr=0x" << weight_vaddr
                     << ", size=" << std::dec << weight_read.size << std::endl;

            if (core()->Read(weight_read) == 0) {
                // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
                weight_dma_buffer->flushCache(weight_vaddr, model.weight.size, true);
                // // Save readback Weight data to file
                // DataDumpBin("debug_weight_readback.bin", weight_vaddr, weight_read.size);
                LOG_DXRT_DBG << "Saved Weight readback data to debug_weight_readback.bin" << std::endl;

                // Compare using virtual address
                ret += memcmp(reinterpret_cast<const void*>(model.weight.data), weight_vaddr, weight_read.size);
                if (ret != 0) {
                    LOG_DXRT << "[WARNING] Weight verification mismatch" << std::endl;
                    // Show first few bytes
                    for (size_t i = 0; i < std::min(static_cast<size_t>(weight_read.size), static_cast<size_t>(64)); ++i) {
                        uint8_t wrote = reinterpret_cast<const uint8_t*>(model.weight.data)[i];
                        uint8_t read = static_cast<uint8_t*>(weight_vaddr)[i];
                        if (wrote != read) {
                            LOG_DXRT << "  Weight mismatch at byte " << i << ": wrote=0x"
                                     << std::hex << (int)wrote << ", read=0x" << (int)read << std::dec << std::endl;
                            break;
                        }
                    }
                }
            }
        }

        if (ret != 0) {
            LOG_DXRT << "[WARNING] Device " << id() << " model parameter verification failed: " << ret << std::endl;
        } else {
            LOG_DXRT << "Device " << id() << " model parameters verified successfully" << std::endl;
        }

        DXRT_ASSERT(ret == 0, "failed to check data integrity of model parameters" + std::to_string(ret));
    } else {
        LOG_DXRT_DBG << "Device " << id() << " skipping verify (rmap.size=" << model.rmap.size
                     << ", weight.size=" << model.weight.size << ")" << std::endl;
    }

    // CMA buffers automatically released via RAII when unique_ptr goes out of scope
    // No manual cleanup needed
    _inputTensorFormats[tId] = task->inputs(reinterpret_cast<void*>(inf.input.data));
    _outputTensorFormats[tId] = task->outputs(reinterpret_cast<void*>(inf.output.data));


    // ACC cache registration similar to Device
    const int64_t block_size = data_align(task->encoded_input_size(), 64)
                           + static_cast<int64_t>(task->_outputMemSize);

   //int npu_cache_count = DXRT_TASK_MAX_LOAD;
    int npu_cache_count = task->get_buffer_count();
    while (npu_cache_count > 0)
    {
        if (memoryCacheManager().registerMemoryCache(task->id(), block_size, npu_cache_count) == false)
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
#endif // USE_VNPU

int AccDeviceTaskLayer::Release(TaskData* task)
{
    UniqueLock lock(_taskDataLock);
    int taskId = task->id();


    dxrt_request_acc_t npu_inference_acc;
    {
        std::unique_lock<std::mutex> inference_lock(npuInferenceLock());
        npu_inference_acc = _npuInferenceAcc[taskId];
        _npuInferenceAcc.erase(taskId);
        npuModelMap().erase(taskId);
    }

    if (memoryCacheManager().canGetCache(taskId))
    {
        memoryCacheManager().unRegisterMemoryCache(taskId);
    }
    serviceLayer()->DeAllocate(id(), npu_inference_acc.cmd_offset);
    serviceLayer()->DeAllocate(id(), npu_inference_acc.weight_offset);

    // Cleanup device-specific PPU binary storage
    _ppuBinaryData.erase(taskId);
    auto ppu_offset_it = _ppuBinaryOffsets.find(taskId);
    if (ppu_offset_it != _ppuBinaryOffsets.end())
    {
        serviceLayer()->DeAllocate(id(), ppu_offset_it->second);
        _ppuBinaryOffsets.erase(ppu_offset_it);
    }

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

    void* req_input_ptr = nullptr;
    if (req->inputs.size() > 0)
        req_input_ptr = req->encoded_inputs_ptr;

    {
        SharedLock lock(_taskDataLock);
        /* accelerator device: runtime allocation */
        dxrt_request_acc_t npu_inference_acc;
        {
            std::unique_lock<std::mutex> inference_lock(npuInferenceLock());
            npu_inference_acc = _npuInferenceAcc[taskId];
        }
        const auto& model = task->_npuModel;

        LOG_DXRT_DBG << "Device " << id() << " InferenceRequestACC: taskId=" << taskId
                 << ", model.type=" << static_cast<int>(model.type)
                 << ", npu_inference_acc.model_type=" << static_cast<int>(npu_inference_acc.model_type)
                 << ", task->_isPPCPU=" << task->_isPPCPU
                 << ", custom_offset(before)=0x" << std::hex << npu_inference_acc.custom_offset << std::dec
                 << std::endl;

        npu_inference_acc.req_id = req->requestId;
        if (req_input_ptr == nullptr)
        {
            LOG_DXRT_ERR("Device::InferenceRequest_ACC - req_input_ptr is nullptr");
        }
        else
        {
#ifndef USE_VNPU
            npu_inference_acc.input.data = SafeCast::PointerToInteger<void*>(req_input_ptr);
#else
            // Use physical address for DMA if available (zero-copy), otherwise virtual address
            if (req->encoded_inputs_phy != 0)
            {
                npu_inference_acc.input.data = req->encoded_inputs_phy;
                LOG_DXRT_DBG << "Device " << id() << " Using CMA input physical address: 0x"
                             << std::hex << req->encoded_inputs_phy << std::dec << std::endl;
            }
            else
            {
                LOG_DXRT_ERR("Device " << id() << " Error: input physical address is zero, falling back to virtual address");
            }
#endif // USE_VNPU
        }

        npu_inference_acc.input.offset = static_cast<uint32_t>(AllocateFromCache(
            data_align(task->_encodedInputSize, 64) + task->_outputMemSize, taskId));
        if (Configuration::_sNpuValidateOpt.load())
        {
            loadCounter()++;
        }

#ifndef USE_VNPU
        npu_inference_acc.output.data = SafeCast::PointerToInteger<void*>(req->encoded_outputs_ptr);  // device buffer -> task buffer
#else
        // Use physical address for DMA if available (zero-copy), otherwise virtual address
        if (req->encoded_outputs_phy != 0)
        {
            npu_inference_acc.output.data = req->encoded_outputs_phy;
            LOG_DXRT_DBG << "Device " << id() << " Using CMA output physical address: 0x"
                         << std::hex << req->encoded_outputs_phy << std::dec << std::endl;
        }
        else
        {
            LOG_DXRT_ERR("Device " << id() << " Error: output physical address is zero, falling back to virtual address");
        }
#endif // USE_VNPU

        auto outputOffset = npu_inference_acc.input.offset;
        if (model.output_all_offset == 0)
            outputOffset += data_align(task->_encodedInputSize, 64);
        else
            outputOffset += model.output_all_offset;

        npu_inference_acc.output.offset = outputOffset + model.last_output_offset;
        // Set custom_offset to PPU binary offset for firmware to execute PPU
        if (task->_isPPCPU)
        {
            // Use device-specific PPU offset (not TaskData->_ppuBinaryOffset to avoid multi-device conflicts)
            auto it = _ppuBinaryOffsets.find(taskId);
            if (it != _ppuBinaryOffsets.end())
            {
                npu_inference_acc.custom_offset = it->second;
                LOG_DXRT_DBG << "Device " << id() << " PPCPU inference: custom_offset=0x" << std::hex
                         << it->second << ", model_type=" << std::dec
                         << static_cast<int>(npu_inference_acc.model_type) << std::endl;
            }
            else
            {
                LOG_DXRT_ERR("Device " << id() << " PPCPU task " << taskId << " missing PPU offset");
                npu_inference_acc.custom_offset = 0;
            }
        }
        else
        {
            npu_inference_acc.custom_offset = 0;
        }

        npu_inference_acc.proc_id = getpid();
        npu_inference_acc.bound = boundOp;
        {
            ObjectsPool::GetInstance().GetRequestById(req->requestId)->setOutputs(
                task->outputs(SafeCast::IntegerToPointer<void*>(npu_inference_acc.output.data)));
        }
        req->outputs = task->outputs(req->output_buffer_base);
        {
            std::unique_lock<std::mutex> npu_inference_lock(npuInferenceLock());
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
        LOG_DXRT_DBG << "Device " << id() << " Pushing request " << req->requestId
                 << " to InputHandlerQueue" << std::endl;
        _inputHandlerQueue.PushWork(req->requestId);

        LOG_DXRT_DBG << "request to input worker returned " << ret << std::endl;
    }
    return 0;
}

dxrt_request_acc_t AccDeviceTaskLayer::peekInference(int id)
{
    std::unique_lock<std::mutex> lock(npuInferenceLock());
    return _ongoingRequests[id];
}

int AccDeviceTaskLayer::InputHandler(const int& requestId, int ch)
{
    LOG_DXRT_DBG << "Device " << id() << " InputHandler START for request " << requestId << std::endl;
    auto& profiler = Profiler::GetInstance();
    dxrt_request_acc_t inferenceAcc = peekInference(requestId);
    int channel = ch;

    inferenceAcc.dma_ch = channel;
    RequestPtr req = Request::GetById(requestId);

    // Debug: Log input DMA parameters
    LOG_DXRT_DBG << "Device " << id() << " InputHandler req=" << requestId
             << ": input.data(phy)=0x" << std::hex << inferenceAcc.input.data
             << ", input.base=0x" << inferenceAcc.input.base
             << ", input.offset=0x" << inferenceAcc.input.offset
             << ", input.size=" << std::dec << inferenceAcc.input.size << std::endl;

    if (SKIP_INFERENCE_IO != 1)
    {
        TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" write input, load: "+std::to_string(load));
#ifdef USE_VNPU
        // Flush CPU cache to RAM before DMA Write (CPU -> RAM -> Device)
        if (req->encoded_inputs_ptr() != nullptr && inferenceAcc.input.size > 0)
        {
            RK_MPI_MMZ_FlushCacheVaddrEnd(req->encoded_inputs_ptr(), inferenceAcc.input.size, RK_MMZ_SYNC_RW);
            LOG_DXRT_DBG << "Device " << id() << " Flushed input cache before DMA Write for request "
                         << requestId << ", ptr=" << req->encoded_inputs_ptr()
                         << ", size=" << inferenceAcc.input.size << std::endl;
        }
#endif // USE_VNPU
#ifdef USE_PROFILER
        profiler.Start("PCIe Write[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(inferenceAcc.dma_ch)+")");
#endif

        int ret = core()->Write(inferenceAcc.input);
        if (ret < 0)
        {
            RuntimeEventDispatcher::GetInstance().DispatchEvent(
                RuntimeEventDispatcher::LEVEL::CRITICAL,
                RuntimeEventDispatcher::TYPE::DEVICE_IO,
                RuntimeEventDispatcher::CODE::WRITE_INPUT,
                LogMessages::RuntimeDispatch_FailToWriteInput(ret, requestId, ch)
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
                LOG_DXRT_ERR("DMA write failed (errno=" << ret << ") in service mode on device "
                    << id() << ". Terminating for service-side recovery.");
                std::_Exit(EXIT_FAILURE);
            }

            // Library mode: EventThread will handle recovery.
            return ret;
        }
#ifdef USE_PROFILER
        profiler.End("PCIe Write[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(inferenceAcc.dma_ch)+")");
        // Store PCIe Write completion timestamp for accurate NPU Core timing
        {
            std::lock_guard<std::mutex> lock(_writeTimestampLock);
            _writeCompleteTimestamps[requestId] = std::chrono::duration_cast<std::chrono::nanoseconds>(
                ProfilerClock::now().time_since_epoch()).count();
        }
#endif
    }

    if (dxrt::DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_encoder_input.bin", req->inputs());
        DataDumpBin(req->taskData()->name() + "_input.bin", req->encoded_inputs_ptr(), req->taskData()->encoded_input_size());
    }
    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" signal to service input");

    serviceLayer()->HandleInferenceAcc(inferenceAcc, id());
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
        LOG_DXRT_DBG << "response from other process reqid: " << response.req_id
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
        if (response_recv_ns > 0) {
            auto queue_delay_tp = std::make_shared<TimePoint>();
            queue_delay_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            queue_delay_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(output_handler_entry_ns));
            profiler.AddTimePoint("Framework Response Handling Delay[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" +
                req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch),
                queue_delay_tp);
        }

        // Get PCIe Write completion timestamp for accurate NPU Core timing
        uint64_t write_complete_ns = 0;
        {
            std::lock_guard<std::mutex> lock(_writeTimestampLock);
            auto it = _writeCompleteTimestamps.find(reqId);
            if (it != _writeCompleteTimestamps.end())
            {
                write_complete_ns = it->second;
                _writeCompleteTimestamps.erase(it);  // Cleanup after use
            }
        }

        // Calculate NPU Core execution time
        // Strategy: Use PCIe Write completion as NPU start time (most accurate for timeline)
        // This assumes NPU starts immediately after PCIe Write completes
        uint64_t inf_time_ns = static_cast<uint64_t>(response.inf_time) * 1000;

        if (write_complete_ns > 0)
        {
            // Best case: Use PCIe Write completion time as NPU start
            auto npu_tp = std::make_shared<TimePoint>();
            npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns));
            npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(write_complete_ns + inf_time_ns));
            profiler.AddTimePoint("NPU Core[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), npu_tp);
        }
        else if (response_recv_ns > 0)
        {
            // Fallback: use response receive time to estimate NPU end, calculate backwards
            auto npu_tp = std::make_shared<TimePoint>();
            npu_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns));
            npu_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response_recv_ns - inf_time_ns));
            profiler.AddTimePoint("NPU Core[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), npu_tp);
        }

        if (response.wait_timestamp > 0) {
            auto wait_tp = std::make_shared<TimePoint>();
            wait_tp->start = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_start_time));
            wait_tp->end = ProfilerClock::time_point(std::chrono::nanoseconds(response.wait_end_time));
            profiler.AddTimePoint("Service Process Wait[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "]_" + std::to_string(response.dma_ch), wait_tp);
        }

        profiler.Start("PCIe Read[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(ch)+")");

#endif
        int read_ch = ch;
        int ret2 = 0;
        bool ctrlCmd = true;
#if DXRT_USB_NETWORK_DRIVER
        ctrlCmd = false;
#endif

        // PPCPU (type=3) processes filtered output with dynamic shape
        if (req->modelType() != ModelType::MODEL_TYPE_PPCPU)
        {
            // Skip memset - output.data is CMA physical address (not CPU-accessible)
            // DMA Read will overwrite the buffer anyway
            // sometimes it is useful to memset for initialization of pages in CMA buffer, but usually not needed
#if 0
            memset(SafeCast::BytePtrToPtr<void*>(output.data), 0, output.size);
#endif

            // Fault injection: corrupt output.base high 32 bits on Nth read.
            // Activate:  export DXRT_FAULT_INJECT_OUTPUT=1000
            // Deactivate: unset DXRT_FAULT_INJECT_OUTPUT  (or don't set it)
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
                        LOG_DXRT_ERR("[FAULT_INJECT] Output Read #" << count
                            << ": Corrupting output.base high 32 bits"
                            << " (0x" << std::hex << output.base << " -> 0x"
                            << (output.base & 0x00000000FFFFFFFFULL) << std::dec << ")");
                        output.base &= 0x00000000FFFFFFFFULL;
                    }
                }
            }

            ret2 = core()->Read(output, read_ch, ctrlCmd);

#ifdef USE_VNPU
            // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
            if (ret2 == 0 && req->encoded_outputs_ptr() != nullptr)
            {
                RK_MPI_MMZ_FlushCacheVaddrStart(req->encoded_outputs_ptr(), output.size, RK_MMZ_SYNC_RW);
            }
#endif // USE_VNPU
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
#ifndef USE_VNPU
                dxrt_meminfo_t ppcpu_output = SetMemInfo_PPCPU(
                    output,
                    validated_filter_num,
                    dtype,
                    req_data->encoded_output_ptrs[0]  // Use output_buffer_base instead of encoded_output_ptrs
                );

                LOG_DXRT_DBG << "PPCPU Read - offset: 0x" << std::hex << ppcpu_output.offset
                             << ", size: " << std::dec << ppcpu_output.size
                             << " (ppu_filter_num: " << validated_filter_num << ")" << std::endl;
#else
                // Use output.data (already set to CMA physical address in InferenceRequestACC)
                dxrt_meminfo_t ppcpu_output = SetMemInfo_PPCPU(
                    output,
                    validated_filter_num,
                    dtype,
                    reinterpret_cast<void*>(output.data)  // Use physical address from output.data
                );

                LOG_DXRT_DBG << "PPCPU Read - base=0x" << std::hex << ppcpu_output.base
                         << ", offset=0x" << ppcpu_output.offset
                         << ", data(paddr)=0x" << ppcpu_output.data
                         << ", size=" << std::dec << ppcpu_output.size
                         << " (ppu_filter_num: " << validated_filter_num << ")" << std::endl;
#endif // USE_VNPU
                // Read PPCPU filtered output from device memory
                ret2 = core()->Read(ppcpu_output, read_ch, ctrlCmd);

#ifdef USE_VNPU
                DXRT_ASSERT(ret2 == 0, "Failed to read PPCPU output, errno=" + std::to_string(ret2) + ", reqId=" + std::to_string(reqId));

                // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
                if (ret2 == 0 && req->encoded_outputs_ptr() != nullptr)
                {
                    // Direct cache invalidation for external CMA buffer (not managed by FixedSizeBuffer)
                    RK_MPI_MMZ_FlushCacheVaddrStart(req->encoded_outputs_ptr(), ppcpu_output.size, RK_MMZ_SYNC_RW);
                    LOG_DXRT_DBG << "Device " << id() << " Invalidated PPCPU output cache (direct RK MPI) for request "
                                 << reqId << ", ptr=" << req->encoded_outputs_ptr()
                                 << ", size=" << ppcpu_output.size << std::endl;
                }
#endif // USE_VNPU
            }
        }

#ifdef DXRT_USE_DEVICE_VALIDATION
        if (req->is_validate_request())
        {
            ReadValidationOutput(req);
        }
#endif


#ifdef USE_PROFILER
        profiler.End("PCIe Read[Device_" + std::to_string(id()) + "][Job_" + std::to_string(req->job_id()) + "][" + req->taskData()->name() + "][Req_" + std::to_string(req->id()) + "](" + std::to_string(ch)+")");
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
                LOG_DXRT_ERR("DMA Read failed (errno=" << ret2 << ") in service mode on device "
                    << id() << ". Terminating for service-side recovery.");
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

    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->taskData()->name()+" output is ready, load :"+std::to_string(_device->load()));

    Deallocate_npuBuf(request_acc.input.offset, req->taskData()->id());

    dxrt_response_t resp2 = response;
    processResponseHandler()(id(),req->id(), &resp2);


    {
        std::unique_lock<std::mutex> lock(npuInferenceLock());
        _ongoingRequests.erase(req->id());
    }
    return 0;
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
    LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Entry" << std::endl;

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
        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Waiting for response..." << std::endl;
#if DXRT_USB_NETWORK_DRIVER
        ret = core()->Process(cmd, &response, sizeof(response));
#else
        LOG_DXRT_DBG << "Device " << core()->name() << " OutputReceiverThread "<<id<<": Calling Process for response..." << std::endl;
        ret = core()->Process(cmd, &response);
        LOG_DXRT_DBG << "Device " << core()->name() << " OutputReceiverThread "<<id<<": Process returned " << ret << std::endl;
#endif
        LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Response : " << response
                     << ", Device Load: " << load() << std::endl;
        if (ret == -1)
        {
            LOG_DXRT_DBG << core()->name() << " OutputReceiverThread "<<id<<": Terminate detected." << std::endl;
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
                LOG_DXRT_ERR("[OutputReceiverThread " << id << "] Recoverable error (code="
                    << errCode << ") on device " << deviceId
                    << ". Deferring to EventThread for DXRT_CMD_RECOVERY.");
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
                HandleDmaAbortError(&eventInfo.dx_rt_err);
            }
            else if (err_code >= 100 && err_code < 200)
            {
                // DMA timeout + soft reset failure — driver's engine_en cycle
                // could not clear CS=2. Full recovery (possibly PCIe SBR) needed.
                HandleDmaFailError(&eventInfo.dx_rt_err);
            }
            else if (err_code == 300)
            {
                // FW Timeout — recoverable
                HandleFwTimeoutError(&eventInfo.dx_rt_err);
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
            DXRT_ASSERT(core()->Write(model.rmap, 3) == 0, "Recovery rmap failed to write model parameters(cmd)");
            LOG_DXRT_ERR("RMAP data has been recovered. This error can cause issues with NPU operation.");
            StartDev(RMAP_RECOVERY_DONE);
            type = "RMAP";
        }
        else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_WEIGHT)
        {
            auto model = npuModelMap().begin()->second;
            DXRT_ASSERT(core()->Write(model.weight, 3) == 0, "Recovery weight failed to write model parameters(weight)");
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
    std::cout << "  err_status=0x" << std::hex << std::setfill('0') << std::setw(8) << err->dma_err << std::dec << std::endl;
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
    std::cout << "  err_status=0x" << std::hex << std::setfill('0') << std::setw(8) << err->dma_err << std::dec << std::endl;
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

        // In library mode (non-service), DDR content is lost after SBR.
        // Models must be re-loaded, which requires process restart.
        LOG_DXRT_INFO("DMA Recovery completed for device " << id()
            << ". Terminating process for clean restart.");
        std::_Exit(EXIT_FAILURE);
    }
    else
    {
        LOG_DXRT_ERR("DmaAbortRecoveryThread: Recovery failed for device " << id()
            << ", ret=" << ret);

        RuntimeEventDispatcher::GetInstance().DispatchEvent(
            RuntimeEventDispatcher::LEVEL::CRITICAL,
            RuntimeEventDispatcher::TYPE::DEVICE_CORE,
            RuntimeEventDispatcher::CODE::RECOVERY_OCCURRED,
            LogMessages::RuntimeDispatch_RecoveryFailed(id(),
                "DXRT_CMD_RECOVERY ioctl returned " + std::to_string(ret)));

        // Fatal: if recovery fails, the device cannot be used
        LOG_DXRT_ERR("Fatal: DMA abort recovery failed. Device " << id()
            << " is unusable. Consider restarting the process.");
        block();
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
        uint64_t mem_offset = serviceLayer()->Allocate(id(), fw_size);

#ifndef USE_VNPU
        dxrt_meminfo_t fw_meminfo;
        fw_meminfo.base = core()->info().mem_addr;
        fw_meminfo.offset = static_cast<uint32_t>(mem_offset);
        fw_meminfo.size = static_cast<uint32_t>(fw_size);
        fw_meminfo.data = SafeCast::PointerToInteger<void*>(PPCPUDataLoader::GetData());

#else
        // Allocate CMA buffer for PPCPU firmware DMA transmission
        std::unique_ptr<FixedSizeBuffer> firmware_dma_buffer = std::make_unique<FixedSizeBuffer>(
            fw_size, 1, BufferAllocType::CMA_DMA);

        void* fw_vaddr = firmware_dma_buffer->getBuffer();
        uint64_t fw_paddr = firmware_dma_buffer->getPhysicalAddress(fw_vaddr);

        DXRT_ASSERT(fw_vaddr != nullptr && fw_paddr != 0, "Failed to allocate CMA buffer for PPCPU firmware");

        // Copy PPCPU firmware to CMA buffer (CPU uses virtual address)
        memcpy(fw_vaddr, PPCPUDataLoader::GetData(), fw_size);

        // Flush CPU cache to RAM before DMA Write (CPU -> RAM -> Device)
        firmware_dma_buffer->flushCache(fw_vaddr, fw_size, false);

        // Use physical address for DMA
        dxrt_meminfo_t fw_meminfo;
        fw_meminfo.base = core()->info().mem_addr;
        fw_meminfo.offset = mem_offset;
        fw_meminfo.size = static_cast<uint32_t>(fw_size);
        fw_meminfo.data = fw_paddr;

        LOG_DXRT << "Device " << id() << " Writing PPCPU firmware: vaddr=0x" << std::hex << fw_vaddr
                 << ", paddr=0x" << fw_paddr
                 << ", base=0x" << fw_meminfo.base << ", offset=0x" << fw_meminfo.offset
                 << ", size=" << std::dec << fw_meminfo.size << std::endl;
#endif // USE_VNPU
        int ret1 = core()->Write(fw_meminfo);
        DXRT_ASSERT(ret1 == 0, "Failed to load PPCPU firmware to device: ret=" + std::to_string(ret1));

        dxrt_req_meminfo_t meminfo_req;
        meminfo_req.base = fw_meminfo.base;
        meminfo_req.offset = fw_meminfo.offset;
        meminfo_req.size = fw_meminfo.size;
        meminfo_req.data = fw_meminfo.data;
        meminfo_req.ch = 0;

        core()->DoCustomCommand(&meminfo_req, dxrt::dxrt_custom_sub_cmt_t::DX_INIT_PPCPU,sizeof(dxrt_req_meminfo_t));
        // CMA buffer automatically released via RAII when unique_ptr goes out of scope

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

    DXRT_ASSERT(core()->Read(memInfo) == 0, "Fail to read device");
    LOG_DXRT_DBG << "  Output Memory Info:" << std::endl;
    LOG_DXRT_DBG << "    data: 0x" << std::hex << memInfo.data << std::endl;
    LOG_DXRT_DBG << "    base: 0x" << std::hex << memInfo.base << std::endl;
    LOG_DXRT_DBG << "    offset: 0x" << std::hex << memInfo.offset << std::endl;
    LOG_DXRT_DBG << "    size: " << std::dec << memInfo.size << " bytes" << std::endl;

    LOG_DXRT_DBG << "  Encoded Input Size: " << req->taskData()->encoded_input_size() << " bytes" << std::endl;
    LOG_DXRT_DBG << "  Encoded Output Size: " << req->taskData()->encoded_output_size() << " bytes" << std::endl;

    if (memInfo.size == 0) memInfo = inferenceAcc.output;  // temporary solution for zero size argmax model

    if (core()->Read(memInfo) != 0) {
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
