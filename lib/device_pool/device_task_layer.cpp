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
#include <fstream>
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
    return _serviceLayer->Allocate(id(), size);
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

int64_t DeviceTaskLayer::AllocateFromCache(int64_t size, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " allocate from cache: " << size << " bytes" << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        return _npuMemoryCacheManager.getNpuMemoryCache(taskId);
    }
    else
    {
        return Allocate(size);
    }
}


void DeviceTaskLayer::Deallocate_npuBuf(int64_t addr, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " deallocate: " << std::showbase << std::hex << addr << std::dec << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.returnNpuMemoryCache(taskId, addr);
    }
    else
    {
        Deallocate(addr);
    }
}

[[noreturn]] void DeviceTaskLayer::ProcessErrorFromService(dxrt_server_err_t err, int value)
{
    std::cout << "============================================================" << std::endl;
    std::cout << "[DXRT] Fatal error from service on device " << id() << std::endl;
    std::cout << " ** Reason : " <<  err <<
        " (err_code=" << value << ")" << std::endl;
    std::cout << " ** Device recovery was performed by the service." << std::endl;
    std::cout << " ** This application must exit and restart to reload models." << std::endl;
    std::cout << "============================================================" << std::endl;

    core()->ShowPCIEDetails();

    // GCOV_EXCL_START
    // Attempt to read detailed error info from temp file (written by service) for debug purposes
    const std::string dumpPath = dxrt::getDxrtErrorDumpReadPath(id());
    std::ifstream ifs(dumpPath);
    if (ifs) {
        std::string line;
        while (std::getline(ifs, line)) {
            std::cout << line << std::endl;
        }
        ifs.close();
    }
    else
    {
        LOG_DXRT_ERR("No additional error details available. refer " + dumpPath + " for more info.");
    }
    // GCOV_EXCL_STOP

    // Application must terminate — DDR content may have been lost due to
    // PCIe SBR during recovery. Models need to be reloaded from scratch.
    // Use _exit() to avoid hanging on atexit handlers or thread joins.
    std::_Exit(EXIT_FAILURE);
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
    // Prevent duplicate recovery via epoch check
    uint32_t currentEpoch = _recoveryEpoch.load(std::memory_order_acquire);

    {
        std::lock_guard<std::mutex> lk(_recoveryMutex);
        // Double-check epoch under lock to prevent race
        if (_recoveryEpoch.load(std::memory_order_acquire) != currentEpoch)
        {
            LOG_DXRT_INFO("Recovery already performed by another thread (epoch advanced)");
            return 0;
        }

        if (_recoveryInProgress.load(std::memory_order_acquire))
        {
            LOG_DXRT_INFO("Recovery already in progress, skipping duplicate");
            return 0;
        }
        _recoveryInProgress.store(true, std::memory_order_release);
    }

    LOG_DXRT_INFO("DMA Abort Recovery: Blocking device " << id() << " and waiting for in-flight DMA");

    // 1. Block the device to prevent new inference requests
    DXRT_ASSERT(false, "You Need to Restart Daemon and Applications due to device error. Please check error message above for details.");

    // 2. Wait for in-flight DMA transfers to drain (with timeout)
    static constexpr uint32_t RECOVERY_WAIT_TIMEOUT_MS = 5000;
    waitForInflightDmaCompletion(RECOVERY_WAIT_TIMEOUT_MS);

    // 3. Call DXRT_CMD_RECOVERY ioctl
    LOG_DXRT_INFO("DMA Abort Recovery: Issuing DXRT_CMD_RECOVERY for device " << id());
    int ret = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
    if (ret < 0)
    {
        LOG_DXRT_ERR("DXRT_CMD_RECOVERY ioctl failed for device " << id()
            << ", ret=" << ret << ", errno=" << errno);
        _recoveryInProgress.store(false, std::memory_order_release);
        return ret;
    }

    // 4. Increment recovery epoch and clear state
    _recoveryEpoch.fetch_add(1, std::memory_order_release);

    // 5. Unblock device and signal recovery complete
    unblock();
    _recoveryInProgress.store(false, std::memory_order_release);
    _recoveryCondVar.notify_all();

    LOG_DXRT_INFO("DMA Abort Recovery: Completed for device " << id()
        << ", epoch=" << _recoveryEpoch.load());

    // 6. Reload models if needed (PCIe SBR may have cleared DDR)
    reloadModelsIfNeeded();

    return 0;
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
