/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

// Non-VNPU (PCIe) implementation of AccDeviceTaskLayer::RegisterTask.
// For the VNPU (CMA/RK SoC) implementation see acc_device_task_layer_register_vnpu.cpp.

#ifndef USE_VNPU

#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <limits>
#include <sstream>
#include "dxrt/common.h"
#include "dxrt/device_task_layer.h"
#include "inference_context.h"
#include "dxrt/task_data.h"
#include "dxrt/util.h"
#include "../resource/log_messages.h"
#include "dxrt/safe_cast.h"

namespace dxrt {

namespace {

static constexpr uint64_t ERROR_ALLOC = (std::numeric_limits<uint64_t>::max)();

SharedMemoryInfo MakeDMAMemoryView(const SharedMemoryInfo &deviceMemory, const void *ptr, uint64_t size)
{
    SharedMemoryInfo info = deviceMemory;
    info.ptr = const_cast<void *>(ptr);
    info.size = size;
    return info;
}

}  // namespace

int AccDeviceTaskLayer::RegisterTask(TaskData* task)
{
    auto dmaPass = _dmaStopGate.WaitIfStopped();
    std::ignore = dmaPass;
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

    ModelMemoryInfos& memInfos = modelMemoryInfos()[tId];

    memInfos.weightMemInfo = serviceLayer()->AllocateInfo(id(), tId, MemoryType::Model_weight, model.weight.size);
    memInfos.rmapMemInfo = serviceLayer()->AllocateInfo(id(), tId, MemoryType::Model_rmap, model.rmap.size);

    if (memInfos.rmapMemInfo.phys_addr_offset > memInfos.weightMemInfo.phys_addr_offset)
    {
        const auto temp_addr = memInfos.rmapMemInfo;
        memInfos.rmapMemInfo = serviceLayer()->AllocateInfo(id(), tId, MemoryType::Model_rmap, model.rmap.size);
        if (memInfos.rmapMemInfo.block_id <= 0)
        {
            LOG_DXRT_ERR("Failed to allocate rmap memory in NPU memory during address conflict resolution");
            if (memInfos.weightMemInfo.block_id > 0)
            {
                serviceLayer()->DeAllocateInfo(id(), memInfos.weightMemInfo);
            }
            if (temp_addr.block_id > 0)
            {
                serviceLayer()->DeAllocateInfo(id(), temp_addr);
            }
            return -1;
        }
        if (temp_addr.block_id > 0)
        {
            serviceLayer()->DeAllocateInfo(id(), temp_addr);
        }
    }

    // block_id is 1-based; 0 or negative means allocation failed.
    if (memInfos.rmapMemInfo.block_id <= 0)
    {
        if (memInfos.weightMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocateInfo(id(), memInfos.weightMemInfo);
        }
        LOG_DXRT_ERR("Failed to allocate rmap memory in NPU memory");
        return -1;
    }
    if (memInfos.weightMemInfo.block_id <= 0)
    {
        serviceLayer()->DeAllocateInfo(id(), memInfos.rmapMemInfo);
        LOG_DXRT_ERR("Failed to allocate weight memory in NPU memory");
        return -1;
    }

    model.weight.offset = static_cast<uint32_t>(memInfos.weightMemInfo.phys_addr_offset);
    model.rmap.offset = static_cast<uint32_t>(memInfos.rmapMemInfo.phys_addr_offset);

    // Store model with offsets set
    npuModelMap()[tId] = model;

    // Store memory info for later management

    auto rollbackTaskRegistration = [this, &memInfos, tId]() {
        auto ppu_offset_it = _ppuBinaryOffsets.find(tId);
        if (ppu_offset_it != _ppuBinaryOffsets.end())
        {
            if (memInfos.ppuMemInfo.block_id > 0)
            {
                serviceLayer()->DeAllocateInfo(id(), memInfos.ppuMemInfo);
            }
            _ppuBinaryOffsets.erase(ppu_offset_it);
        }
        _ppuBinaryData.erase(tId);

        if (memInfos.weightMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocateInfo(id(), memInfos.weightMemInfo);
            memInfos.weightMemInfo.block_id = 0;
        }
        if (memInfos.rmapMemInfo.block_id > 0)
        {
            serviceLayer()->DeAllocateInfo(id(), memInfos.rmapMemInfo);
            memInfos.rmapMemInfo.block_id = 0;
        }

        {
            std::unique_lock<std::mutex> lk(npuInferenceLock());
            npuModelMap().erase(tId);
        }

        _inputTensorFormats.erase(tId);
        _outputTensorFormats.erase(tId);
        if (memoryCacheManager().canGetCache(tId))
        {
            memoryCacheManager().unRegisterMemoryCache(tId);
        }
    };

    try
    {
        // Service mode: copy source data into the SHM block (ptr), then ask the service to DMA it
        //   to device memory. The service identifies the SHM block via block_id/fd.
        // NoService mode: AllocateInfo returns ptr=nullptr (no host mapping). Use MakeDMAMemoryView
        //   to pass (ptr=source data, phys_addr=NPU destination) directly to core->Write().
        if (serviceLayer()->isRunOnService())
        {
            std::memcpy(memInfos.rmapMemInfo.ptr,
                SafeCast::IntegerToPointer<const void *>(model.rmap.data),
                model.rmap.size);
            ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(memInfos.rmapMemInfo));
        }
        else
        {
            ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(
                MakeDMAMemoryView(memInfos.rmapMemInfo,
                    SafeCast::IntegerToPointer<const void *>(model.rmap.data),
                    model.rmap.size)));
        }
        DXRT_ASSERT(ret == 0, "failed to write model rmap parameters" + std::to_string(ret));

        if (serviceLayer()->isRunOnService())
        {
            std::memcpy(memInfos.weightMemInfo.ptr,
                SafeCast::IntegerToPointer<const void *>(model.weight.data),
                model.weight.size);
            ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(memInfos.weightMemInfo));
        }
        else
        {
            ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(
                MakeDMAMemoryView(memInfos.weightMemInfo,
                    SafeCast::IntegerToPointer<const void *>(model.weight.data),
                    model.weight.size)));
        }
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

                auto ppu_memory = serviceLayer()->AllocateInfo(id(), tId, MemoryType::Model_ppu_binary, ppu_binary_copy.size());
                if (ppu_memory.block_id <= 0)
                {
                    LOG_DXRT_ERR("Failed to allocate ppuMem memory in NPU memory");
                    rollbackTaskRegistration();
                    return -1;
                }

                ppu_mem.offset = static_cast<uint32_t>(ppu_memory.phys_addr_offset);
                ppu_mem.size = static_cast<uint32_t>(ppu_binary_copy.size());
                ppu_mem.data = SafeCast::PointerToInteger<const uint8_t*>(ppu_binary_copy.data());
                if (serviceLayer()->isRunOnService())
                {
                    std::memcpy(ppu_memory.ptr, ppu_binary_copy.data(), ppu_binary_copy.size());
                    ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(ppu_memory));
                }
                else
                {
                    ret = serviceLayer()->DMAWrite(SharedMemoryView::ofWhole(
                        MakeDMAMemoryView(ppu_memory, ppu_binary_copy.data(), ppu_binary_copy.size())));
                }
                DXRT_ASSERT(ret == 0, "failed to write PPU binary parameters" + std::to_string(ret));

                // Store PPU binary offset in device-specific map (not in TaskData to avoid conflicts)
                _ppuBinaryOffsets[tId] = ppu_mem.offset;

                // Store PPU memory info
                memInfos.ppuMemInfo = ppu_memory;

                LOG_DXRT_DBG << "Device " << id()
                            << " wrote PPU binary (device-specific copy): offset=0x"
                            << std::hex << ppu_mem.offset
                            << ", size=" << std::dec << ppu_mem.size << " bytes" << std::endl;
            }
        }

        // Verify (skip if size is 0)
        if (model.rmap.size > 0 && model.weight.size > 0)
        {
            auto verify = [this](const dxrt_meminfo_t& info, const SharedMemoryInfo &deviceInfo, const std::string& name) {
                if (info.size == 0)
                {
                    return 0;
                }

                // Service mode: DMARead writes into the SHM block (deviceInfo.ptr, identified by block_id).
                //   Do NOT modify ptr — the service uses block_id as the write target.
                // NoService mode: ptr is null after AllocateInfo; use a local read-back buffer.
                std::vector<uint8_t> noServiceReadBuf;
                SharedMemoryInfo readInfo = deviceInfo;
                if (!serviceLayer()->isRunOnService())
                {
                    noServiceReadBuf.resize(info.size);
                    readInfo.ptr = noServiceReadBuf.data();
                }
                const int readRc = serviceLayer()->DMARead(SharedMemoryView::ofWhole(readInfo));
                if (readRc == 0)
                {
                    const auto *src = static_cast<const uint8_t*>(SafeCast::IntegerToPointer<void*>(info.data));
                    const auto *dst = static_cast<const uint8_t*>(readInfo.ptr);
                    int cmpRet = std::memcmp(src, dst, info.size);
                    if (cmpRet != 0)
                    {
                        // Print first mismatch location and surrounding bytes
                        for (uint32_t i = 0; i < info.size; ++i)
                        {
                            if (src[i] != dst[i])
                            {
                                LOG_DXRT_ERR(name + " first mismatch at byte "
                                    + std::to_string(i)
                                    + ": src=0x" + ToHexString(src[i])
                                    + " dst=0x" + ToHexString(dst[i]));
                                // Print first 8 bytes of each
                                std::string srcStr;
                                std::string dstStr;
                                std::ostringstream srcOss;
                                std::ostringstream dstOss;
                                srcOss << std::hex << std::setfill('0');
                                dstOss << std::hex << std::setfill('0');
                                for (int j = 0; j < 8 && j < static_cast<int>(info.size); ++j)
                                {
                                    srcOss << std::setw(2) << static_cast<unsigned int>(src[j]);
                                    dstOss << std::setw(2) << static_cast<unsigned int>(dst[j]);
                                }
                                srcStr = srcOss.str();
                                dstStr = dstOss.str();
                                LOG_DXRT_ERR(name + " src[0..7]=" + srcStr + " dst[0..7]=" + dstStr);
                                break;
                            }
                        }
                    }
                    return cmpRet == 0 ? 0 : 1;
                }
                else
                {
                    LOG_DXRT_ERR("Failed to read back " + name + " for verification");
                }
                return 1;
            };

            int fail_count = 0;
            fail_count += verify(model.rmap, memInfos.rmapMemInfo, "rmap");
            fail_count += verify(model.weight, memInfos.weightMemInfo, "weight");

            DXRT_ASSERT(fail_count == 0, "failed to verify model parameters, fail count: " + std::to_string(fail_count));
        }
        else
        {
            LOG_DXRT_DBG << "Device " << id() << " skipping verify (rmap.size=" << model.rmap.size
                            << ", weight.size=" << model.weight.size << ")" << std::endl;
        }


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

        // int npu_cache_count equals to DXRT_TASK_MAX_LOAD
        int npu_cache_count = task->get_buffer_count();
        while (npu_cache_count > 0)
        {
            if (memoryCacheManager().registerMemoryCache(task->id(), block_size, npu_cache_count) == false)
            {
                npu_cache_count--;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // brief pause before retrying
            }
            else
            {
                memInfos.inputOutputMemInfos.push_back(memoryCacheManager().getBackingInfo(task->id()));
                break;
            }
        }
        if (npu_cache_count < 1)
        {
            LOG_DXRT_ERR("Failed to register memory cache for task " + std::to_string(task->id()));
            rollbackTaskRegistration();
            return -1;
        }
    }
    catch (...)
    {
        rollbackTaskRegistration();
        throw;
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
        cfg.memory_base         = model.rmap.base;  // 0 (service-ON) or mem_addr (service-OFF)
        for (int i = 0; i < MAX_CHECKPOINT_COUNT; ++i)
            cfg.datas[i] = model.checkpoints[i];
        // PPU binary offset: 0 when absent, otherwise set by the allocation above.
        auto ppu_it = _ppuBinaryOffsets.find(tId);
        cfg.custom_offset = (ppu_it != _ppuBinaryOffsets.end()) ? ppu_it->second : 0;

        std::unique_lock<std::mutex> lk(npuInferenceLock());
        taskStaticConfigs()[tId] = cfg;
    }

    return ret;
}

}  // namespace dxrt

#endif  // USE_VNPU
