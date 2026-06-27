/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <stdint.h>

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <functional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ipc_packet.hpp"
#include "../transport/ipc_server_endpoint.hpp"
#include "dxrt/common.h"
#include "dxrt/safe_cast.h"
#include "../device_pool/inference_context.h"

namespace dxrt {

class DXRT_INTERNAL_API IPCPacketHandlerRegistry
{
 public:
    static constexpr int kResponseAlreadySent = 1;

    using OnClientPidHandler = std::function<void(pid_t)>;

    using Handler = std::function<int(
        int clientFd,
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize,
        int receivedFd)>;

    using ServerOnReceiveHandler = std::function<int(
        int clientFd,
        const uint8_t *message,
        size_t messageSize,
        int receivedFd)>;

    using ServerOnClientConnectedHandler = std::function<void(int clientFd)>;
    using ServerOnClientDisconnectedHandler = std::function<void(int clientFd)>;

    using AllocateHandler = std::function<int(
        int clientFd,
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int taskId,
        uint64_t bufferSize,
        dxrt::MemoryType memoryType)>;

    using FreeBufferHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        uint64_t bufferAddress)>;

    using InferenceRequestHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int taskId,
        const dxrt_request_acc_t &request)>;

    using DMAReadHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int64_t  blockId,
        uint64_t blockOffset,
        uint64_t size)>;

    using DMAWriteHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int64_t  blockId,
        uint64_t blockOffset,
        uint64_t size)>;

    using TaskInitHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int taskId,
        int bound,
        uint64_t modelMemorySize,
        const TaskStaticConfig &config)>;

    using TaskDeInitHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int taskId,
        int bound)>;

    using ViewFreeMemoryLegacyHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId)>;

    using ViewUsedMemoryLegacyHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId)>;

    using GetUsageLegacyHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int channel)>;

    using GetDeviceTelemetryHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId)>;

    using RecoveryStartAckHandler = std::function<int(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        uint32_t recoveryId,
        int result)>;

    ~IPCPacketHandlerRegistry();

    void clearHandlers();
    bool hasHandler(IPCMessageType type) const;

    void setAllocateHandler(AllocateHandler handler);
    void setFreeBufferHandler(FreeBufferHandler handler);
    void setInferenceRequestHandler(InferenceRequestHandler handler);
    void setDMAReadHandler(DMAReadHandler handler);
    void setDMAReadWithFaultInjectionHandler(DMAReadHandler handler);
    void setDMAWriteHandler(DMAWriteHandler handler);
    void setTaskInitHandler(TaskInitHandler handler);
    void setTaskDeInitHandler(TaskDeInitHandler handler);
    void setViewFreeMemoryLegacyHandler(ViewFreeMemoryLegacyHandler handler);
    void setViewUsedMemoryLegacyHandler(ViewUsedMemoryLegacyHandler handler);
    void setGetUsageLegacyHandler(GetUsageLegacyHandler handler);
    void setGetDeviceTelemetryHandler(GetDeviceTelemetryHandler handler);
    void setRecoveryStartAckHandler(RecoveryStartAckHandler handler);

    void setOnClientConnectedPid(OnClientPidHandler handler);
    void setOnClientDisconnectedPid(OnClientPidHandler handler);

    static int assembleAllocateResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        uint64_t bufferAddress,
        uint64_t allocatedBufferSize,
        int64_t blockId,
        std::vector<uint8_t> *response,
        intptr_t inPacketFd = -1);

    static int assembleFreeBufferResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        std::vector<uint8_t> *response);

    static int assembleInferenceResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        const dxrt_response_t &inferenceResponse,
        std::vector<uint8_t> *response);

    static int assembleDMAReadResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        std::vector<uint8_t> *response);

    static int assembleDMAWriteResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        std::vector<uint8_t> *response);

    static int assembleTaskInitResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        std::vector<uint8_t> *response);

    static int assembleTaskDeInitResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        std::vector<uint8_t> *response);

    static int assembleViewFreeMemoryLegacyResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        uint64_t bytes,
        std::vector<uint8_t> *response);

    static int assembleViewUsedMemoryLegacyResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        uint64_t bytes,
        std::vector<uint8_t> *response);

    static int assembleGetUsageLegacyResponsePacket(
        const IPCPacketHeader &requestHeader,
        int result,
        double usage,
        std::vector<uint8_t> *response);

    static int assembleGetDeviceTelemetryResponsePacket(
        const IPCPacketHeader &requestHeader,
        const IPCDeviceTelemetryPayload &telemetry,
        std::vector<uint8_t> *response);

    static int assembleRecoveryStartNotificationPacket(
        const IPCPacketHeader &requestHeader,
        uint32_t recoveryId,
        std::vector<uint8_t> *response);

    static int assembleRecoverySuccessNotificationPacket(
        const IPCPacketHeader &requestHeader,
        uint32_t recoveryId,
        std::vector<uint8_t> *response);

    static int assembleRecoveryFailNotificationPacket(
        const IPCPacketHeader &requestHeader,
        uint32_t recoveryId,
        std::vector<uint8_t> *response);

    int SendAllocResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        uint64_t bufferAddress,
        uint64_t allocatedBufferSize,
        int64_t blockId = 0,
        intptr_t responseFd = -1);

    int SendAllocResultPacketToClient(
        int clientFd,
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        uint64_t bufferAddress,
        uint64_t allocatedBufferSize,
        int64_t blockId = 0,
        intptr_t responseFd = -1);

    int SendFreeBufferResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result);

    int SendInferenceResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        const dxrt_response_t &inferenceResponse);

    int SendDMAReadResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result);

    int SendDMAWriteResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result);

    int SendTaskInitResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result);

    int SendTaskDeInitResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result);

    int SendViewFreeMemoryLegacyResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        uint64_t bytes);

    int SendViewUsedMemoryLegacyResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        uint64_t bytes);

    int SendGetUsageLegacyResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        int result,
        double usage);

    int SendGetDeviceTelemetryResultPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        const IPCDeviceTelemetryPayload &telemetry);

    int SendRecoveryStartNotificationPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        uint32_t recoveryId);

    int SendRecoverySuccessNotificationPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        uint32_t recoveryId);

    int SendRecoveryFailNotificationPacket(
        pid_t pid,
        int deviceId,
        int32_t seqId,
        uint32_t recoveryId);

    int handle(
        int clientFd,
        const uint8_t *message,
        size_t messageSize,
        int receivedFd);

    void onClientAccepted(int clientFd);
    void onClientDisconnected(int clientFd, pid_t pid);

    ServerOnReceiveHandler createOnReceiveHandler();
    ServerOnClientConnectedHandler createOnClientConnectedHandler();
    IPCServerEndpoint::OnClientDisconnectedHandler createOnClientDisconnectedHandler();
    void attachToServer(IPCServerEndpoint *server);

 private:
    template <typename ResponsePacket>
    static void buildResponsePacket(
        ResponsePacket *responsePacket,
        IPCMessageType type,
        const IPCPacketHeader &requestHeader);

    template <typename RequestPacket>
    static int parseRequestPacket(
        RequestPacket *requestPacket,
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize);

    template <typename ResponsePacket>
    static void writeResponsePacket(
        std::vector<uint8_t> *response,
        const ResponsePacket &responsePacket);

    // Core template: build, populate via SetupFn, and serialize a response packet.
    // Eliminates per-packet boilerplate from every assemble*ResponsePacket implementation.
    //
    // Usage:
    //   return assemblePacketImpl<IPCPacketFooResponse, IPCMessageType::FooResponse>(
    //       requestHeader, response,
    //       [result, extraField](IPCPacketFooResponse &pkt) {
    //           pkt.result = result;
    //           pkt.extra  = extraField;
    //       });
    template <typename ResponsePacket, IPCMessageType MsgType, typename SetupFn>
    static int assemblePacketImpl(
        const IPCPacketHeader &requestHeader,
        std::vector<uint8_t> *response,
        SetupFn setup);

    // All request handlers share the same signature. The clientFd is unused
    // by most handlers but kept for uniformity so the dispatch table can be
    // generated from a single X-macro list.
    int handleAllocateRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleFreeBufferRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleInferenceRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleDMAReadRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleDMAReadWithFaultInjectionRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleDMAWriteRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleTaskInitRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleTaskDeInitRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleViewFreeMemoryLegacyRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleViewUsedMemoryLegacyRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleGetUsageLegacyRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleGetDeviceTelemetryRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;
    int handleRecoveryStartAckRequest(
        int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const;

    AllocateHandler _allocateHandler;
    FreeBufferHandler _freeBufferHandler;
    InferenceRequestHandler _inferenceRequestHandler;
    DMAReadHandler _dmaReadHandler;
    DMAWriteHandler _dmaWriteHandler;
    TaskInitHandler _taskInitHandler;
    TaskDeInitHandler _taskDeInitHandler;
    ViewFreeMemoryLegacyHandler _viewFreeMemoryLegacyHandler;
    ViewUsedMemoryLegacyHandler _viewUsedMemoryLegacyHandler;
    GetUsageLegacyHandler _getUsageLegacyHandler;
    GetDeviceTelemetryHandler _getDeviceTelemetryHandler;
    RecoveryStartAckHandler _recoveryStartAckHandler;
    DMAReadHandler _dmaReadWithFaultInjectionHandler;

    IPCServerEndpoint *_server{nullptr};
    OnClientPidHandler _onClientConnectedPid;
    OnClientPidHandler _onClientDisconnectedPid;
};

template <typename ResponsePacket>
void IPCPacketHandlerRegistry::buildResponsePacket(
    ResponsePacket *responsePacket,
    IPCMessageType type,
    const IPCPacketHeader &requestHeader)
{
    initializeIPCPacket(responsePacket, type, requestHeader.seqId);
    responsePacket->header.seqId = requestHeader.seqId;
    responsePacket->header.deviceId = requestHeader.deviceId;
    responsePacket->header.pid = requestHeader.pid;
}

template <typename RequestPacket>
int IPCPacketHandlerRegistry::parseRequestPacket(
    RequestPacket *requestPacket,
    const IPCPacketHeader &header,
    const uint8_t *payload,
    size_t payloadSize)
{
    if (requestPacket == nullptr || payload == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (payloadSize != sizeof(RequestPacket) - sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        return -1;
    }

    requestPacket->header = header;
    std::memcpy(
        SafeCast::PtrToBytePtr(requestPacket) + sizeof(IPCPacketHeader),
        payload,
        payloadSize);
    return 0;
}

template <typename ResponsePacket>
void IPCPacketHandlerRegistry::writeResponsePacket(
    std::vector<uint8_t> *response,
    const ResponsePacket &responsePacket)
{
    if (response == nullptr)
    {
        return;
    }

    response->resize(sizeof(ResponsePacket));
    std::memcpy(response->data(), &responsePacket, sizeof(ResponsePacket));
}

template <typename ResponsePacket, IPCMessageType MsgType, typename SetupFn>
int IPCPacketHandlerRegistry::assemblePacketImpl(
    const IPCPacketHeader &requestHeader,
    std::vector<uint8_t> *response,
    SetupFn setup)
{
    if (response == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    ResponsePacket pkt{};
    buildResponsePacket(&pkt, MsgType, requestHeader);
    setup(pkt);
    writeResponsePacket(response, pkt);
    return 0;
}

}  // namespace dxrt
