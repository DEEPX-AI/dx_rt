/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "dxrt/driver.h"
#include "dxrt/exception/server_err.h"
#include "../dynamic_ipc/protocol/memory_type.hpp"
#include "../dynamic_ipc/shm/shm.h"
#include "inference_context.h"

namespace dxrt {

struct ServiceLayerDynamicIpcClient;
using IpcClientOwner =
    std::unique_ptr<ServiceLayerDynamicIpcClient, void (*)(ServiceLayerDynamicIpcClient *)>;

using ThrottleNotificationCallback = std::function<void(int, int, const dx_pcie_dev_ntfy_throt_t&)>;
using ErrorNotificationCallback = std::function<void(int, dxrt_server_err_t, const dx_pcie_dev_err_t&)>;
using InferenceResponseCallback = std::function<void(int, const dxrt_response_t&)>;
using RecoveryStartNotificationCallback = std::function<void(int, uint32_t)>;

IpcClientOwner CreateServiceLayerDynamicIpcClient(const std::string &endpoint);
void DestroyServiceLayerDynamicIpcClient(ServiceLayerDynamicIpcClient *client);

void ServiceLayerDynamicIpcSetThrottleNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    ThrottleNotificationCallback callback);

void ServiceLayerDynamicIpcSetErrorNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    ErrorNotificationCallback callback);

void ServiceLayerDynamicIpcSetInferenceResponseHandler(
    ServiceLayerDynamicIpcClient *client,
    InferenceResponseCallback callback);

void ServiceLayerDynamicIpcSetRecoveryStartNotificationHandler(
    ServiceLayerDynamicIpcClient *client,
    RecoveryStartNotificationCallback callback);

int ServiceLayerDynamicIpcSendRecoveryStartAck(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    uint32_t recoveryId,
    int result);

int ServiceLayerDynamicIpcTaskInit(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    int bound,
    uint64_t modelMemorySize,
    const TaskStaticConfig &config);

int ServiceLayerDynamicIpcTaskDeInit(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    int bound);

int ServiceLayerDynamicIpcAllocate(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    MemoryType memoryType,
    uint32_t bufferSize,
    uint64_t *bufferAddress);

int ServiceLayerDynamicIpcAllocateInfo(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    MemoryType memoryType,
    uint32_t bufferSize,
    SharedMemoryInfo *infoOut);

int ServiceLayerDynamicIpcDeallocate(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    uint64_t bufferAddress);

int ServiceLayerDynamicIpcDeallocateInfo(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    SharedMemoryInfo *info);

int ServiceLayerDynamicIpcInferenceRequest(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int taskId,
    const dxrt_request_acc_t &request,
    dxrt_response_t *responseOut);

int ServiceLayerDynamicIpcDMARead(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size);

int ServiceLayerDynamicIpcDMAWrite(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size);

int ServiceLayerDynamicIPCDMAReadWithFaultInjection(
    ServiceLayerDynamicIpcClient *client,
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size);

}  // namespace dxrt
