/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "service_layer_dynamic_ipc_bridge.hpp"

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/dynamic_ipc_endpoint.h"

#include "../dynamic_ipc/protocol/ipc_packet_client.hpp"
#include "../dynamic_ipc/protocol/memory_type.hpp"
#include "../dynamic_ipc/shm/shared_memory_syscall_adapter.h"

#include <chrono>
#include <cerrno>
#include <cstring>
#include <memory>
#include <thread>
#include <utility>

namespace dxrt {

struct ServiceLayerDynamicIpcClient
{
    IPCPacketClient client;
    ThrottleNotificationCallback onThrottleNotification;
    ErrorNotificationCallback onErrorNotification;
    InferenceResponseCallback onInferenceResponse;
    RecoveryStartNotificationCallback onRecoveryStartNotification;

    ~ServiceLayerDynamicIpcClient()
    {
        client.close();
    }
};

IpcClientOwner CreateServiceLayerDynamicIpcClient(const std::string &endpoint)
{
    LOG_DXRT_DBG << "CreateServiceLayerDynamicIpcClient START: endpoint="
      << dxrt::GetDynamicIpcEndpointForLog(endpoint) << std::endl;
    auto runtime = MAKE_UNIQUE<ServiceLayerDynamicIpcClient>();
    ServiceLayerDynamicIpcClient *runtimeRaw = runtime.get();

    auto connectStart = std::chrono::high_resolution_clock::now();
    LOG_DXRT_DBG << "  IPC_CONNECT_START: endpoint=" << dxrt::GetDynamicIpcEndpointForLog(endpoint) << std::endl;
    if (runtimeRaw->client.connectToServer(endpoint, 5, 100) != 0)
    {
        auto connectEnd = std::chrono::high_resolution_clock::now();
        auto connectDuration = std::chrono::duration_cast<std::chrono::milliseconds>(connectEnd - connectStart);
        LOG_DXRT_ERR("IPC_CONNECT_FAILED: endpoint=" + dxrt::GetDynamicIpcEndpointForLog(endpoint)
                     + " elapsed_ms=" + std::to_string(connectDuration.count())
                     + " errno=" + std::to_string(errno));
        return IpcClientOwner(nullptr, &DestroyServiceLayerDynamicIpcClient);
    }

    auto connectEnd = std::chrono::high_resolution_clock::now();
    auto connectDuration = std::chrono::duration_cast<std::chrono::milliseconds>(connectEnd - connectStart);
    LOG_DXRT_DBG << "  IPC_CONNECT_OK: endpoint=" << dxrt::GetDynamicIpcEndpointForLog(endpoint)
                 << " elapsed_ms=" << connectDuration.count() << std::endl;

    LOG_DXRT_DBG << "  connectToServer succeeded" << std::endl;

    runtimeRaw->client.setThrottleNotificationHandler(
        [runtimeRaw](int deviceId, const dx_pcie_dev_ntfy_throt_t &throtInfo) {
            if (runtimeRaw->onThrottleNotification == nullptr)
            {
                return 0;
            }

            LOG_DXRT_DBG << "ThrottleNotification deviceId=" << deviceId
                         << ", ntfy_code=" << throtInfo.ntfy_code
                         << ", npu_id=" << throtInfo.npu_id << std::endl;

            runtimeRaw->onThrottleNotification(deviceId, static_cast<int>(throtInfo.npu_id), throtInfo);
            return 0;
        });

    runtimeRaw->client.setErrorNotificationHandler(
        [runtimeRaw](int deviceId, dxrt_server_err_t serverErr, const dx_pcie_dev_err_t &errorInfo) {
            if (runtimeRaw->onErrorNotification == nullptr)
            {
                return 0;
            }

            LOG_DXRT_DBG << "ErrorNotification deviceId=" << deviceId
                         << ", serverErr=" << static_cast<long>(serverErr)
                         << ", err_code=" << errorInfo.err_code
                         << ", npu_id=" << errorInfo.npu_id << std::endl;

            runtimeRaw->onErrorNotification(deviceId, serverErr, errorInfo);
            return 0;
        });

    runtimeRaw->client.setInferenceResponseHandler(
        [runtimeRaw](const IPCPacketClient::InferenceResult &response) {
            dxrt_response_t inferenceResponse = response.response;
            if (response.result != 0 && inferenceResponse.status == 0)
            {
                inferenceResponse.status = response.result;
            }

            LOG_DXRT_DBG << "DynamicIPC InferenceResponse deviceId=" << response.deviceId
                         << ", result=" << response.result
                         << ", req_id=" << inferenceResponse.req_id
                         << ", status=" << inferenceResponse.status << std::endl;

            if (runtimeRaw->onInferenceResponse != nullptr)
            {
                runtimeRaw->onInferenceResponse(response.deviceId, inferenceResponse);
            }
            return 0;
        });

    runtimeRaw->client.setRecoveryStartNotificationHandler(
        [runtimeRaw](int deviceId, uint32_t recoveryId) {
            if (runtimeRaw->onRecoveryStartNotification != nullptr)
            {
                runtimeRaw->onRecoveryStartNotification(deviceId, recoveryId);
            }
            return true;
        });

    LOG_DXRT_DBG << "CreateServiceLayerDynamicIpcClient SUCCESS: all handlers registered" << std::endl;
    return IpcClientOwner(runtime.release(), &DestroyServiceLayerDynamicIpcClient);
}

void DestroyServiceLayerDynamicIpcClient(ServiceLayerDynamicIpcClient *client)
{
    if (client == nullptr)
    {
        return;
    }

    delete client;
}

int ServiceLayerDynamicIpcTaskInit(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    int bound,
    uint64_t modelMemorySize,
    const TaskStaticConfig &config)
{
    if (client == nullptr)
    {
        return -1;
    }

    LOG_DXRT_DBG << "ServiceLayerDynamicIpcTaskInit deviceId=" << deviceId
                 << ", taskId=" << taskId
                 << ", bound=" << bound
                 << ", modelMemorySize=" << modelMemorySize << std::endl;
    const int rc = client->client.TaskInit(
        deviceId,
        taskId,
        bound,
        modelMemorySize,
        config);
    LOG_DXRT_DBG << "ServiceLayerDynamicIpcTaskInit result=" << rc << std::endl;
    return rc;
}

int ServiceLayerDynamicIpcTaskDeInit(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    int bound)
{
    if (client == nullptr)
    {
        return -1;
    }

    LOG_DXRT_DBG << "ServiceLayerDynamicIpcTaskDeInit deviceId=" << deviceId
                 << ", taskId=" << taskId
                 << ", bound=" << bound << std::endl;
    const int rc = client->client.TaskDeInit(
        deviceId,
        taskId,
        bound);
    LOG_DXRT_DBG << "ServiceLayerDynamicIpcTaskDeInit result=" << rc << std::endl;
    return rc;
}

int ServiceLayerDynamicIpcAllocate(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    MemoryType memoryType,
    uint32_t bufferSize,
    uint64_t *bufferAddress)
{
    if (client == nullptr || bufferAddress == nullptr)
    {
        return -1;
    }

    IPCPacketClient::AllocateResult response{};
    LOG_DXRT_DBG << "ServiceLayerDynamicIpcAllocate deviceId=" << deviceId
                 << ", taskId=" << taskId
                 << ", bufferSize=" << bufferSize
                 << ", memoryType=" << static_cast<int>(memoryType) << std::endl;
    const int rc = client->client.Allocate(
        deviceId,
        bufferSize,
        taskId,
        memoryType,
        &response);
    if (rc != 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayerDynamicIpcAllocate request failed: deviceId=" + std::to_string(deviceId) +
            ", taskId=" + std::to_string(taskId) +
            ", bufferSize=" + std::to_string(bufferSize) +
            ", memoryType=" + std::to_string(static_cast<int>(memoryType)) +
            ", errno=" + std::to_string(errno) +
            " (" + std::string(strerror(errno)) + ")");
        return rc;
    }

    if (response.result != 0)
    {
        LOG_DXRT_ERR(
            "ServiceLayerDynamicIpcAllocate response failed: deviceId=" + std::to_string(deviceId) +
            ", taskId=" + std::to_string(taskId) +
            ", bufferSize=" + std::to_string(bufferSize) +
            ", memoryType=" + std::to_string(static_cast<int>(memoryType)) +
            ", result=" + std::to_string(response.result));
        return response.result;
    }

    *bufferAddress = response.bufferAddress;
    LOG_DXRT_DBG << "ServiceLayerDynamicIpcAllocate success address=0x"
                 << std::hex << *bufferAddress << std::dec
                 << ", responseSize=" << response.bufferSize << std::endl;
    return 0;
}

int ServiceLayerDynamicIpcAllocateInfo(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    MemoryType memoryType,
    uint32_t bufferSize,
    SharedMemoryInfo *infoOut)
{
    if (client == nullptr || infoOut == nullptr)
    {
        return -1;
    }

    IPCPacketClient::AllocateResult response{};
    intptr_t responseFd = dxrt::shm::kInvalidMemFDHandle;
    const int rc = client->client.Allocate(
        deviceId,
        bufferSize,
        taskId,
        memoryType,
        &response,
        &responseFd);
    if (rc != 0)
    {
        return rc;
    }

    if (response.result != 0)
    {
        return response.result;
    }

    SharedMemoryInfo info{};
    info.block_id = response.blockId;
    info.size = response.bufferSize;
    info.ptr = nullptr;
    info.set_phys_addr(response.bufferAddress);
    info.deviceid = deviceId;
    info.taskId = taskId;
    info.pid = getpid();
    info.fd = responseFd;

    if (responseFd != dxrt::shm::kInvalidMemFDHandle)
    {
        try {
            info.ptr = shm::SharedMemorySyscallAdapter::MapMemFD(responseFd, static_cast<size_t>(response.bufferSize));
        } catch (...) {
            shm::SharedMemorySyscallAdapter::CloseMemFD(responseFd);
            return -1;
        }
    }

    *infoOut = info;
    return 0;
}

int ServiceLayerDynamicIpcDeallocate(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    uint64_t bufferAddress)
{
    if (client == nullptr)
    {
        return -1;
    }

    LOG_DXRT_DBG << "ServiceLayerDynamicIpcDeallocate deviceId=" << deviceId
                 << ", bufferAddress=0x" << std::hex << bufferAddress << std::dec << std::endl;
    const int rc = client->client.Deallocate(
        deviceId,
        bufferAddress);
    LOG_DXRT_DBG << "ServiceLayerDynamicIpcDeallocate result=" << rc << std::endl;
    return rc;
}

int ServiceLayerDynamicIpcDeallocateInfo(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    SharedMemoryInfo *info)
{
    if (client == nullptr || info == nullptr)
    {
        return -1;
    }

    const uint64_t key = (info->block_id != 0) ? info->block_id : info->phys_addr();
    const int result = ServiceLayerDynamicIpcDeallocate(client, deviceId, key);

    if (info->ptr != nullptr && info->size > 0)
    {
        shm::SharedMemorySyscallAdapter::UnmapMemFD(info->ptr, static_cast<size_t>(info->size));
        info->ptr = nullptr;
    }
    if (info->fd >= 0)
    {
        shm::SharedMemorySyscallAdapter::CloseMemFD(info->fd);
        info->fd = -1;
    }

    return result;
}

void ServiceLayerDynamicIpcSetThrottleNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    ThrottleNotificationCallback callback)
{
    if (client != nullptr)
    {
        client->onThrottleNotification = std::move(callback);
    }
}

void ServiceLayerDynamicIpcSetErrorNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    ErrorNotificationCallback callback)
{
    if (client != nullptr)
    {
        client->onErrorNotification = std::move(callback);
    }
}

void ServiceLayerDynamicIpcSetInferenceResponseHandler(
    ServiceLayerDynamicIpcClient *client,
    InferenceResponseCallback callback)
{
    if (client != nullptr)
    {
        client->onInferenceResponse = std::move(callback);
    }
}

void ServiceLayerDynamicIpcSetRecoveryStartNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    RecoveryStartNotificationCallback callback)
{
    if (client != nullptr)
    {
        client->onRecoveryStartNotification = std::move(callback);
    }
}

int ServiceLayerDynamicIpcSendRecoveryStartAck(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    uint32_t recoveryId,
    int result)
{
    if (client == nullptr)
    {
        return -1;
    }

    return client->client.sendRecoveryStartAck(deviceId, recoveryId, result);
}

int ServiceLayerDynamicIpcInferenceRequest(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    const dxrt_request_acc_t &request,
    dxrt_response_t *responseOut)
{
    if (client == nullptr)
    {
        return -1;
    }

    LOG_DXRT_DBG << "ServiceLayerDynamicIpcInferenceRequest deviceId=" << deviceId
                 << ", taskId=" << taskId
                 << ", req_id=" << request.req_id << std::endl;

    if (responseOut != nullptr)
    {
        *responseOut = dxrt_response_t{};
    }

    const int rc = client->client.InferenceRequestAsync(
        deviceId,
        taskId,
        request);

    if (rc != 0)
    {
        LOG_DXRT_ERR("ServiceLayerDynamicIpcInferenceRequest IPC call failed: deviceId="
            + std::to_string(deviceId) + ", rc=" + std::to_string(rc));
        return rc;
    }

    LOG_DXRT_DBG << "ServiceLayerDynamicIpcInferenceRequest queued req_id="
                 << request.req_id << std::endl;
    return 0;
}

int ServiceLayerDynamicIpcDMARead(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (client == nullptr)
    {
        return -1;
    }

    return client->client.DMARead(
        deviceId,
        blockId,
        blockOffset,
        size);
}

int ServiceLayerDynamicIpcDMAWrite(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (client == nullptr)
    {
        return -1;
    }

    return client->client.DMAWrite(
        deviceId,
        blockId,
        blockOffset,
        size);
}

int ServiceLayerDynamicIPCDMAReadWithFaultInjection(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (client == nullptr)
    {
        return -1;
    }
    return client->client.DMAReadWithFaultInjection(
        deviceId,
        blockId,
        blockOffset,
        size);
}

}  // namespace dxrt
