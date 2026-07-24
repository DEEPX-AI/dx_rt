/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

// VNPU (CMA / RK SoC) implementation of AccDeviceTaskLayer::RegisterTask.
// For the non-VNPU (PCIe) implementation see acc_device_task_layer_register_pcie.cpp.

#ifdef USE_VNPU

#include <memory>
#include <cstring>
#include <limits>
#include <algorithm>
#include "dxrt/common.h"
#include "dxrt/device_task_layer.h"
#include "inference_context.h"
#include "dxrt/task_data.h"
#include "dxrt/util.h"
#include "dxrt/fixed_size_buffer.h"
#include "../resource/log_messages.h"
#include "dxrt/safe_cast.h"

namespace dxrt {

namespace {
constexpr uint64_t ERROR_ALLOC = std::numeric_limits<uint64_t>::max();
}

int AccDeviceTaskLayer::RegisterTask(TaskData* task)
{
    auto dmaPass = _dmaStopGate.WaitIfStopped();
    std::ignore = dmaPass;
    LOG_DXRT_DBG << "Device " << id() << " RegisterTask ACC" << std::endl;
    int ret = 0;
    const int tId = task->id();
    UniqueLock lock(_taskDataLock);

    dxrt_model_t model = task->_npuModel;

    DXRT_ASSERT(task->input_size() > 0, "Input size is 0");
    DXRT_ASSERT(task->output_size() > 0, "Output size is 0");

    model.rmap.base = core()->info().mem_addr;
    model.weight.base = core()->info().mem_addr;
    if (serviceLayer()->isRunOnService())
    {
        // In service mode, dynamic IPC alloc/dealloc paths are offset-oriented.
        // Keep base at 0 to avoid client/server mem_addr base mismatches.
        model.rmap.base = 0;
        model.weight.base = 0;
    }

    // Allocate model param regions (simple forward allocation)
    model.weight.offset = serviceLayer()->Allocate(id(), tId, MemoryType::Model_weight, model.weight.size);
    model.rmap.offset = serviceLayer()->Allocate(id(), tId, MemoryType::Model_rmap, model.rmap.size);
    if (model.weight.offset == ERROR_ALLOC || model.rmap.offset == ERROR_ALLOC)
    {
        if (model.weight.offset != ERROR_ALLOC)
        {
            serviceLayer()->DeAllocate(id(), model.weight.offset);
        }
        if (model.rmap.offset != ERROR_ALLOC)
        {
            serviceLayer()->DeAllocate(id(), model.rmap.offset);
        }
        LOG_DXRT_ERR("Failed to allocate model memory (weight/rmap)");
        return -1;
    }

    if (model.rmap.offset > model.weight.offset)
    {
        const uint64_t temp_addr = model.rmap.offset;
        model.rmap.offset = serviceLayer()->Allocate(id(), tId, MemoryType::Model_rmap, model.rmap.size);
        if (model.rmap.offset == ERROR_ALLOC)
        {
            serviceLayer()->DeAllocate(id(), model.weight.offset);
            serviceLayer()->DeAllocate(id(), temp_addr);
            LOG_DXRT_ERR("Failed to re-allocate rmap memory during address conflict resolution");
            return -1;
        }
        serviceLayer()->DeAllocate(id(), temp_addr);
    }

    // Store model with offsets set
    npuModelMap()[tId] = model;

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

            // LOG_DXRT_DBG << "Saved RMAP original data to debug_rmap_original.bin" << std::endl;

            ret = serviceLayer()->DMAWrite(id(), rmap_dma.data, rmap_dma.base + rmap_dma.offset, rmap_dma.size);
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

            // LOG_DXRT_DBG << "Saved Weight original data to debug_weight_original.bin" << std::endl;

            ret = serviceLayer()->DMAWrite(id(), weight_dma.data, weight_dma.base + weight_dma.offset, weight_dma.size);
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
                ppuMem.offset = serviceLayer()->Allocate(id(), tId, MemoryType::Model_ppu_binary, ppuBinary.size());
                ppuMem.size = ppuBinary.size();
                ppuMem.data = ppu_paddr;  // Use physical address for DMA

                LOG_DXRT << "Device " << id() << " Writing PPU binary: vaddr=0x" << std::hex << ppu_vaddr
                         << ", paddr=0x" << ppu_paddr
                         << ", base=0x" << ppuMem.base << ", offset=0x" << ppuMem.offset
                         << ", size=" << std::dec << ppuMem.size << std::endl;

                ret = serviceLayer()->DMAWrite(id(), ppuMem.data, ppuMem.base + ppuMem.offset, ppuMem.size);
                DXRT_ASSERT(ret == 0, "failed to write PPU binary parameters" + std::to_string(ret));

                // Store PPU binary offset in device-specific map (not in TaskData to avoid conflicts)
                _ppuBinaryOffsets[tId] = ppuMem.offset;

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
            cmd_read.data = rmap_paddr;  // Use physical address for DMA

            LOG_DXRT << "Device " << id() << " Reading rmap for verification: "
                     << "base=0x" << std::hex << cmd_read.base << ", offset=0x" << cmd_read.offset
                     << ", data(paddr)=0x" << cmd_read.data << ", vaddr=0x" << rmap_vaddr
                     << ", size=" << std::dec << cmd_read.size << std::endl;

            if (serviceLayer()->DMARead(id(), cmd_read.base + cmd_read.offset, cmd_read.data, cmd_read.size) == 0) {
                // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
                rmap_dma_buffer->flushCache(rmap_vaddr, model.rmap.size, true);

                // LOG_DXRT_DBG << "Saved RMAP readback data to debug_rmap_readback.bin" << std::endl;

                // Compare using virtual address
                ret += memcmp(reinterpret_cast<const void*>(model.rmap.data), rmap_vaddr, cmd_read.size);
                if (ret != 0) {
                    LOG_DXRT << "[WARNING] RMAP verification mismatch" << std::endl;
                    for (size_t i = 0; i < (std::min)(static_cast<size_t>(cmd_read.size), static_cast<size_t>(64)); ++i) {
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
            weight_read.data = weight_paddr;  // Use physical address for DMA

            LOG_DXRT << "Device " << id() << " Reading weight for verification: "
                     << "base=0x" << std::hex << weight_read.base << ", offset=0x" << weight_read.offset
                     << ", data(paddr)=0x" << weight_read.data << ", vaddr=0x" << weight_vaddr
                     << ", size=" << std::dec << weight_read.size << std::endl;

            if (serviceLayer()->DMARead(id(), weight_read.base + weight_read.offset, weight_read.data, weight_read.size) == 0) {
                // Invalidate CPU cache after DMA Read (Device -> RAM, then CPU reads RAM)
                weight_dma_buffer->flushCache(weight_vaddr, model.weight.size, true);

                // LOG_DXRT_DBG << "Saved Weight readback data to debug_weight_readback.bin" << std::endl;

                // Compare using virtual address
                ret += memcmp(reinterpret_cast<const void*>(model.weight.data), weight_vaddr, weight_read.size);
                if (ret != 0) {
                    LOG_DXRT << "[WARNING] Weight verification mismatch" << std::endl;
                    for (size_t i = 0;
                        i < (std::min)(static_cast<size_t>(weight_read.size), static_cast<size_t>(64));
                        ++i) {
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
    _inputTensorFormats[tId]  = task->inputs(nullptr);
    _outputTensorFormats[tId] = task->outputs(nullptr);


    // ACC cache registration similar to Device.
    // Ensure the cache slice can cover the actual output location:
    // output_delta + last_output_offset + last_output_size.
    const int64_t aligned_input_size = static_cast<int64_t>(data_align(task->encoded_input_size(), 64));
    const int64_t output_delta = (model.output_all_offset != 0)
        ? static_cast<int64_t>(model.output_all_offset)
        : aligned_input_size;
    const int64_t required_output_span = output_delta
        + static_cast<int64_t>(model.last_output_offset)
        + static_cast<int64_t>(model.last_output_size);
    const int64_t block_size = (std::max)(
        aligned_input_size + static_cast<int64_t>(task->_outputMemSize),
        required_output_span);

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
        // cleanup allocated resources before returning failure
        serviceLayer()->DeAllocate(id(), model.rmap.offset);
        serviceLayer()->DeAllocate(id(), model.weight.offset);
        auto ppu_offset_it = _ppuBinaryOffsets.find(tId);
        if (ppu_offset_it != _ppuBinaryOffsets.end())
        {
            serviceLayer()->DeAllocate(id(), ppu_offset_it->second);
            _ppuBinaryOffsets.erase(ppu_offset_it);
        }

        {
            std::unique_lock<std::mutex> lk(npuInferenceLock());
            npuModelMap().erase(tId);
            taskStaticConfigs().erase(tId);
        }
        _inputTensorFormats.erase(tId);
        _outputTensorFormats.erase(tId);
        return -1;
    }

    // Layer 3: populate TaskStaticConfig from the now-finalised model state.
    // custom_offset is available here because PPU binary allocation (if any) completed above.
    {
        TaskStaticConfig cfg{};
        cfg.model_type          = model.type;
        cfg.model_format        = model.format;
        cfg.model_cmds          = static_cast<uint32_t>(model.cmds);
        cfg.op_mode             = model.op_mode;
        cfg.cmd_offset          = model.rmap.offset;
        cfg.weight_offset       = model.weight.offset;
        cfg.input_size          = task->encoded_input_size();
        cfg.output_size         = model.last_output_size;
        cfg.last_output_offset  = model.last_output_offset;
        cfg.output_all_offset   = model.output_all_offset;
        cfg.memory_base         = model.rmap.base;
        for (int i = 0; i < MAX_CHECKPOINT_COUNT; ++i)
            cfg.datas[i] = model.checkpoints[i];
        auto ppu_it = _ppuBinaryOffsets.find(tId);
        cfg.custom_offset = (ppu_it != _ppuBinaryOffsets.end()) ? ppu_it->second : 0;

        std::unique_lock<std::mutex> lk(npuInferenceLock());
        taskStaticConfigs()[tId] = cfg;
    }

    return ret;
}

}  // namespace dxrt

#endif  // USE_VNPU
