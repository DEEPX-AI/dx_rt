/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_packet.hpp"

#include <atomic>
#include <cerrno>
#include <cstring>

#ifdef __linux__
#include <unistd.h>
#elif defined(_WIN32)
#include <processthreadsapi.h>
#endif

namespace dxrt {

namespace packet_internal {

std::atomic<int32_t> &packetSequenceIdCounter()
{
    static std::atomic<int32_t> counter{1};
    return counter;
}

int32_t normalizeSequenceId(int32_t sequenceId)
{
    if (sequenceId <= 0)
    {
        return 1;
    }
    return sequenceId;
}

}  // namespace packet_internal

int32_t getNextIPCPacketSequenceId()
{
    int32_t nextSequence = packet_internal::packetSequenceIdCounter().fetch_add(1, std::memory_order_relaxed);
    if (nextSequence > 0)
    {
        return nextSequence;
    }

    int32_t expected = nextSequence + 1;
    if (packet_internal::packetSequenceIdCounter().compare_exchange_strong(expected, 1, std::memory_order_relaxed))
    {
        return 1;
    }

    nextSequence = packet_internal::packetSequenceIdCounter().fetch_add(1, std::memory_order_relaxed);
    return packet_internal::normalizeSequenceId(nextSequence);
}

pid_t getCurrentIPCPacketPid()
{
#ifdef __linux__
    return ::getpid();
#elif defined(_WIN32)
    return static_cast<pid_t>(::GetCurrentProcessId());
#else
    return 0;
#endif
}

bool isValidIPCMessageType(uint32_t typeValue)
{
    switch (static_cast<IPCMessageType>(typeValue))
    {
#define X(name, value) case IPCMessageType::name: return true;
        DXRT_IPC_MESSAGE_TYPE_LIST(X)
#undef X
        default:
            return false;
    }
}

bool tryGetIPCResponseMessageType(IPCMessageType requestType, IPCMessageType *responseType)
{
    if (responseType == nullptr)
    {
        return false;
    }

    switch (requestType)
    {
#define X(requestTypeName, responseTypeName) \
        case IPCMessageType::requestTypeName: \
            *responseType = IPCMessageType::responseTypeName; \
            return true;
        DXRT_IPC_REQUEST_RESPONSE_TYPE_LIST(X)
#undef X
        default:
            return false;
    }
}

bool isValidIPCPacketHeader(const IPCPacketHeader &header)
{
    if (header.magic != kIPCPacketMagic)
    {
        return false;
    }

    if (!isValidIPCMessageType(header.type))
    {
        return false;
    }

    if (header.version != kIPCPacketWireVersion)
    {
        return false;
    }

    if (header.seqId < 0)
    {
        return false;
    }

    return true;
}

size_t getSerializedIPCDeviceTelemetryResponseSize(const IPCDeviceTelemetryPayload &telemetry)
{
    const size_t coreCount = telemetry.cores.size();
    return sizeof(IPCPacketGetDeviceTelemetryResponse)
        + (sizeof(uint32_t) * coreCount)
        + (sizeof(int32_t) * coreCount)
        + (sizeof(uint32_t) * coreCount)
        + (sizeof(uint32_t) * coreCount);
}

int serializeIPCDeviceTelemetryResponsePacket(
    const IPCPacketHeader &requestHeader,
    const IPCDeviceTelemetryPayload &telemetry,
    std::vector<uint8_t> *response)
{
    if (response == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    const size_t totalSize = getSerializedIPCDeviceTelemetryResponseSize(telemetry);
    if (totalSize > kIPCMaxMessageSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    IPCPacketGetDeviceTelemetryResponse responsePacket{};
    initializeIPCPacketHeader(
        &responsePacket.header,
        IPCMessageType::GetDeviceTelemetryResponse,
        static_cast<uint32_t>(totalSize - sizeof(IPCPacketHeader)),
        requestHeader.seqId,
        requestHeader.pid,
        kIPCPacketWireVersion);
    responsePacket.header.deviceId = requestHeader.deviceId;
    responsePacket.result = telemetry.result;
    responsePacket.coreCount = static_cast<uint32_t>(telemetry.cores.size());
    responsePacket.usedMemoryBytes = telemetry.usedMemoryBytes;
    responsePacket.freeMemoryBytes = telemetry.freeMemoryBytes;

    response->assign(totalSize, 0);
    std::memcpy(response->data(), &responsePacket, sizeof(responsePacket));

    uint8_t *cursor = response->data() + sizeof(responsePacket);
    for (const auto &core : telemetry.cores)
    {
        std::memcpy(cursor, &core.utilizationPermille, sizeof(core.utilizationPermille));
        cursor += sizeof(core.utilizationPermille);
    }
    for (const auto &core : telemetry.cores)
    {
        std::memcpy(cursor, &core.temperature, sizeof(core.temperature));
        cursor += sizeof(core.temperature);
    }
    for (const auto &core : telemetry.cores)
    {
        std::memcpy(cursor, &core.clock, sizeof(core.clock));
        cursor += sizeof(core.clock);
    }
    for (const auto &core : telemetry.cores)
    {
        std::memcpy(cursor, &core.voltage, sizeof(core.voltage));
        cursor += sizeof(core.voltage);
    }

    return 0;
}

int deserializeIPCDeviceTelemetryResponsePacket(
    const uint8_t *message,
    size_t messageSize,
    IPCPacketHeader *header,
    IPCDeviceTelemetryPayload *telemetry)
{
    if (message == nullptr || header == nullptr || telemetry == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (messageSize < sizeof(IPCPacketGetDeviceTelemetryResponse))
    {
        errno = EMSGSIZE;
        return -1;
    }

    IPCPacketGetDeviceTelemetryResponse responsePacket{};
    std::memcpy(&responsePacket, message, sizeof(responsePacket));
    *header = responsePacket.header;

    if (!isValidIPCPacketHeader(responsePacket.header))
    {
        errno = EPROTO;
        return -1;
    }

    if (responsePacket.header.type != static_cast<uint32_t>(IPCMessageType::GetDeviceTelemetryResponse))
    {
        errno = EPROTO;
        return -1;
    }

    const size_t coreCount = responsePacket.coreCount;
    const size_t expectedSize = sizeof(IPCPacketGetDeviceTelemetryResponse)
        + (sizeof(uint32_t) * coreCount)
        + (sizeof(int32_t) * coreCount)
        + (sizeof(uint32_t) * coreCount)
        + (sizeof(uint32_t) * coreCount);
    if (messageSize != expectedSize)
    {
        errno = EMSGSIZE;
        return -1;
    }

    if (responsePacket.header.messageSize != expectedSize - sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        return -1;
    }

    telemetry->result = responsePacket.result;
    telemetry->usedMemoryBytes = responsePacket.usedMemoryBytes;
    telemetry->freeMemoryBytes = responsePacket.freeMemoryBytes;
    telemetry->cores.assign(coreCount, IPCDeviceTelemetryCoreData{});

    const uint8_t *cursor = message + sizeof(responsePacket);
    for (size_t index = 0; index < coreCount; ++index)
    {
        std::memcpy(&telemetry->cores[index].utilizationPermille, cursor, sizeof(uint32_t));
        cursor += sizeof(uint32_t);
    }
    for (size_t index = 0; index < coreCount; ++index)
    {
        std::memcpy(&telemetry->cores[index].temperature, cursor, sizeof(int32_t));
        cursor += sizeof(int32_t);
    }
    for (size_t index = 0; index < coreCount; ++index)
    {
        std::memcpy(&telemetry->cores[index].clock, cursor, sizeof(uint32_t));
        cursor += sizeof(uint32_t);
    }
    for (size_t index = 0; index < coreCount; ++index)
    {
        std::memcpy(&telemetry->cores[index].voltage, cursor, sizeof(uint32_t));
        cursor += sizeof(uint32_t);
    }

    return 0;
}

bool isValidIPCPacketHeader(const IPCPacketHeader &header, IPCMessageType expectedType, size_t expectedPayloadSize)
{
    if (!isValidIPCPacketHeader(header))
    {
        return false;
    }

    if (header.type != static_cast<uint32_t>(expectedType))
    {
        return false;
    }

    return header.messageSize == expectedPayloadSize;
}

const char *toString(IPCMessageType type)
{
    switch (type)
    {
#define X(name, value) case IPCMessageType::name: return #name;
        DXRT_IPC_MESSAGE_TYPE_LIST(X)
#undef X
        default:
            return "Unknown";
    }
}

void initializeIPCPacketHeader(
    IPCPacketHeader *header,
    IPCMessageType type,
    uint32_t messageSize,
    int32_t seqId,
    pid_t pid,
    uint16_t version)
{
    if (header == nullptr)
    {
        return;
    }

    if (pid == 0)
    {
        pid = getCurrentIPCPacketPid();
    }

    header->magic = kIPCPacketMagic;
    header->version = version;
    header->flags = 0;
    header->type = static_cast<uint32_t>(type);
    header->messageSize = messageSize;
    header->seqId = seqId;
    header->deviceId = 0;
    header->pid = pid;
}

}  // namespace dxrt
