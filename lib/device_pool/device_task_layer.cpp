/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


// Core task layer base implementation (common utilities only)
#include "dxrt/device_task_layer.h"

#include <chrono>
#include <signal.h>
#include <string>
#include <thread>

#include "dxrt/common.h"
#include "dxrt/device_core.h"
#include "dxrt/filesys_support.h"
#include "dxrt/request_data.h"
#include "dxrt/request_response_class.h"
#include "dxrt/task.h"

#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {

DeviceTaskLayer::DeviceTaskLayer(std::shared_ptr<DeviceCore> core, std::shared_ptr<ServiceLayerInterface> service_interface)
: _core(core), _serviceLayer(service_interface), _npuMemoryCacheManager(this)
{
    // Default no-op callback - actual callback is set later via RegisterCallback()
    _processResponseHandler = [](int deviceId, int reqId, const dxrt_response_t *response){
        RequestResponse::ProcessByData(reqId, *response, deviceId);
    };
}

int DeviceTaskLayer::load() const
{
    return _load.load();
}

void DeviceTaskLayer::pick()
{
    ++_load;
}

int DeviceTaskLayer::infCnt() const
{
    return _inferenceCnt;
}


int64_t DeviceTaskLayer::Allocate(uint64_t size) const
{
    return static_cast<int64_t>(_serviceLayer->Allocate(id(), size));
}

int64_t DeviceTaskLayer::AllocateWithTaskId(int taskId, uint64_t size) const
{
    return static_cast<int64_t>(_serviceLayer->Allocate(id(), taskId, MemoryType::Normal, size));
}

SharedMemoryInfo DeviceTaskLayer::AllocateInfo(int taskId, MemoryType type, uint64_t size) const
{
    return _serviceLayer->AllocateInfo(id(), taskId, type, size);
}

void DeviceTaskLayer::Deallocate(uint64_t addr) const
{
    _serviceLayer->DeAllocate(id(), addr);
}

void DeviceTaskLayer::CallBack()
{
    // Decrement load atomically
    _load--;
    _inferenceCnt++;

    // Notify device pool that this device is now available
    if (_onCompleteInferenceHandler)
    {
        _onCompleteInferenceHandler();
    }
}

void DeviceTaskLayer::RegisterCallback(std::function<void()> f)
{
    _onCompleteInferenceHandler = std::move(f);
}

static constexpr int TERMINATE_NUM_CHANNEL = 3;

void DeviceTaskLayer::Terminate()
{
#ifdef __linux__
    core()->Close();
#else
    dxrt_response_t data;
    memset(static_cast<void*>(&data), 0x00, sizeof(dxrt_response_t));
    std::ignore = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE_EVENT, &data);
    for (int i = 0; i < TERMINATE_NUM_CHANNEL; i++)
    {
        data.req_id = i;
        int ret = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, &data);
        std::ignore = ret;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
#endif
}

NpuMemoryCacheSlice DeviceTaskLayer::AllocateFromCache(int64_t size, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " allocate from cache: " << size << " bytes" << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        return _npuMemoryCacheManager.getNpuMemoryCache(taskId);
    }
    else
    {
        SharedMemoryInfo info = AllocateInfo(taskId, MemoryType::Input_output, static_cast<uint64_t>(size));
        NpuMemoryCacheSlice slice;
        slice.view.info   = info;
        slice.view.offset = 0;
        slice.view.size   = static_cast<uint64_t>(size);
        return slice;
    }
}


void DeviceTaskLayer::DeallocateInfo(const SharedMemoryInfo &info) const
{
    _serviceLayer->DeAllocateInfo(id(), info);
}


void DeviceTaskLayer::Deallocate_npuBuf(const NpuMemoryCacheSlice &slice, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " deallocate: start="
                 << std::showbase << std::hex << slice.view.offset << std::dec
                 << ", size=" << slice.view.size << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.returnNpuMemoryCache(taskId, slice);
    }
    else
    {
        DeallocateInfo(slice.view.info);
    }
}

[[noreturn]] void DeviceTaskLayer::ProcessErrorFromService(
    dxrt_server_err_t err,
    int value,
    const dx_pcie_dev_err_t *errorInfo)
{
    std::cout << "============================================================" << std::endl;
    std::cout << "[DXRT] Fatal error from service on device " << id() << std::endl;
    std::cout << " ** Reason : " <<  err <<
        " (err_code=" << value << ")" << std::endl;
    if (errorInfo != nullptr)
    {
        std::cout << " ** Detailed error payload from service:" << std::endl;
        std::cout << "    - npu_id=" << errorInfo->npu_id
                  << ", err_code=" << errorInfo->err_code
                  << ", cmd_num=" << errorInfo->cmd_num
                  << ", last_cmd=" << errorInfo->last_cmd
                  << ", busy=" << errorInfo->busy << std::endl;
        std::cout << "    - PCIe BDF="
                  << static_cast<int>(errorInfo->bus) << ":"
                  << static_cast<int>(errorInfo->dev) << "."
                  << static_cast<int>(errorInfo->func)
                  << ", ltssm=" << errorInfo->ltssm
                  << ", speed=" << errorInfo->speed
                  << ", width=" << errorInfo->width << std::endl;
        std::cout << "    - DMA WR status=["
                  << errorInfo->dma_wr_ch_sts[0] << ", "
                  << errorInfo->dma_wr_ch_sts[1] << ", "
                  << errorInfo->dma_wr_ch_sts[2] << ", "
                  << errorInfo->dma_wr_ch_sts[3] << "]" << std::endl;
        std::cout << "    - DMA RD status=["
                  << errorInfo->dma_rd_ch_sts[0] << ", "
                  << errorInfo->dma_rd_ch_sts[1] << ", "
                  << errorInfo->dma_rd_ch_sts[2] << ", "
                  << errorInfo->dma_rd_ch_sts[3] << "]" << std::endl;
    }
    std::cout << " ** Device recovery was performed by the service." << std::endl;
    std::cout << " ** This application must exit and restart to reload models." << std::endl;
    std::cout << "============================================================" << std::endl;

    core()->ShowPCIEDetails();

    // Application must terminate — DDR content may have been lost due to
    // PCIe SBR during recovery. Models need to be reloaded from scratch.
    // Use _exit() to avoid hanging on atexit handlers or thread joins.
    std::_Exit(EXIT_FAILURE);
}

void DeviceTaskLayer::ProcessThrottleFromService(const dx_pcie_dev_ntfy_throt_t &throtInfo)
{
    std::ignore = throtInfo;
}

void DeviceTaskLayer::waitForInflightDmaCompletion(uint32_t timeoutMs)   // NOSONAR
{
    // Wait for in-flight DMA transfers to complete (driver returns -EIO for aborted channels).
    // Worker threads that are blocked on DMA will receive errors and decrement load.
    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::milliseconds(timeoutMs);

    while (_load.load(std::memory_order_acquire) > 0)
    {
        auto elapsed = std::chrono::steady_clock::now() - start;
        if (elapsed >= timeout)
        {
            LOG_DXRT_WARN("waitForInflightDmaCompletion: Timeout after "
                << timeoutMs << "ms, remaining load=" << _load.load());
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int DeviceTaskLayer::triggerRecovery()
{
    LOG_DXRT_DBG << "DeviceTaskLayer::triggerRecovery start for device " << id() << std::endl;

    // Prevent duplicate recovery via epoch check
    uint32_t currentEpoch = _recoveryEpoch.load(std::memory_order_acquire);

    {
        std::lock_guard<std::mutex> lk(_recoveryMutex);
        // Double-check epoch under lock to prevent race
        if (_recoveryEpoch.load(std::memory_order_acquire) != currentEpoch)
        {
            LOG_DXRT_DBG << "Recovery already performed by another thread (epoch advanced)" << std::endl;
            return 0;
        }

        if (_recoveryInProgress.load(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << "Recovery already in progress, skipping duplicate" << std::endl;
            return 0;
        }
        _recoveryInProgress.store(true, std::memory_order_release);
    }

    LOG_DXRT_DBG << "DMA Abort Recovery: Blocking device " << id()
                 << " and waiting for in-flight DMA" << std::endl;

    // Step 1+2: Stop new DMA requests via service layer and drain in-flight ones.
    LOG_DXRT_DBG << "Calling serviceLayer()->PauseForRecovery(" << id() << ")" << std::endl;
    serviceLayer()->PauseForRecovery(id());

    // Step 3: Call DXRT_CMD_RECOVERY ioctl
    LOG_DXRT_DBG << "DMA Abort Recovery: Issuing DXRT_CMD_RECOVERY for device "
                 << id() << std::endl;
    int ret = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
    if (ret < 0)
    {
        LOG_DXRT_ERR("DXRT_CMD_RECOVERY ioctl failed for device " << id()
            << ", ret=" << ret << ", errno=" << errno);
        _recoveryInProgress.store(false, std::memory_order_release);
        serviceLayer()->OnRecoveryFailed(id());  // [[noreturn]]
    }
    LOG_DXRT_DBG << "DXRT_CMD_RECOVERY returned successfully, ret=" << ret << std::endl;

    // Step 4: Increment recovery epoch and clear state
    _recoveryEpoch.fetch_add(1, std::memory_order_release);

    // Step 5: Unblock device and signal recovery complete
    unblock();
    _recoveryInProgress.store(false, std::memory_order_release);
    _recoveryCondVar.notify_all();

    LOG_DXRT_DBG << "DMA Abort Recovery: Completed for device " << id()
                 << ", epoch=" << _recoveryEpoch.load() << std::endl;

    // Step 6: Notify service layer to resume and reload models if needed
    LOG_DXRT_DBG << "Calling serviceLayer()->ResumeAfterRecovery(" << id() << ")" << std::endl;
    serviceLayer()->ResumeAfterRecovery(id());
    LOG_DXRT_DBG << "Reloading models if needed" << std::endl;
    reloadModelsIfNeeded();

    LOG_DXRT_DBG << "DeviceTaskLayer::triggerRecovery complete for device " << id() << std::endl;

    return 0;
}

void DeviceTaskLayer::SignalStoppedDmaToWaitRecovery()
{
    _serviceLayer->SignalStoppedDmaToWaitRecovery(id());
}

[[noreturn]]
void DeviceTaskLayer::OnRecoveryFailed(int deviceId)
{
    LOG_DXRT_ERR("Recovery failed for device " << deviceId << ". Terminating.");
    DXRT_ASSERT(false, "Recovery failed for device " + std::to_string(deviceId));
}

void DeviceTaskLayer::reloadModelsIfNeeded()
{
    // After recovery (especially if PCIe SBR was performed), DDR content may be lost.
    // Re-write RMAP and weight data for all registered models.
    for (auto& pair : _npuModel)   // NOSONAR
    {
        auto& model = pair.second;   // NOSONAR
        LOG_DXRT_INFO("Recovery: Reloading model for task " << pair.first
            << " on device " << id());
        if (model.rmap.data != 0 && model.rmap.size > 0)
        {
            int ret = core()->Write(model.rmap, 3);
            if (ret != 0)
            {
                LOG_DXRT_ERR("Recovery: Failed to reload RMAP for task " << pair.first
                    << " on device " << id());
            }
        }
        if (model.weight.data != 0 && model.weight.size > 0)
        {
            int ret = core()->Write(model.weight, 3);
            if (ret != 0)
            {
                LOG_DXRT_ERR("Recovery: Failed to reload weight for task " << pair.first
                    << " on device " << id());
            }
        }
    }

    // Notify FW to resume
    uint32_t start = 1;
    core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_START, &start, sizeof(start));
}

}  // namespace dxrt
