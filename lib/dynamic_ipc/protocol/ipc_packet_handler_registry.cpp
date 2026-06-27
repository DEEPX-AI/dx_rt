/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_packet_handler_registry.hpp"

#include <cerrno>
#include <cstring>
#include <utility>

#include "../shm/memfd_handle_transfer.h"
#include "../transport/ipc_server_endpoint.hpp"
#include "dxrt/common.h"

namespace dxrt {

namespace {

static TaskStaticConfig taskStaticConfigFromPacket(const IPCPacketTaskInitRequest& pkt)
{
    TaskStaticConfig cfg{};
    cfg.model_type         = pkt.model_type;
    cfg.model_format       = pkt.model_format;
    cfg.model_cmds         = pkt.model_cmds;
    cfg.op_mode            = pkt.op_mode;
    cfg.cmd_offset         = pkt.cmd_offset;
    cfg.weight_offset      = pkt.weight_offset;
    cfg.custom_offset      = pkt.custom_offset;
    cfg.input_size         = pkt.input_size;
    cfg.output_size        = pkt.output_size;
    cfg.last_output_offset = pkt.last_output_offset;
    cfg.output_all_offset  = pkt.output_all_offset;
    static_assert(sizeof(cfg.datas) == sizeof(pkt.datas), "datas size mismatch");
    std::memcpy(cfg.datas, pkt.datas, sizeof(pkt.datas));
    cfg.memory_base        = pkt.memory_base;
    return cfg;
}

#define DXRT_IPC_REQUEST_HANDLER_LIST(X) \
    X(AllocateBuffer, Allocate, AllocateHandler, _allocateHandler, \
        handleAllocateRequest(clientFd, *header, payload, payloadSize)) \
    X(FreeBuffer, FreeBuffer, FreeBufferHandler, _freeBufferHandler, \
        handleFreeBufferRequest(clientFd, *header, payload, payloadSize)) \
    X(InferenceRequest, InferenceRequest, InferenceRequestHandler, _inferenceRequestHandler, \
        handleInferenceRequest(clientFd, *header, payload, payloadSize)) \
    X(DMAReadRequest, DMARead, DMAReadHandler, _dmaReadHandler, \
        handleDMAReadRequest(clientFd, *header, payload, payloadSize)) \
    X(DMAReadRequestWithFaultInjection, DMAReadWithFaultInjection, DMAReadHandler, _dmaReadWithFaultInjectionHandler, \
        handleDMAReadWithFaultInjectionRequest(clientFd, *header, payload, payloadSize)) \
    X(DMAWriteRequest, DMAWrite, DMAWriteHandler, _dmaWriteHandler, \
        handleDMAWriteRequest(clientFd, *header, payload, payloadSize)) \
    X(TaskInitRequest, TaskInit, TaskInitHandler, _taskInitHandler, \
        handleTaskInitRequest(clientFd, *header, payload, payloadSize)) \
    X(TaskDeInitRequest, TaskDeInit, TaskDeInitHandler, _taskDeInitHandler, \
        handleTaskDeInitRequest(clientFd, *header, payload, payloadSize)) \
    X(ViewFreeMemoryRequestLegacy, ViewFreeMemoryLegacy, ViewFreeMemoryLegacyHandler, \
        _viewFreeMemoryLegacyHandler, handleViewFreeMemoryLegacyRequest(clientFd, *header, payload, payloadSize)) \
    X(ViewUsedMemoryRequestLegacy, ViewUsedMemoryLegacy, ViewUsedMemoryLegacyHandler, \
        _viewUsedMemoryLegacyHandler, handleViewUsedMemoryLegacyRequest(clientFd, *header, payload, payloadSize)) \
    X(GetUsageRequestLegacy, GetUsageLegacy, GetUsageLegacyHandler, _getUsageLegacyHandler, \
        handleGetUsageLegacyRequest(clientFd, *header, payload, payloadSize)) \
    X(GetDeviceTelemetryRequest, GetDeviceTelemetry, GetDeviceTelemetryHandler, \
        _getDeviceTelemetryHandler, handleGetDeviceTelemetryRequest(clientFd, *header, payload, payloadSize)) \
    X(RecoveryStartAck, RecoveryStartAck, RecoveryStartAckHandler, _recoveryStartAckHandler, \
        handleRecoveryStartAckRequest(clientFd, *header, payload, payloadSize))

IPCPacketHeader makeRequestHeaderForResponse(pid_t pid, int deviceId, int32_t seqId)
{
    IPCPacketHeader header{};
    header.pid = pid;
    header.deviceId = deviceId;
    header.seqId = seqId;
    return header;
}

int sendAssembledPacket(IPCServerEndpoint *server, pid_t pid, const std::vector<uint8_t> &packet, int responseFd)
{
    if (server == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    return server->sendToPid(
        pid,
        packet.empty() ? nullptr : packet.data(),
        packet.size(),
        responseFd);
}

int sendAssembledPacketToClient(
    IPCServerEndpoint *server,
    int clientFd,
    const std::vector<uint8_t> &packet,
    int responseFd)
{
    if (server == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    return server->sendToClient(
        clientFd,
        packet.empty() ? nullptr : packet.data(),
        packet.size(),
        responseFd);
}

template <typename AssemblePacketFn>
int buildAndSendPacketToPid(
    IPCServerEndpoint *server,
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int responseFd,
    AssemblePacketFn assemblePacket)
{
    std::vector<uint8_t> packet;
    const IPCPacketHeader requestHeader = makeRequestHeaderForResponse(pid, deviceId, seqId);
    if (assemblePacket(requestHeader, &packet) != 0)
    {
        return -1;
    }

    return sendAssembledPacket(server, pid, packet, responseFd);
}

template <typename AssemblePacketFn>
int buildAndSendPacketToClient(
    IPCServerEndpoint *server,
    int clientFd,
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int responseFd,
    AssemblePacketFn assemblePacket)
{
    std::vector<uint8_t> packet;
    const IPCPacketHeader requestHeader = makeRequestHeaderForResponse(pid, deviceId, seqId);
    if (assemblePacket(requestHeader, &packet) != 0)
    {
        return -1;
    }

    return sendAssembledPacketToClient(server, clientFd, packet, responseFd);
}

}  // namespace

IPCPacketHandlerRegistry::~IPCPacketHandlerRegistry()
{
    // Explicitly detach from server to break lambda capture cycles
    if (_server != nullptr)
    {
        _server->setOnReceive(nullptr);
        _server->setOnClientConnected(nullptr);
        _server->setOnClientDisconnected(nullptr);
        _server = nullptr;
    }
}

int IPCPacketHandlerRegistry::assembleAllocateResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    uint64_t bufferAddress,
    uint64_t allocatedBufferSize,
    int64_t blockId,
    std::vector<uint8_t> *response,
    intptr_t inPacketFd)
{
    return assemblePacketImpl<IPCPacketResponseAllocateBuffer, IPCMessageType::AllocateBuffer>(
        requestHeader, response,
        [result, bufferAddress, allocatedBufferSize, blockId, inPacketFd](IPCPacketResponseAllocateBuffer &pkt) {
            pkt.result          = result;
            pkt.bufferAddress   = bufferAddress;
            pkt.bufferSize      = allocatedBufferSize;
            pkt.blockId         = blockId;
            pkt.fd              = inPacketFd;
        });
}

int IPCPacketHandlerRegistry::assembleFreeBufferResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketResponseFreeBuffer, IPCMessageType::FreeBuffer>(
        requestHeader, response,
        [result](IPCPacketResponseFreeBuffer &pkt) { pkt.result = result; });
}

int IPCPacketHandlerRegistry::assembleInferenceResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    const dxrt_response_t &inferenceResponse,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketResponseInference, IPCMessageType::InferenceResponse>(
        requestHeader, response,
        [result, &inferenceResponse](IPCPacketResponseInference &pkt) {
            pkt.result   = result;
            pkt.response = inferenceResponse;
        });
}

int IPCPacketHandlerRegistry::assembleDMAReadResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketDMAReadResponse, IPCMessageType::DMAReadResponse>(
        requestHeader, response,
        [result](IPCPacketDMAReadResponse &pkt) { pkt.result = result; });
}

int IPCPacketHandlerRegistry::assembleDMAWriteResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketDMAWriteResponse, IPCMessageType::DMAWriteResponse>(
        requestHeader, response,
        [result](IPCPacketDMAWriteResponse &pkt) { pkt.result = result; });
}

int IPCPacketHandlerRegistry::assembleTaskInitResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketTaskInitResponse, IPCMessageType::TaskInitResponse>(
        requestHeader, response,
        [result](IPCPacketTaskInitResponse &pkt) { pkt.result = result; });
}

int IPCPacketHandlerRegistry::assembleTaskDeInitResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketTaskDeInitResponse, IPCMessageType::TaskDeInitResponse>(
        requestHeader, response,
        [result](IPCPacketTaskDeInitResponse &pkt) { pkt.result = result; });
}

int IPCPacketHandlerRegistry::assembleViewFreeMemoryLegacyResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    uint64_t bytes,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketViewFreeMemoryResponseLegacy, IPCMessageType::ViewFreeMemoryResponseLegacy>(
        requestHeader, response,
        [result, bytes](IPCPacketViewFreeMemoryResponseLegacy &pkt) {
            pkt.result = result;
            pkt.bytes  = bytes;
        });
}

int IPCPacketHandlerRegistry::assembleViewUsedMemoryLegacyResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    uint64_t bytes,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketViewUsedMemoryResponseLegacy, IPCMessageType::ViewUsedMemoryResponseLegacy>(
        requestHeader, response,
        [result, bytes](IPCPacketViewUsedMemoryResponseLegacy &pkt) {
            pkt.result = result;
            pkt.bytes  = bytes;
        });
}

int IPCPacketHandlerRegistry::assembleGetUsageLegacyResponsePacket(
    const IPCPacketHeader &requestHeader,
    int result,
    double usage,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketGetUsageResponseLegacy, IPCMessageType::GetUsageResponseLegacy>(
        requestHeader, response,
        [result, usage](IPCPacketGetUsageResponseLegacy &pkt) {
            pkt.result = result;
            pkt.usage  = usage;
        });
}

int IPCPacketHandlerRegistry::assembleGetDeviceTelemetryResponsePacket(
    const IPCPacketHeader &requestHeader,
    const IPCDeviceTelemetryPayload &telemetry,
    std::vector<uint8_t> *response)
{
    return serializeIPCDeviceTelemetryResponsePacket(requestHeader, telemetry, response);
}

int IPCPacketHandlerRegistry::assembleRecoveryStartNotificationPacket(
    const IPCPacketHeader &requestHeader,
    uint32_t recoveryId,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketRecoveryStartNotification, IPCMessageType::RecoveryStartNotification>(
        requestHeader, response,
        [recoveryId](IPCPacketRecoveryStartNotification &pkt) {
            pkt.recoveryId = recoveryId;
        });
}

int IPCPacketHandlerRegistry::assembleRecoverySuccessNotificationPacket(
    const IPCPacketHeader &requestHeader,
    uint32_t recoveryId,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketRecoverySuccessNotification, IPCMessageType::RecoverySuccessNotification>(
        requestHeader, response,
        [recoveryId](IPCPacketRecoverySuccessNotification &pkt) {
            pkt.recoveryId = recoveryId;
        });
}

int IPCPacketHandlerRegistry::assembleRecoveryFailNotificationPacket(
    const IPCPacketHeader &requestHeader,
    uint32_t recoveryId,
    std::vector<uint8_t> *response)
{
    return assemblePacketImpl<IPCPacketRecoveryFailNotification, IPCMessageType::RecoveryFailNotification>(
        requestHeader, response,
        [recoveryId](IPCPacketRecoveryFailNotification &pkt) {
            pkt.recoveryId = recoveryId;
        });
}

int IPCPacketHandlerRegistry::SendAllocResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    uint64_t bufferAddress,
    uint64_t allocatedBufferSize,
    int64_t blockId,
    intptr_t responseFd)
{
    // PrepareForPacket: Windows = DuplicateHandle'd HANDLE, Linux = kInvalidMemFDHandle(-1, no-op)
    // responseFd is passed through as-is for Linux SCM_RIGHTS.
    // The Windows IPC channel (named pipe) discards the fd argument right before transmission.
    const intptr_t inPacketFd = dxrt::shm::MemFDHandleTransfer::PrepareForPacket(
        responseFd, static_cast<uint32_t>(pid));
    const int rc = buildAndSendPacketToPid(
        _server, pid, deviceId, seqId, static_cast<int>(responseFd),
        [result, bufferAddress, allocatedBufferSize, blockId, inPacketFd](
            const IPCPacketHeader &requestHeader,
            std::vector<uint8_t> *packet) {
            return assembleAllocateResponsePacket(
                requestHeader, result, bufferAddress, allocatedBufferSize, blockId, packet, inPacketFd);
        });
    if (rc < 0 && inPacketFd != dxrt::shm::kInvalidMemFDHandle)
    {
        dxrt::shm::MemFDHandleTransfer::RevokeOnFailure(inPacketFd, static_cast<uint32_t>(pid));
    }
    return rc;
}

int IPCPacketHandlerRegistry::SendAllocResultPacketToClient(
    int clientFd,
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    uint64_t bufferAddress,
    uint64_t allocatedBufferSize,
    int64_t blockId,
    intptr_t responseFd)
{
    // PrepareForPacket: Windows = DuplicateHandle'd HANDLE, Linux = kInvalidMemFDHandle(-1, no-op)
    // responseFd is passed through as-is for Linux SCM_RIGHTS.
    // The Windows IPC channel (named pipe) discards the fd argument right before transmission.
    const intptr_t inPacketFd = dxrt::shm::MemFDHandleTransfer::PrepareForPacket(
        responseFd, static_cast<uint32_t>(pid));
    const int rc = buildAndSendPacketToClient(
        _server, clientFd, pid, deviceId, seqId, static_cast<int>(responseFd),
        [result, bufferAddress, allocatedBufferSize, blockId, inPacketFd](
            const IPCPacketHeader &requestHeader,
            std::vector<uint8_t> *packet) {
            return assembleAllocateResponsePacket(
                requestHeader, result, bufferAddress, allocatedBufferSize, blockId, packet, inPacketFd);
        });
    if (rc < 0 && inPacketFd != dxrt::shm::kInvalidMemFDHandle)
    {
        dxrt::shm::MemFDHandleTransfer::RevokeOnFailure(inPacketFd, static_cast<uint32_t>(pid));
    }
    return rc;
}

int IPCPacketHandlerRegistry::SendFreeBufferResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleFreeBufferResponsePacket(requestHeader, result, packet);
        });
}

int IPCPacketHandlerRegistry::SendInferenceResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    const dxrt_response_t &inferenceResponse)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result, &inferenceResponse](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleInferenceResponsePacket(requestHeader, result, inferenceResponse, packet);
        });
}

int IPCPacketHandlerRegistry::SendDMAReadResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleDMAReadResponsePacket(requestHeader, result, packet);
        });
}

int IPCPacketHandlerRegistry::SendDMAWriteResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleDMAWriteResponsePacket(requestHeader, result, packet);
        });
}

int IPCPacketHandlerRegistry::SendTaskInitResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleTaskInitResponsePacket(requestHeader, result, packet);
        });
}

int IPCPacketHandlerRegistry::SendTaskDeInitResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleTaskDeInitResponsePacket(requestHeader, result, packet);
        });
}

int IPCPacketHandlerRegistry::SendViewFreeMemoryLegacyResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    uint64_t bytes)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result, bytes](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleViewFreeMemoryLegacyResponsePacket(requestHeader, result, bytes, packet);
        });
}

int IPCPacketHandlerRegistry::SendViewUsedMemoryLegacyResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    uint64_t bytes)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result, bytes](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleViewUsedMemoryLegacyResponsePacket(requestHeader, result, bytes, packet);
        });
}

int IPCPacketHandlerRegistry::SendGetUsageLegacyResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    int result,
    double usage)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [result, usage](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleGetUsageLegacyResponsePacket(requestHeader, result, usage, packet);
        });
}

int IPCPacketHandlerRegistry::SendGetDeviceTelemetryResultPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    const IPCDeviceTelemetryPayload &telemetry)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [&telemetry](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleGetDeviceTelemetryResponsePacket(requestHeader, telemetry, packet);
        });
}

int IPCPacketHandlerRegistry::SendRecoveryStartNotificationPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    uint32_t recoveryId)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [recoveryId](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleRecoveryStartNotificationPacket(requestHeader, recoveryId, packet);
        });
}

int IPCPacketHandlerRegistry::SendRecoverySuccessNotificationPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    uint32_t recoveryId)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [recoveryId](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleRecoverySuccessNotificationPacket(requestHeader, recoveryId, packet);
        });
}

int IPCPacketHandlerRegistry::SendRecoveryFailNotificationPacket(
    pid_t pid,
    int deviceId,
    int32_t seqId,
    uint32_t recoveryId)
{
    return buildAndSendPacketToPid(_server, pid, deviceId, seqId, -1,
        [recoveryId](const IPCPacketHeader &requestHeader, std::vector<uint8_t> *packet) {
            return assembleRecoveryFailNotificationPacket(requestHeader, recoveryId, packet);
        });
}


void IPCPacketHandlerRegistry::clearHandlers()
{
#define X(messageType, methodSuffix, handlerType, member, dispatchCall) member = nullptr;
    DXRT_IPC_REQUEST_HANDLER_LIST(X)
#undef X
}

bool IPCPacketHandlerRegistry::hasHandler(IPCMessageType type) const
{
    switch (type)
    {
#define X(messageType, methodSuffix, handlerType, member, dispatchCall) \
        case IPCMessageType::messageType: \
            return member != nullptr;
        DXRT_IPC_REQUEST_HANDLER_LIST(X)
#undef X
        default:
            return false;
    }
}

#define X(messageType, methodSuffix, handlerType, member, dispatchCall) \
void IPCPacketHandlerRegistry::set##methodSuffix##Handler(handlerType handler) \
{ \
    member = std::move(handler); \
}
DXRT_IPC_REQUEST_HANDLER_LIST(X)
#undef X

void IPCPacketHandlerRegistry::setOnClientConnectedPid(OnClientPidHandler handler)
{
    _onClientConnectedPid = std::move(handler);
}

void IPCPacketHandlerRegistry::setOnClientDisconnectedPid(OnClientPidHandler handler)
{
    _onClientDisconnectedPid = std::move(handler);
}

// ---------------------------------------------------------------------------
// Per-request dispatch — generated from DXRT_IPC_HANDLER_IMPL_LIST.
// Each entry: (funcSuffix, member, RequestPacketType, handlerCallExpression)
// ---------------------------------------------------------------------------

// clang-format off
#define DXRT_IPC_HANDLER_IMPL_LIST(X) \
    X(Allocate,             _allocateHandler,             IPCPacketRequestAllocateBuffer,          \
        _allocateHandler(clientFd, header.pid, requestPacket.header.deviceId, header.seqId,        \
            requestPacket.taskId, requestPacket.bufferSize, requestPacket.memoryType))              \
    X(FreeBuffer,           _freeBufferHandler,           IPCPacketRequestFreeBuffer,               \
        _freeBufferHandler(header.pid, requestPacket.header.deviceId, header.seqId,                \
            requestPacket.bufferAddress))                                                            \
    X(Inference,            _inferenceRequestHandler,     IPCPacketRequestInference,                \
        _inferenceRequestHandler(header.pid, requestPacket.header.deviceId,                         \
            requestPacket.taskId, requestPacket.request))                                            \
    X(DMARead,              _dmaReadHandler,              IPCPacketDMAReadRequest,                   \
        _dmaReadHandler(header.pid, requestPacket.header.deviceId, header.seqId,                    \
            requestPacket.blockId, requestPacket.blockOffset, requestPacket.size))                  \
    X(DMAReadWithFaultInjection, _dmaReadWithFaultInjectionHandler, IPCPacketDMAReadRequestWithFaultInjection, \
        _dmaReadWithFaultInjectionHandler(header.pid, requestPacket.header.deviceId, header.seqId,  \
            requestPacket.blockId, requestPacket.blockOffset, requestPacket.size))                  \
    X(DMAWrite,             _dmaWriteHandler,             IPCPacketDMAWriteRequest,                  \
        _dmaWriteHandler(header.pid, requestPacket.header.deviceId, header.seqId,                   \
            requestPacket.blockId, requestPacket.blockOffset, requestPacket.size))                  \
    X(TaskInit,             _taskInitHandler,             IPCPacketTaskInitRequest,                  \
        _taskInitHandler(header.pid, requestPacket.header.deviceId, header.seqId,                   \
            requestPacket.taskId, requestPacket.bound, requestPacket.modelMemorySize,               \
            taskStaticConfigFromPacket(requestPacket)))                                              \
    X(TaskDeInit,           _taskDeInitHandler,           IPCPacketTaskDeInitRequest,                \
        _taskDeInitHandler(header.pid, requestPacket.header.deviceId, header.seqId,                 \
            requestPacket.taskId, requestPacket.bound))                                              \
    X(ViewFreeMemoryLegacy, _viewFreeMemoryLegacyHandler, IPCPacketViewFreeMemoryRequestLegacy,      \
        _viewFreeMemoryLegacyHandler(header.pid, requestPacket.header.deviceId, header.seqId))      \
    X(ViewUsedMemoryLegacy, _viewUsedMemoryLegacyHandler, IPCPacketViewUsedMemoryRequestLegacy,      \
        _viewUsedMemoryLegacyHandler(header.pid, requestPacket.header.deviceId, header.seqId))      \
    X(GetUsageLegacy,       _getUsageLegacyHandler,       IPCPacketGetUsageRequestLegacy,            \
        _getUsageLegacyHandler(header.pid, requestPacket.header.deviceId, header.seqId,             \
            requestPacket.channel))                                                                  \
    X(GetDeviceTelemetry,   _getDeviceTelemetryHandler,   IPCPacketGetDeviceTelemetryRequest,        \
        _getDeviceTelemetryHandler(header.pid, requestPacket.header.deviceId, header.seqId))         \
    X(RecoveryStartAck,     _recoveryStartAckHandler,     IPCPacketRecoveryStartAck,                  \
        _recoveryStartAckHandler(header.pid, requestPacket.header.deviceId, header.seqId,            \
            requestPacket.recoveryId, requestPacket.result))
// clang-format on

#define X(funcSuffix, member, ReqPacket, handlerCall)                                              \
int IPCPacketHandlerRegistry::handle##funcSuffix##Request(                                         \
    int clientFd, const IPCPacketHeader &header, const uint8_t *payload, size_t payloadSize) const \
{                                                                                                   \
    (void)clientFd;                                                                                \
    if (member == nullptr) { errno = ENOSYS; return -1; }                                         \
    ReqPacket requestPacket{};                                                                     \
    if (parseRequestPacket(&requestPacket, header, payload, payloadSize) != 0) return -1;          \
    return handlerCall;                                                                            \
}
DXRT_IPC_HANDLER_IMPL_LIST(X)
#undef X
#undef DXRT_IPC_HANDLER_IMPL_LIST

int IPCPacketHandlerRegistry::handle(
    int clientFd,
    const uint8_t *message,
    size_t messageSize,
    int receivedFd)
{
    std::ignore = receivedFd;  // Currently not used

    if (message == nullptr)
    {
        errno = EINVAL;
        LOG_DXRT_S_ERR("IPCPacketHandlerRegistry::handle invalid arguments");
        return -1;
    }

    if (messageSize < sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        LOG_DXRT_S_ERR("IPCPacketHandlerRegistry::handle short message: size=" + std::to_string(messageSize));
        return -1;
    }

    const auto* header = SafeCast::BytePtrToPtr<const IPCPacketHeader*>(message);
    if (!isValidIPCPacketHeader(*header))
    {
        errno = EPROTO;
        LOG_DXRT_S_ERR(
            "IPCPacketHandlerRegistry::handle invalid header: type=" + std::to_string(header->type) +
            ", seqId=" + std::to_string(header->seqId) +
            ", messageSize=" + std::to_string(header->messageSize));
        return -1;
    }

    const bool wasRegistered = (_server != nullptr && _server->getPidForClient(clientFd) == 0);
    if (_server != nullptr)
    {
        _server->bindPidToClient(header->pid, clientFd);
    }

    bool registered = wasRegistered;
    if (registered)
    {
        LOG_DXRT_S_DBG << "IPCPacketHandlerRegistry::handle registered new client: clientFd=" << clientFd
                       << ", pid=" << header->pid << std::endl;
        if (_onClientConnectedPid != nullptr)
        {
            _onClientConnectedPid(header->pid);
        }
    }

    LOG_DXRT_S_DBG << "IPCPacketHandlerRegistry::handle clientFd=" << clientFd
                   << ", pid=" << header->pid
                   << ", type=" << header->type
                   << ", seqId=" << header->seqId
                   << ", messageSize=" << messageSize << std::endl;

    const size_t payloadSize = messageSize - sizeof(IPCPacketHeader);
    if (header->messageSize != payloadSize)
    {
        errno = EMSGSIZE;
        LOG_DXRT_S_ERR(
            "IPCPacketHandlerRegistry::handle payload size mismatch: type=" + std::to_string(header->type) +
            ", header=" + std::to_string(header->messageSize) +
            ", actual=" + std::to_string(payloadSize));
        return -1;
    }

    const uint8_t *payload = message + sizeof(IPCPacketHeader);

    switch (static_cast<IPCMessageType>(header->type))
    {
        case IPCMessageType::ClientHello:
            // PID is already bound above; no response needed.
            return 0;
#define X(messageType, methodSuffix, handlerType, member, dispatchCall) \
        case IPCMessageType::messageType: \
            return dispatchCall;
        DXRT_IPC_REQUEST_HANDLER_LIST(X)
#undef X
        default:
            errno = ENOSYS;
            LOG_DXRT_S_ERR("IPCPacketHandlerRegistry::handle unsupported type=" + std::to_string(header->type));
            return -1;
    }
}

void IPCPacketHandlerRegistry::onClientAccepted(int clientFd)
{
    LOG_DXRT_S_DBG << "IPCPacketHandlerRegistry::onClientAccepted clientFd=" << clientFd << std::endl;
}

void IPCPacketHandlerRegistry::onClientDisconnected(int clientFd, pid_t pid)
{
    // do not lookup pid because they are already erased

    LOG_DXRT_S_DBG << "IPCPacketHandlerRegistry::onClientDisconnected clientFd=" << clientFd
                   << ", pid=" << pid << std::endl;
    if (_onClientDisconnectedPid != nullptr && pid > 0)
    {
        _onClientDisconnectedPid(pid);
    }
    else if (pid <= 0)
    {
        LOG_DXRT_S << "IPCPacketHandlerRegistry::onClientDisconnected no pid associated with clientFd=" << clientFd <<", pid=" << pid << std::endl;
    }
    else
    {
        LOG_DXRT_S << "IPCPacketHandlerRegistry::onClientDisconnected no handler for pid=" << pid << std::endl;
    }
}

IPCPacketHandlerRegistry::ServerOnReceiveHandler IPCPacketHandlerRegistry::createOnReceiveHandler()
{
    return [this](
        int clientFd,
        const uint8_t *message,
        size_t messageSize,
        int receivedFd) {
            return handle(clientFd, message, messageSize, receivedFd);
    };
}

IPCPacketHandlerRegistry::ServerOnClientConnectedHandler IPCPacketHandlerRegistry::createOnClientConnectedHandler()
{
    return [this](int clientFd) {
        onClientAccepted(clientFd);
    };
}

IPCServerEndpoint::OnClientDisconnectedHandler
IPCPacketHandlerRegistry::createOnClientDisconnectedHandler()
{
    return [this](int clientFd, pid_t pid) {
        (void)pid;  // pid already handled via _server->unbindClient
        onClientDisconnected(clientFd, pid);
    };
}

void IPCPacketHandlerRegistry::attachToServer(IPCServerEndpoint *server)
{
    _server = server;
    if (_server == nullptr)
    {
        return;
    }

    _server->setOnReceive(createOnReceiveHandler());
    _server->setOnClientConnected(createOnClientConnectedHandler());
    _server->setOnClientDisconnected(createOnClientDisconnectedHandler());
}

#undef DXRT_IPC_REQUEST_HANDLER_LIST

}  // namespace dxrt
