/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/service_abstract_layer.h"
#include "service_layer_dynamic_ipc_bridge.hpp"
#include "dxrt/dynamic_ipc_endpoint.h"
#include "dxrt/service_util.h"
#include "dxrt/exception/exception.h"
#include "../dynamic_ipc/protocol/memory_type.hpp"
#include "../dynamic_ipc/shm/shared_memory_syscall_adapter.h"
#include "../dynamic_ipc/protocol/ipc_packet_client.hpp"
#include <cerrno>
#include <cstring>
#include <sstream>

namespace dxrt
{
namespace {

constexpr int kDynamicIpcEndpointRetryRounds = 3;

struct DynamicIpcState
{
    ServiceLayerDynamicIpcClient *client{nullptr};
    std::string endpoint;
    bool connected{false};
};

std::string ToHexU64(uint64_t value)
{
    std::ostringstream oss;
    oss << std::hex << value;
    return oss.str();
}

}  // namespace

void ServiceLayerDynamicIpcClientDeleter::operator()(ServiceLayerDynamicIpcClient *client) const
{
    DestroyServiceLayerDynamicIpcClient(client);
}

// ServiceLayer --------------------------------------------------
ServiceLayer::ServiceLayer()
{
}

void ServiceLayer::EnsureDynamicIPCConnected()
{
    if (_dynamicIpcConnected)
    {
        return;
    }

    const auto endpoints = GetDynamicIpcEndpointCandidates();
    std::string triedEndpoints;
    std::string connectedEndpoint;
    int attemptCount = 0;

    for (int round = 0; round < kDynamicIpcEndpointRetryRounds && _dynamicIpcClient == nullptr; ++round)
    {
        for (const auto &endpoint : endpoints)
        {
            ++attemptCount;
            if (!triedEndpoints.empty())
            {
                triedEndpoints += ", ";
            }
            triedEndpoints += GetDynamicIpcEndpointForLog(endpoint);

            auto dynamicIpcClient = CreateServiceLayerDynamicIpcClient(endpoint);
            if (dynamicIpcClient != nullptr)
            {
                _dynamicIpcClient.reset(dynamicIpcClient.release());
                connectedEndpoint = endpoint;
                break;
            }
        }
    }

    if (_dynamicIpcClient == nullptr)
    {
        throw dxrt::ServiceIOException(
            EXCEPTION_MESSAGE(
                "Failed to connect dynamic IPC endpoints after " + std::to_string(attemptCount)
                + " attempts: " + triedEndpoints));
    }

    LOG_DXRT_I_DBG << "ServiceLayer connected dynamic IPC endpoint: "
                   << GetDynamicIpcEndpointForLog(connectedEndpoint) << std::endl;

    ServiceLayerDynamicIpcSetInferenceResponseHandler(
        _dynamicIpcClient.get(),
        [this](int deviceId, const dxrt_response_t &response) {
            DispatchInferenceResponse(deviceId, response);
        });

    ServiceLayerDynamicIpcSetThrottleNotificationHandler(
        _dynamicIpcClient.get(),
        [this](int deviceId, int npuId, const dx_pcie_dev_ntfy_throt_t &throtInfo) {
            LOG_DXRT_DBG << "ServiceLayer throttle notification: deviceId=" << deviceId
                         << ", npuId=" << npuId
                         << ", ntfy_code=" << throtInfo.ntfy_code
                         << ", npu_id=" << throtInfo.npu_id << std::endl;
            DispatchThrottleNotification(deviceId, throtInfo);
        });

    ServiceLayerDynamicIpcSetErrorNotificationHandler(
        _dynamicIpcClient.get(),
        [this](int deviceId, dxrt_server_err_t serverErr, const dx_pcie_dev_err_t &errorInfo) {
            LOG_DXRT_ERR("ServiceLayer error notification: deviceId=" + std::to_string(deviceId) +
            ", serverErr=" + std::to_string(static_cast<long>(serverErr)) +
                ", err_code=" + std::to_string(errorInfo.err_code) +
                ", npu_id=" + std::to_string(errorInfo.npu_id));
            DispatchErrorNotification(deviceId, serverErr, static_cast<int>(errorInfo.err_code), errorInfo);
        });

    ServiceLayerDynamicIpcSetRecoveryStartNotificationHandler(
        _dynamicIpcClient.get(),
        [this](int deviceId, uint32_t recoveryId) {
            LOG_DXRT_ERR(
                "ServiceLayer recovery notification: deviceId=" + std::to_string(deviceId) +
                ", recoveryId=" + std::to_string(recoveryId));
            SignalStoppedDmaToWaitRecovery(deviceId, recoveryId);
        });

    _dynamicIpcConnected = true;
}

void ServiceLayer::HandleInferenceAcc(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut)
{
    // IPC uses SOCK_STREAM; serialize blocking InferenceRequest calls to avoid
    // packet coalescing across concurrent sender threads.
    // std::lock_guard<std::mutex> lock(_ipcMutex);
    HandleInferenceAccLocked(acc, deviceId, responseOut);
}

void ServiceLayer::HandleInferenceAccLocked(const dxrt_request_acc_t &acc, int deviceId, dxrt_response_t *responseOut)
{
    EnsureDynamicIPCConnected();

    if (_dynamicIpcClient == nullptr)
    {
        LOG_DXRT_ERR("ServiceLayer::HandleInferenceAcc skipped: dynamic IPC client is null");
        return;
    }

    const int rc = ServiceLayerDynamicIpcInferenceRequest(
        _dynamicIpcClient.get(),
        deviceId,
        static_cast<int>(acc.task_id),
        acc,
        responseOut);

    if (rc != 0)
    {
        LOG_DXRT_ERR("ServiceLayer::HandleInferenceAcc IPC failed: deviceId="
            + std::to_string(deviceId) + ", rc=" + std::to_string(rc));
    }
}

void ServiceLayer::SignalDeviceReset(int id)
{
    std::ignore = id;
}

SharedMemoryInfo ServiceLayer::AllocateInfo(int deviceId, int taskId, MemoryType memoryType, uint64_t size)
{
    // std::lock_guard<std::mutex> lock(_ipcMutex);
    return AllocateInfoLocked(deviceId, taskId, memoryType, size);
}

SharedMemoryInfo ServiceLayer::AllocateInfoLocked(int deviceId, int taskId, MemoryType memoryType, uint64_t size)
{
    EnsureDynamicIPCConnected();

    SharedMemoryInfo info{};
    const int rc = ServiceLayerDynamicIpcAllocateInfo(
        _dynamicIpcClient.get(),
        deviceId,
        taskId,
        memoryType,
        static_cast<uint32_t>(size),
        &info);
    if (rc != 0)
    {
        info.set_phys_addr(static_cast<uint64_t>(-1));
        info.fd = -1;
        info.block_id = -1;
        info.ptr = nullptr;
    }

    return info;
}

void ServiceLayer::DeAllocateInfo(int deviceId, const SharedMemoryInfo& info)
{
    // std::lock_guard<std::mutex> lock(_ipcMutex);
    DeAllocateInfoLocked(deviceId, info);
}

void ServiceLayer::DeAllocateInfoLocked(int deviceId, const SharedMemoryInfo &info)
{
    EnsureDynamicIPCConnected();

    SharedMemoryInfo infoCopy = info;
    (void)ServiceLayerDynamicIpcDeallocateInfo(_dynamicIpcClient.get(), deviceId, &infoCopy);
}

void ServiceLayer::SignalEndJobs(int id)
{
    std::ignore = id;
}

void ServiceLayer::CheckServiceRunning()
{
}

bool ServiceLayer::isRunOnService() const { return true; }

void ServiceLayer::RegisterDeviceCore(DeviceCore *core) { std::ignore = core; }

void ServiceLayer::RegisterInferenceResponseHandler(
    int deviceId,
    std::function<void(const dxrt_response_t &)> handler)
{
    std::lock_guard<std::mutex> lock(_responseHandlerMutex);
    if (handler)
    {
        _inferenceResponseHandlers[deviceId] = std::move(handler);
        return;
    }

    _inferenceResponseHandlers.erase(deviceId);
}

void ServiceLayer::RegisterErrorHandler(
    int deviceId,
    std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler)
{
    std::lock_guard<std::mutex> lock(_errorHandlerMutex);
    if (handler)
    {
        _errorHandlers[deviceId] = std::move(handler);
        return;
    }

    _errorHandlers.erase(deviceId);
}

void ServiceLayer::RegisterThrottleHandler(
    int deviceId,
    std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler)
{
    std::lock_guard<std::mutex> lock(_throttleHandlerMutex);
    if (handler)
    {
        _throttleHandlers[deviceId] = std::move(handler);
        return;
    }

    _throttleHandlers.erase(deviceId);
}

void ServiceLayer::DispatchInferenceResponse(int deviceId, const dxrt_response_t &response)
{
    std::function<void(const dxrt_response_t &)> handler;
    {
        std::lock_guard<std::mutex> lock(_responseHandlerMutex);
        const auto it = _inferenceResponseHandlers.find(deviceId);
        if (it != _inferenceResponseHandlers.end())
        {
            handler = it->second;
        }
    }

    if (handler)
    {
        handler(response);
        return;
    }

    LOG_DXRT_ERR("ServiceLayer dropped DynamicIPC inference response: deviceId="
        + std::to_string(deviceId) + ", req_id=" + std::to_string(response.req_id));
}

void ServiceLayer::DispatchErrorNotification(
    int deviceId,
    dxrt_server_err_t err,
    int value,
    const dx_pcie_dev_err_t &errorInfo)
{
    std::function<void(dxrt_server_err_t, int, const dx_pcie_dev_err_t &)> handler;
    {
        std::lock_guard<std::mutex> lock(_errorHandlerMutex);
        const auto it = _errorHandlers.find(deviceId);
        if (it != _errorHandlers.end())
        {
            handler = it->second;
        }
    }

    if (handler)
    {
        handler(err, value, errorInfo);
    }
}

void ServiceLayer::DispatchThrottleNotification(int deviceId, const dx_pcie_dev_ntfy_throt_t &throtInfo)
{
    std::function<void(const dx_pcie_dev_ntfy_throt_t &)> handler;
    {
        std::lock_guard<std::mutex> lock(_throttleHandlerMutex);
        const auto it = _throttleHandlers.find(deviceId);
        if (it != _throttleHandlers.end())
        {
            handler = it->second;
        }
    }

    if (handler)
    {
        handler(throtInfo);
    }
}

void ServiceLayer::SignalTaskInit(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config)
{
    // std::lock_guard<std::mutex> lock(_ipcMutex);
    SignalTaskInitLocked(deviceId, taskId, bound, modelMemorySize, config);
}

void ServiceLayer::SignalTaskInitLocked(int deviceId, int taskId, npu_bound_op bound, uint64_t modelMemorySize, const TaskStaticConfig &config)
{
    EnsureDynamicIPCConnected();

    const int rc = ServiceLayerDynamicIpcTaskInit(
        _dynamicIpcClient.get(),
        deviceId,
        taskId,
        static_cast<int>(bound),
        modelMemorySize,
        config);
    if (rc != 0)
    {
        throw dxrt::ServiceIOException(EXCEPTION_MESSAGE("Dynamic IPC task init failed"));
    }
}

void ServiceLayer::SignalTaskDeInit(int deviceId, int taskId, npu_bound_op bound)
{
    // std::lock_guard<std::mutex> lock(_ipcMutex);
    SignalTaskDeInitLocked(deviceId, taskId, bound);
}

void ServiceLayer::SignalTaskDeInitLocked(int deviceId, int taskId, npu_bound_op bound)
{
    EnsureDynamicIPCConnected();

    const int rc = ServiceLayerDynamicIpcTaskDeInit(
        _dynamicIpcClient.get(),
        deviceId,
        taskId,
        static_cast<int>(bound));
    if (rc != 0)
    {
        throw dxrt::ServiceIOException(EXCEPTION_MESSAGE("Dynamic IPC task deinit failed"));
    }
}

void ServiceLayer::CommitMemory(const SharedMemoryInfo &info)
{
    if (info.ptr == nullptr || info.size == 0)
    {
        return;
    }

    if (!dxrt::shm::SharedMemorySyscallAdapter::SyncMemory(info.ptr, static_cast<size_t>(info.size)))
    {
        LOG_DXRT_ERR(
            "ServiceLayer::CommitMemory SyncMemory failed: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size) +
            ", errno=" + std::to_string(errno) +
            ", reason=" + std::string(std::strerror(errno)));
    }
}

int ServiceLayer::DMAWrite(SharedMemoryView view)
{
    if (view.hostPtr() == nullptr || !view.isValid())
    {
        errno = EINVAL;
        return -1;
    }

    if (view.size == 0)
    {
        return 0;
    }

    // block_id is 1-based: valid allocations have block_id >= 1.
    if (view.info.fd < 0 || view.info.ptr == nullptr || view.info.block_id <= 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMAWrite requires a registered shared memory block "
            "(block_id=" + std::to_string(view.info.block_id) + ", fd=" + std::to_string(view.info.fd) + ")");
        errno = EINVAL;
        return -1;
    }

    // std::lock_guard<std::mutex> lock(_ipcMutex);
    return DMAWriteLocked(view);
}

int ServiceLayer::DMAWriteLocked(SharedMemoryView view)
{
    EnsureDynamicIPCConnected();

    if (_dynamicIpcClient == nullptr)
    {
        LOG_DXRT_ERR("ServiceLayer::DMAWrite IPC client is null");
        return -1;
    }

    const int deviceId = view.info.deviceid;

    dxrt::shm::SharedMemorySyscallAdapter::SyncMemory(view.hostPtr(), static_cast<size_t>(view.size));

    const int rc = ServiceLayerDynamicIpcDMAWrite(
        _dynamicIpcClient.get(),
        deviceId,
        view.info.block_id,
        view.offset,
        view.size);

    if (rc != 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMAWrite request failed: "
            "deviceId=" + std::to_string(deviceId) +
            ", block_id=" + std::to_string(view.info.block_id) +
            ", offset=" + std::to_string(view.offset) +
            ", size=" + std::to_string(view.size) +
            ", rc=" + std::to_string(rc));
    }
    return rc;
}

void ServiceLayer::InvalidateMemory(const SharedMemoryInfo &info)
{
    if (info.ptr == nullptr || info.size == 0)
    {
        return;
    }

    if (!dxrt::shm::SharedMemorySyscallAdapter::InvalidateMemory(info.ptr, static_cast<size_t>(info.size)))
    {
        LOG_DXRT_ERR(
            "ServiceLayer::InvalidateMemory InvalidateMemory failed: "
            "block_id=" + std::to_string(info.block_id) +
            ", deviceid=" + std::to_string(info.deviceid) +
            ", phys_addr=0x" + ToHexU64(info.phys_addr()) +
            ", size=" + std::to_string(info.size) +
            ", errno=" + std::to_string(errno) +
            ", reason=" + std::string(std::strerror(errno)));
    }
}

int ServiceLayer::DMARead(SharedMemoryView view)
{
    if (view.hostPtr() == nullptr || !view.isValid())
    {
        errno = EINVAL;
        return -1;
    }

    if (view.size == 0)
    {
        return 0;
    }

    // block_id is 1-based: valid allocations have block_id >= 1.
    if (view.info.fd < 0 || view.info.ptr == nullptr || view.info.block_id <= 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMARead requires a registered shared memory block "
            "(block_id=" + std::to_string(view.info.block_id) + ", fd=" + std::to_string(view.info.fd) + ")");
        errno = EINVAL;
        return -1;
    }

    // std::lock_guard<std::mutex> lock(_ipcMutex);
    return DMAReadLocked(view);
}

int ServiceLayer::DMAReadLocked(SharedMemoryView view)
{
    EnsureDynamicIPCConnected();

    if (_dynamicIpcClient == nullptr)
    {
        LOG_DXRT_ERR("ServiceLayer::DMARead IPC client is null");
        return -1;
    }

    const int deviceId = view.info.deviceid;

    const int rc = ServiceLayerDynamicIpcDMARead(
        _dynamicIpcClient.get(),
        deviceId,
        view.info.block_id,
        view.offset,
        view.size);

    if (rc == 0)
    {
        dxrt::shm::SharedMemorySyscallAdapter::InvalidateMemory(view.hostPtr(), static_cast<size_t>(view.size));
    }
    else
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMARead request failed: "
            "deviceId=" + std::to_string(deviceId) +
            ", block_id=" + std::to_string(view.info.block_id) +
            ", offset=" + std::to_string(view.offset) +
            ", size=" + std::to_string(view.size) +
            ", rc=" + std::to_string(rc));
    }
    return rc;
}

int ServiceLayer::DMAReadWithFaultInjection(SharedMemoryView view)
{
    if (view.hostPtr() == nullptr || !view.isValid())
    {
        errno = EINVAL;
        return -1;
    }

    if (view.size == 0)
    {
        return 0;
    }

    // block_id is 1-based: valid allocations have block_id >= 1.
    if (view.info.fd < 0 || view.info.ptr == nullptr || view.info.block_id <= 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMAReadWithFaultInjection requires a registered shared memory block "
            "(block_id=" + std::to_string(view.info.block_id) + ", fd=" + std::to_string(view.info.fd) + ")");
        errno = EINVAL;
        return -1;
    }

    // std::lock_guard<std::mutex> lock(_ipcMutex);
    return DMAReadWithFaultInjectionLocked(view);
}

int ServiceLayer::DMAReadWithFaultInjectionLocked(SharedMemoryView view)
{
    EnsureDynamicIPCConnected();

    if (_dynamicIpcClient == nullptr)
    {
        LOG_DXRT_ERR("ServiceLayer::DMAReadWithFaultInjection IPC client is null");
        return -1;
    }

    const int deviceId = view.info.deviceid;

    const int rc = ServiceLayerDynamicIPCDMAReadWithFaultInjection(
        _dynamicIpcClient.get(),
        deviceId,
        view.info.block_id,
        view.offset,
        view.size);

    if (rc == 0)
    {
        dxrt::shm::SharedMemorySyscallAdapter::InvalidateMemory(view.hostPtr(), static_cast<size_t>(view.size));
    }
    else
    {
        LOG_DXRT_ERR(
            "ServiceLayer::DMAReadWithFaultInjection request failed: "
            "deviceId=" + std::to_string(deviceId) +
            ", block_id=" + std::to_string(view.info.block_id) +
            ", offset=" + std::to_string(view.offset) +
            ", size=" + std::to_string(view.size) +
            ", rc=" + std::to_string(rc));
    }
    return rc;
}
void ServiceLayer::SignalStoppedDmaToWaitRecovery(int deviceId)
{
    SignalStoppedDmaToWaitRecovery(deviceId, 0);
}

void ServiceLayer::SignalStoppedDmaToWaitRecovery(int deviceId, uint32_t recoveryId)
{
    if (_dynamicIpcClient == nullptr)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::SignalStoppedDmaToWaitRecovery skipped: dynamic IPC client is null");
        return;
    }

    const int rc = ServiceLayerDynamicIpcSendRecoveryStartAck(
        _dynamicIpcClient.get(),
        deviceId,
        recoveryId,
        0);
    if (rc != 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayer::SignalStoppedDmaToWaitRecovery ACK failed: deviceId=" +
            std::to_string(deviceId) +
            ", recoveryId=" + std::to_string(recoveryId) +
            ", rc=" + std::to_string(rc));
    }
}

}  // namespace dxrt
