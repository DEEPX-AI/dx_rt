/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_packet_client.hpp"

#include <chrono>
#include <cerrno>
#include <cstring>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <algorithm>

#include "../shm/shared_memory_syscall_adapter.h"
#include "dxrt/common.h"
#include "dxrt/dynamic_ipc_endpoint.h"
#include "dxrt/safe_cast.h"

namespace dxrt {

int IPCPacketClient::connectToServer(const std::string &socketPath, int retryCount, int retryDelayMs)
{
    LOG_DXRT_DBG << "IPCPacketClient connectToServer endpoint=" << GetDynamicIpcEndpointForLog(socketPath)
                 << ", retryCount=" << retryCount
                 << ", retryDelayMs=" << retryDelayMs << std::endl;
    const int rc = _client.connectToServer(socketPath, retryCount, retryDelayMs);
    if (rc != 0)
    {
        LOG_DXRT_DBG << "IPCPacketClient connectToServer failed errno=" << errno << std::endl;
        return rc;
    }

    _client.setOnReceive([this](const IPCClientReceivedMessage &message) {
        onClientReceived(message);
    });

    // Introduce ourselves so the server can bind clientFd → pid immediately.
    IPCPacketClientHello hello{};
    initializeIPCPacket(&hello, IPCMessageType::ClientHello);
    hello.header.pid = getCurrentIPCPacketPid();
    _client.sendMessageAsync(SafeCast::PtrToBytePtr(&hello), sizeof(hello), -1);

    LOG_DXRT_DBG << "IPCPacketClient connectToServer success" << std::endl;
    return 0;
}

void IPCPacketClient::close()
{
    _client.setOnReceive(nullptr);
    _client.close();
}

void IPCPacketClient::setThrottleNotificationHandler(ThrottleNotificationHandler handler)
{
    _throttleHandler = std::move(handler);
}

void IPCPacketClient::setErrorNotificationHandler(ErrorNotificationHandler handler)
{
    _errorHandler = std::move(handler);
}

void IPCPacketClient::setInferenceResponseHandler(InferenceResponseHandler handler)
{
    _inferenceHandler = std::move(handler);
}

void IPCPacketClient::setRecoveryStartNotificationHandler(RecoveryStartNotificationHandler handler)
{
    _recoveryStartNotificationHandler = std::move(handler);
}

int IPCPacketClient::receiveAndDispatch(int *receivedFd) const
{
    if (receivedFd != nullptr)
    {
        *receivedFd = -1;
    }
    return 0;
}

int IPCPacketClient::Allocate(
    int deviceId,
    uint32_t bufferSize,
    int taskId,
    MemoryType memoryType,
    AllocateResult *result,
    intptr_t *responseFd)
{
    if (result == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    IPCPacketRequestAllocateBuffer request{};
    IPCPacketResponseAllocateBuffer packetResponse{};
    request.bufferSize = bufferSize;
    request.taskId = taskId;
    request.memoryType = memoryType;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::AllocateBuffer,
        &packetResponse,
        deviceId,
        -1,
        responseFd);
    if (rc != 0)
    {
        return rc;
    }

    result->result = packetResponse.result;
    result->bufferAddress = packetResponse.bufferAddress;
    result->bufferSize = packetResponse.bufferSize;
    result->blockId = packetResponse.blockId;
    result->deviceId = packetResponse.header.deviceId;

    // fd is transferred via SCM_RIGHTS on Linux and via an inline packet field (DuplicateHandle'd HANDLE)
    // on Windows.
    // On Linux, packetResponse.fd is always kInvalidMemFDHandle(-1), so this branch is never executed.
    if (responseFd != nullptr && packetResponse.fd != dxrt::shm::kInvalidMemFDHandle)
    {
        *responseFd = packetResponse.fd;
    }

    return 0;
}

int IPCPacketClient::Deallocate(
    int deviceId,
    uint64_t bufferAddress)
{
    IPCPacketRequestFreeBuffer request{};
    IPCPacketResponseFreeBuffer packetResponse{};
    request.bufferAddress = bufferAddress;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::FreeBuffer,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}

int IPCPacketClient::InferenceRequest(
    int deviceId,
    int taskId,
    const dxrt_request_acc_t &requestData,
    InferenceResult *result)
{
    if (result == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    IPCPacketRequestInference request{};
    IPCPacketResponseInference packetResponse{};
    request.taskId = taskId;
    request.request = requestData;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::InferenceRequest,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    result->result = packetResponse.result;
    result->response = packetResponse.response;
    result->deviceId = packetResponse.header.deviceId;
    return 0;
}

int IPCPacketClient::InferenceRequestAsync(
    int deviceId,
    int taskId,
    const dxrt_request_acc_t &requestData)
{
    IPCPacketRequestInference request{};
    request.taskId = taskId;
    request.request = requestData;
    initializeIPCPacket(&request, IPCMessageType::InferenceRequest);
    request.header.deviceId = deviceId;

    const int sequenceId = _client.sendMessageAsync(
        SafeCast::PtrToBytePtr(&request),
        sizeof(request),
        -1);

    if (sequenceId < 0)
    {
        LOG_DXRT_ERR(
            "IPCPacketClient::InferenceRequestAsync send failed: deviceId=" +
            std::to_string(deviceId) +
            ", taskId=" + std::to_string(taskId) +
            ", errno=" + std::to_string(errno) +
            " (" + std::strerror(errno) + ")");
        return -1;
    }
    return 0;
}

int IPCPacketClient::DMARead(
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    IPCPacketDMAReadRequest request{};
    IPCPacketDMAReadResponse packetResponse{};
    request.blockId = blockId;
    request.blockOffset = blockOffset;
    request.size = size;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::DMAReadRequest,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}

int IPCPacketClient::DMAWrite(
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    IPCPacketDMAWriteRequest request{};
    IPCPacketDMAWriteResponse packetResponse{};
    request.blockId = blockId;
    request.blockOffset = blockOffset;
    request.size = size;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::DMAWriteRequest,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}

int IPCPacketClient::TaskInit(
    int deviceId,
    int taskId,
    int bound,
    uint64_t modelMemorySize,
    const TaskStaticConfig &config)
{
    IPCPacketTaskInitRequest request{};
    IPCPacketTaskInitResponse packetResponse{};
    request.taskId = taskId;
    request.bound = bound;
    request.modelMemorySize = modelMemorySize;
    request.model_type         = config.model_type;
    request.model_format       = config.model_format;
    request.model_cmds         = config.model_cmds;
    request.op_mode            = config.op_mode;
    request.cmd_offset         = config.cmd_offset;
    request.weight_offset      = config.weight_offset;
    request.custom_offset      = config.custom_offset;
    request.input_size         = config.input_size;
    request.output_size        = config.output_size;
    request.last_output_offset = config.last_output_offset;
    request.output_all_offset  = config.output_all_offset;
    static_assert(sizeof(request.datas) == sizeof(config.datas), "datas size mismatch");
    std::memcpy(request.datas, config.datas, sizeof(config.datas));
    request.memory_base        = config.memory_base;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::TaskInitRequest,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}

int IPCPacketClient::TaskDeInit(
    int deviceId,
    int taskId,
    int bound)
{
    IPCPacketTaskDeInitRequest request{};
    IPCPacketTaskDeInitResponse packetResponse{};
    request.taskId = taskId;
    request.bound = bound;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::TaskDeInitRequest,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}

int IPCPacketClient::ViewFreeMemoryLegacy(
    int deviceId,
    uint64_t *outBytes)
{
    IPCPacketViewFreeMemoryRequestLegacy request{};
    IPCPacketViewFreeMemoryResponseLegacy packetResponse{};
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::ViewFreeMemoryRequestLegacy,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    if (outBytes != nullptr)
    {
        *outBytes = packetResponse.bytes;
    }
    return packetResponse.result;
}

int IPCPacketClient::ViewUsedMemoryLegacy(
    int deviceId,
    uint64_t *outBytes)
{
    IPCPacketViewUsedMemoryRequestLegacy request{};
    IPCPacketViewUsedMemoryResponseLegacy packetResponse{};
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::ViewUsedMemoryRequestLegacy,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    if (outBytes != nullptr)
    {
        *outBytes = packetResponse.bytes;
    }
    return packetResponse.result;
}

int IPCPacketClient::GetUsageLegacy(
    int deviceId,
    int channel,
    double *outUsage)
{
    IPCPacketGetUsageRequestLegacy request{};
    IPCPacketGetUsageResponseLegacy packetResponse{};
    request.channel = channel;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::GetUsageRequestLegacy,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    if (outUsage != nullptr)
    {
        *outUsage = packetResponse.usage;
    }
    return packetResponse.result;
}

int IPCPacketClient::GetDeviceTelemetry(
    int deviceId,
    DeviceTelemetryResult *response)
{
    if (response == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    IPCPacketGetDeviceTelemetryRequest request{};
    std::vector<uint8_t> responseBytes;
    const int rc = sendRequestAndWaitForResponseBytes(
        &request,
        IPCMessageType::GetDeviceTelemetryRequest,
        deviceId,
        -1,
        nullptr,
        &responseBytes);
    if (rc != 0)
    {
        return rc;
    }

    IPCPacketHeader responseHeader{};
    IPCDeviceTelemetryPayload telemetry{};
    if (deserializeIPCDeviceTelemetryResponsePacket(
            responseBytes.data(),
            responseBytes.size(),
            &responseHeader,
            &telemetry) != 0)
    {
        return -1;
    }

    response->result = telemetry.result;
    response->coreCount = static_cast<uint32_t>(telemetry.cores.size());
    response->usedMemoryBytes = telemetry.usedMemoryBytes;
    response->freeMemoryBytes = telemetry.freeMemoryBytes;
    response->cores = telemetry.cores;
    std::fill(response->utilizationPermille.begin(), response->utilizationPermille.end(), 0U);
    std::fill(response->temperature.begin(), response->temperature.end(), 0);
    std::fill(response->clock.begin(), response->clock.end(), 0U);
    std::fill(response->voltage.begin(), response->voltage.end(), 0U);
    const size_t legacyCount = (std::min)(response->cores.size(), response->utilizationPermille.size());
    for (size_t index = 0; index < legacyCount; ++index)
    {
        response->utilizationPermille[index] = response->cores[index].utilizationPermille;
        response->temperature[index] = response->cores[index].temperature;
        response->clock[index] = response->cores[index].clock;
        response->voltage[index] = response->cores[index].voltage;
    }
    return 0;
}

template <typename RequestPacket, typename ResponsePacket>
int IPCPacketClient::sendRequestAndWaitForResponse(
    RequestPacket *request,
    IPCMessageType requestType,
    ResponsePacket *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd)
{
    if (response == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    std::vector<uint8_t> responseBytes;
    const int rc = sendRequestAndWaitForResponseBytes(
        request,
        requestType,
        deviceId,
        requestFd,
        responseFd,
        &responseBytes);
    if (rc != 0)
    {
        return rc;
    }

    if (responseBytes.size() != sizeof(ResponsePacket))
    {
        errno = EMSGSIZE;
        LOG_DXRT_ERR(
            "IPCPacketClient received unexpected response size: expected=" +
            std::to_string(sizeof(ResponsePacket)) + ", actual=" + std::to_string(responseBytes.size()));
        return -1;
    }

    std::memcpy(response, responseBytes.data(), sizeof(ResponsePacket));
    return 0;
}

template <typename RequestPacket>
int IPCPacketClient::sendRequestAndWaitForResponseBytes(
    RequestPacket *request,
    IPCMessageType requestType,
    int deviceId,
    int requestFd,
    intptr_t *responseFd,
    std::vector<uint8_t> *responseBytes)
{
    if (request == nullptr || responseBytes == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    IPCMessageType responseType = IPCMessageType::ErrorNotification;
    if (!tryGetIPCResponseMessageType(requestType, &responseType))
    {
        errno = EINVAL;
        return -1;
    }

    // seqId is assigned by IPCClientEndpoint::sendMessageAsync — initialize packet without it
    initializeIPCPacket(request, requestType);
    request->header.deviceId = deviceId;

    LOG_DXRT_DBG << "sendRequestAndWaitForResponse requestType=" << static_cast<uint32_t>(requestType)
                 << ", responseType=" << static_cast<uint32_t>(responseType)
                 << ", deviceId=" << deviceId
                 << ", requestBytes=" << sizeof(RequestPacket)
                 << ", maxMessageSize=" << kMaxMessageSize << std::endl;

    if (sizeof(RequestPacket) > kMaxMessageSize)
    {
        errno = EMSGSIZE;
        LOG_DXRT_ERR(
            "IPCPacketClient packet size exceeds limit: requestBytes=" +
            std::to_string(sizeof(RequestPacket)) +
            ", maxMessageSize=" + std::to_string(kMaxMessageSize));
        return -1;
    }

    IPCClientReceivedMessage clientResponse;
    const int32_t sequenceId = _client.sendMessageSync(
        SafeCast::PtrToBytePtr(request),
        sizeof(RequestPacket),
        requestFd,
        &clientResponse);
    if (sequenceId < 0)
    {
        LOG_DXRT_ERR(
            "IPCPacketClient::sendRequestAndWaitForResponse sendMessageSync failed: requestType=" +
            std::to_string(static_cast<uint32_t>(requestType)) +
            ", deviceId=" + std::to_string(deviceId) +
            ", errno=" + std::to_string(errno) +
            " (" + std::strerror(errno) + ")");
        return -1;
    }

    if (responseFd != nullptr)
    {
        *responseFd = static_cast<intptr_t>(clientResponse.fd);
    }

    LOG_DXRT_DBG << "sendRequestAndWaitForResponse response seqId=" << sequenceId
                 << ", bytes=" << clientResponse.data.size()
                 << ", fd=" << clientResponse.fd << std::endl;

    const size_t messageSize = clientResponse.data.size();
    if (messageSize > kMaxMessageSize)
    {
        errno = EMSGSIZE;
        LOG_DXRT_ERR(
            "IPCPacketClient received oversized response: size=" +
            std::to_string(messageSize) +
            ", maxMessageSize=" + std::to_string(kMaxMessageSize));
        return -1;
    }

    if (messageSize < sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        LOG_DXRT_ERR("IPCPacketClient received short response: size=" + std::to_string(messageSize));
        return -1;
    }

    const auto* header = SafeCast::BytePtrToPtr<const IPCPacketHeader*>(clientResponse.data.data());
    if (!isValidIPCPacketHeader(*header))
    {
        errno = EPROTO;
        LOG_DXRT_ERR("IPCPacketClient received invalid response header");
        return -1;
    }

    if (header->type != static_cast<uint32_t>(responseType) || header->seqId != sequenceId)
    {
        errno = EPROTO;
        LOG_DXRT_ERR(
            "IPCPacketClient received mismatched response: expectedType=" +
            std::to_string(static_cast<uint32_t>(responseType)) +
            ", actualType=" + std::to_string(header->type) +
            ", expectedSeqId=" + std::to_string(sequenceId) +
            ", actualSeqId=" + std::to_string(header->seqId));
        return -1;
    }

    *responseBytes = std::move(clientResponse.data);
    LOG_DXRT_DBG << "sendRequestAndWaitForResponse completed seqId=" << sequenceId
                 << ", responseType=" << header->type << std::endl;
    return 0;
}

int IPCPacketClient::dispatchIncomingPacket(
    const uint8_t *message,
    size_t messageSize,
    int receivedFd,
    bool *dispatched) const
{
    if (dispatched == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    *dispatched = false;

    if (message == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (messageSize < sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        return -1;
    }

    const auto* header =
        SafeCast::BytePtrToPtr<const IPCPacketHeader*>(message);
    if (!isValidIPCPacketHeader(*header))
    {
        errno = EPROTO;
        return -1;
    }

    const size_t payloadSize = messageSize - sizeof(IPCPacketHeader);
    if (header->messageSize != payloadSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    const uint8_t *payload = message + sizeof(IPCPacketHeader);
    switch (static_cast<IPCMessageType>(header->type))
    {
        case IPCMessageType::ThrottleNotification:
            if (!_throttleHandler)
            {
                return 0;
            }
            *dispatched = true;
            return handleThrottleNotification(*header, payload, payloadSize, receivedFd);

        case IPCMessageType::ErrorNotification:
            if (!_errorHandler)
            {
                return 0;
            }
            *dispatched = true;
            return handleErrorNotification(*header, payload, payloadSize, receivedFd);

        case IPCMessageType::InferenceResponse:
            if (!_inferenceHandler)
            {
                return 0;
            }
            *dispatched = true;
            return handleInferenceResponse(*header, payload, payloadSize, receivedFd);

        case IPCMessageType::RecoveryStartNotification:
            *dispatched = true;
            return handleRecoveryStartNotification(*header, payload, payloadSize, receivedFd);

        default:
            return 0;
    }
}

void IPCPacketClient::onClientReceived(const IPCClientReceivedMessage &message) const
{
    bool dispatched = false;
    const int dispatchRc = dispatchIncomingPacket(message.data.data(),
        message.data.size(), message.fd, &dispatched);
    if (dispatchRc < 0)
    {
        LOG_DXRT_ERR(
            "IPCPacketClient onReceive dispatch failed: rc=" + std::to_string(dispatchRc) +
            ", errno=" + std::to_string(errno) +
            " (" + std::string(std::strerror(errno)) + ")" +
            ", bytes=" + std::to_string(message.data.size()) +
            ", fd=" + std::to_string(message.fd));
    }
}

int IPCPacketClient::handleThrottleNotification(
    const IPCPacketHeader &header,
    const uint8_t *payload,
    size_t payloadSize,
    int) const
{
    constexpr size_t kExpectedPayloadSize =
        sizeof(IPCPacketThrottleNotification) - sizeof(IPCPacketHeader);
    if (payload == nullptr || payloadSize != kExpectedPayloadSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    if (!_throttleHandler)
    {
        return -1;
    }

    IPCPacketThrottleNotification packet{};
    packet.header = header;
    std::memcpy(
        SafeCast::PtrToBytePtr(&packet) + sizeof(IPCPacketHeader),
        payload,
        payloadSize);

    return _throttleHandler(header.deviceId, packet.throtInfo);
}

int IPCPacketClient::handleErrorNotification(
    const IPCPacketHeader &header,
    const uint8_t *payload,
    size_t payloadSize,
    int) const
{
    constexpr size_t kExpectedPayloadSize =
        sizeof(IPCPacketErrorNotification) - sizeof(IPCPacketHeader);
    if (payload == nullptr || payloadSize != kExpectedPayloadSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    if (!_errorHandler)
    {
        return -1;
    }

    IPCPacketErrorNotification packet{};
    packet.header = header;
    std::memcpy(
        SafeCast::PtrToBytePtr(&packet) + sizeof(IPCPacketHeader),
        payload,
        payloadSize);

    return _errorHandler(
        header.deviceId,
        static_cast<dxrt_server_err_t>(packet.serverErr),
        packet.errorInfo);
}

int IPCPacketClient::handleInferenceResponse(
    const IPCPacketHeader &header,
    const uint8_t *payload,
    size_t payloadSize,
    int) const
{
    constexpr size_t kExpectedPayloadSize =
        sizeof(IPCPacketResponseInference) - sizeof(IPCPacketHeader);
    if (payload == nullptr || payloadSize != kExpectedPayloadSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    if (!_inferenceHandler)
    {
        return -1;
    }

    IPCPacketResponseInference packet{};
    packet.header = header;
    std::memcpy(
        SafeCast::PtrToBytePtr(&packet) + sizeof(IPCPacketHeader),
        payload,
        payloadSize);

    InferenceResult dto{};
    dto.result = packet.result;
    dto.response = packet.response;
    dto.deviceId = header.deviceId;
    return _inferenceHandler(dto);
}

int IPCPacketClient::handleRecoveryStartNotification(
    const IPCPacketHeader &header,
    const uint8_t *payload,
    size_t payloadSize,
    int) const
{
    constexpr size_t kExpectedPayloadSize =
        sizeof(IPCPacketRecoveryStartNotification) - sizeof(IPCPacketHeader);
    if (payload == nullptr || payloadSize != kExpectedPayloadSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    IPCPacketRecoveryStartNotification packet{};
    packet.header = header;
    std::memcpy(
        SafeCast::PtrToBytePtr(&packet) + sizeof(IPCPacketHeader),
        payload,
        payloadSize);

    if (_recoveryStartNotificationHandler != nullptr)
    {
        (void)_recoveryStartNotificationHandler(header.deviceId, packet.recoveryId);
    }

    return 0;
}

int IPCPacketClient::sendRecoveryStartAck(
    int deviceId,
    uint32_t recoveryId,
    int result)
{
    IPCPacketRecoveryStartAck ack{};
    initializeIPCPacket(&ack, IPCMessageType::RecoveryStartAck);
    ack.header.deviceId = deviceId;
    ack.recoveryId = recoveryId;
    ack.result = result;

    const int sequenceId = _client.sendMessageAsync(
        SafeCast::PtrToBytePtr(&ack),
        sizeof(ack),
        -1);
    if (sequenceId < 0)
    {
        LOG_DXRT_ERR(
            "IPCPacketClient::sendRecoveryStartAck failed: deviceId=" +
            std::to_string(deviceId) +
            ", recoveryId=" + std::to_string(recoveryId) +
            ", result=" + std::to_string(result) +
            ", errno=" + std::to_string(errno) +
            " (" + std::strerror(errno) + ")");
        return -1;
    }

    return 0;
}
int IPCPacketClient::DMAReadWithFaultInjection(
    int deviceId,
    int64_t  blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    IPCPacketDMAReadRequestWithFaultInjection request{};
    IPCPacketDMAReadResponse packetResponse{};
    request.blockId = blockId;
    request.blockOffset = blockOffset;
    request.size = size;
    const int rc = sendRequestAndWaitForResponse(
        &request,
        IPCMessageType::DMAReadRequestWithFaultInjection,
        &packetResponse,
        deviceId,
        -1,
        nullptr);
    if (rc != 0)
    {
        return rc;
    }

    return packetResponse.result;
}


template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketRequestAllocateBuffer,
    IPCPacketResponseAllocateBuffer>(
    IPCPacketRequestAllocateBuffer *request,
    IPCMessageType requestType,
    IPCPacketResponseAllocateBuffer *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketRequestFreeBuffer,
    IPCPacketResponseFreeBuffer>(
    IPCPacketRequestFreeBuffer *request,
    IPCMessageType requestType,
    IPCPacketResponseFreeBuffer *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketRequestInference,
    IPCPacketResponseInference>(
    IPCPacketRequestInference *request,
    IPCMessageType requestType,
    IPCPacketResponseInference *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketDMAReadRequest,
    IPCPacketDMAReadResponse>(
    IPCPacketDMAReadRequest *request,
    IPCMessageType requestType,
    IPCPacketDMAReadResponse *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketDMAWriteRequest,
    IPCPacketDMAWriteResponse>(
    IPCPacketDMAWriteRequest *request,
    IPCMessageType requestType,
    IPCPacketDMAWriteResponse *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketTaskInitRequest,
    IPCPacketTaskInitResponse>(
    IPCPacketTaskInitRequest *request,
    IPCMessageType requestType,
    IPCPacketTaskInitResponse *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketTaskDeInitRequest,
    IPCPacketTaskDeInitResponse>(
    IPCPacketTaskDeInitRequest *request,
    IPCMessageType requestType,
    IPCPacketTaskDeInitResponse *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketViewFreeMemoryRequestLegacy,
    IPCPacketViewFreeMemoryResponseLegacy>(
    IPCPacketViewFreeMemoryRequestLegacy *request,
    IPCMessageType requestType,
    IPCPacketViewFreeMemoryResponseLegacy *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketViewUsedMemoryRequestLegacy,
    IPCPacketViewUsedMemoryResponseLegacy>(
    IPCPacketViewUsedMemoryRequestLegacy *request,
    IPCMessageType requestType,
    IPCPacketViewUsedMemoryResponseLegacy *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

template int IPCPacketClient::sendRequestAndWaitForResponse<
    IPCPacketGetUsageRequestLegacy,
    IPCPacketGetUsageResponseLegacy>(
    IPCPacketGetUsageRequestLegacy *request,
    IPCMessageType requestType,
    IPCPacketGetUsageResponseLegacy *response,
    int deviceId,
    int requestFd,
    intptr_t *responseFd);

}  // namespace dxrt
