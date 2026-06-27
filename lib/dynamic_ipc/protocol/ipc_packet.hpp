/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "ipc_protocol_limits.hpp"
#include "memory_type.hpp"

namespace dxrt {

#define DXRT_IPC_REQUEST_RESPONSE_TYPE_LIST(X) \
    X(AllocateBuffer, AllocateBuffer) \
    X(FreeBuffer, FreeBuffer) \
    X(InferenceRequest, InferenceResponse) \
    X(DMAReadRequest, DMAReadResponse) \
    X(DMAReadRequestWithFaultInjection, DMAReadResponse) /* intentionally reuses DMAReadResponse */ \
    X(DMAWriteRequest, DMAWriteResponse) \
    X(TaskInitRequest, TaskInitResponse) \
    X(TaskDeInitRequest, TaskDeInitResponse) \
    X(ViewFreeMemoryRequestLegacy, ViewFreeMemoryResponseLegacy) \
    X(ViewUsedMemoryRequestLegacy, ViewUsedMemoryResponseLegacy) \
    X(GetUsageRequestLegacy, GetUsageResponseLegacy) \
    X(GetDeviceTelemetryRequest, GetDeviceTelemetryResponse)

#define DXRT_IPC_MESSAGE_TYPE_LIST(X) \
    X(AllocateBuffer, 101) \
    X(FreeBuffer, 102) \
    X(InferenceRequest, 103) \
    X(InferenceResponse, 104) \
    X(ThrottleNotification, 105) \
    X(ErrorNotification, 106) \
    X(DMAReadRequest, 107) \
    X(DMAReadResponse, 108) \
    X(DMAWriteRequest, 109) \
    X(DMAWriteResponse, 110) \
    X(TaskInitRequest, 111) \
    X(TaskInitResponse, 112) \
    X(TaskDeInitRequest, 113) \
    X(TaskDeInitResponse, 114) \
    X(ViewFreeMemoryRequestLegacy, 115) \
    X(ViewFreeMemoryResponseLegacy, 116) \
    X(ViewUsedMemoryRequestLegacy, 117) \
    X(ViewUsedMemoryResponseLegacy, 118) \
    X(GetUsageRequestLegacy, 119) \
    X(GetUsageResponseLegacy, 120) \
    X(GetDeviceTelemetryRequest, 121) \
    X(GetDeviceTelemetryResponse, 122) \
    X(ClientHello, 123) \
    X(RecoveryStartNotification, 124) \
    X(RecoveryStartAck, 125) \
    X(RecoverySuccessNotification, 126) \
    X(RecoveryFailNotification, 127) \
    X(DMAReadRequestWithFaultInjection, 199)

enum class IPCMessageType : uint32_t
{
#define X(name, value) name = value,
    DXRT_IPC_MESSAGE_TYPE_LIST(X)
#undef X
};

static constexpr uint32_t kIPCPacketMagic = 0x44584950U;
static constexpr uint16_t kIPCPacketWireVersion = 1;
static constexpr uint16_t kIPCPacketCurrentVersion = kIPCPacketWireVersion;

template <IPCMessageType RequestType>
struct IPCResponseMessageType;

#define X(requestType, responseType) \
template <> \
struct IPCResponseMessageType<IPCMessageType::requestType> \
{ \
    static constexpr IPCMessageType value = IPCMessageType::responseType; \
};
DXRT_IPC_REQUEST_RESPONSE_TYPE_LIST(X)
#undef X

struct IPCDeviceTelemetryCoreData
{
    uint32_t utilizationPermille{0};
    int32_t temperature{0};
    uint32_t clock{0};
    uint32_t voltage{0};
};

struct IPCDeviceTelemetryPayload
{
    int result{0};
    uint64_t usedMemoryBytes{0};
    uint64_t freeMemoryBytes{0};
    std::vector<IPCDeviceTelemetryCoreData> cores;
};

#pragma pack(push, 1)

struct IPCPacketHeader
{
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    uint32_t type;
    uint32_t messageSize;
    int32_t seqId;
    int32_t deviceId;
    pid_t pid;
};

#define DXRT_IPC_PACKET_STRUCT_LIST(X) \
    X(IPCPacketClientHello, /* no payload */) \
    X(IPCPacketRecoveryStartNotification, \
        uint32_t recoveryId;) \
    X(IPCPacketRecoveryStartAck, \
        uint32_t recoveryId; \
        int result;) \
    X(IPCPacketRecoverySuccessNotification, \
        uint32_t recoveryId;) \
    X(IPCPacketRecoveryFailNotification, \
        uint32_t recoveryId;) \
    X(IPCPacketRequestAllocateBuffer, \
        uint64_t bufferSize; \
        int taskId; \
        dxrt::MemoryType memoryType;) \
    X(IPCPacketResponseAllocateBuffer, \
        uint64_t bufferAddress; \
        uint64_t bufferSize; \
        int result; \
        intptr_t fd; \
        int64_t blockId;) \
    X(IPCPacketRequestFreeBuffer, \
        uint64_t bufferAddress;) \
    X(IPCPacketResponseFreeBuffer, \
        int result;) \
    X(IPCPacketRequestInference, \
        int taskId; \
        dxrt_request_acc_t request;) \
    X(IPCPacketResponseInference, \
        int result; \
        dxrt_response_t response;) \
    X(IPCPacketThrottleNotification, \
        dx_pcie_dev_ntfy_throt_t throtInfo;) \
    X(IPCPacketErrorNotification, \
        int64_t serverErr; \
        dx_pcie_dev_err_t errorInfo;) \
    X(IPCPacketDMAReadRequest, \
        int64_t  blockId; \
        uint64_t blockOffset; \
        uint64_t size;) \
    X(IPCPacketDMAReadResponse, \
        int result;) \
    X(IPCPacketDMAWriteRequest, \
        int64_t  blockId; \
        uint64_t blockOffset; \
        uint64_t size;) \
    X(IPCPacketDMAWriteResponse, \
        int result;) \
    X(IPCPacketTaskInitRequest, \
        int taskId; \
        int bound; \
        uint64_t modelMemorySize; \
        int8_t  model_type; \
        int8_t  model_format; \
        uint32_t model_cmds; \
        uint32_t op_mode; \
        uint32_t cmd_offset; \
        uint32_t weight_offset; \
        uint32_t custom_offset; \
        uint32_t input_size; \
        uint32_t output_size; \
        uint32_t last_output_offset; \
        uint32_t output_all_offset; \
        uint32_t datas[MAX_CHECKPOINT_COUNT]; \
        uint64_t memory_base;) \
    X(IPCPacketTaskInitResponse, \
        int result;) \
    X(IPCPacketTaskDeInitRequest, \
        int taskId; \
        int bound;) \
    X(IPCPacketTaskDeInitResponse, \
        int result;) \
    X(IPCPacketViewFreeMemoryRequestLegacy, /* no payload */) \
    X(IPCPacketViewFreeMemoryResponseLegacy, \
        int result; \
        uint64_t bytes;) \
    X(IPCPacketViewUsedMemoryRequestLegacy, /* no payload */) \
    X(IPCPacketViewUsedMemoryResponseLegacy, \
        int result; \
        uint64_t bytes;) \
    X(IPCPacketGetUsageRequestLegacy, \
        int channel;) \
    X(IPCPacketGetUsageResponseLegacy, \
        int result; \
        double usage;) \
    X(IPCPacketGetDeviceTelemetryRequest, /* no payload */) \
    X(IPCPacketGetDeviceTelemetryResponse, \
        int result; \
        uint32_t coreCount; \
        uint64_t usedMemoryBytes; \
        uint64_t freeMemoryBytes;) \
    X(IPCPacketDMAReadRequestWithFaultInjection, \
        int64_t  blockId; \
        uint64_t blockOffset; \
        uint64_t size; )

#define X(name, fields) \
    struct name \
    { \
        IPCPacketHeader header; \
        fields \
    };
DXRT_IPC_PACKET_STRUCT_LIST(X)
#undef X

int32_t getNextIPCPacketSequenceId();
pid_t getCurrentIPCPacketPid();

bool isValidIPCMessageType(uint32_t typeValue);
bool tryGetIPCResponseMessageType(IPCMessageType requestType, IPCMessageType *responseType);
bool isValidIPCPacketHeader(const IPCPacketHeader &header);
bool isValidIPCPacketHeader(
    const IPCPacketHeader &header,
    IPCMessageType expectedType,
    size_t expectedPayloadSize);

size_t getSerializedIPCDeviceTelemetryResponseSize(const IPCDeviceTelemetryPayload &telemetry);
int serializeIPCDeviceTelemetryResponsePacket(
    const IPCPacketHeader &requestHeader,
    const IPCDeviceTelemetryPayload &telemetry,
    std::vector<uint8_t> *response);
int deserializeIPCDeviceTelemetryResponsePacket(
    const uint8_t *message,
    size_t messageSize,
    IPCPacketHeader *header,
    IPCDeviceTelemetryPayload *telemetry);

const char *toString(IPCMessageType type);

void initializeIPCPacketHeader(
    IPCPacketHeader *header,
    IPCMessageType type,
    uint32_t messageSize,
    int32_t seqId = 0,
    pid_t pid = 0,
    uint16_t version = kIPCPacketWireVersion);

template <typename Packet>
void initializeIPCPacket(
    Packet *packet,
    IPCMessageType type,
    int32_t seqId = 0,
    pid_t pid = 0,
    uint16_t version = kIPCPacketWireVersion)
{
    static_assert(std::is_standard_layout<Packet>::value, "Packet must use standard layout");
    static_assert(sizeof(Packet) >= sizeof(IPCPacketHeader), "Packet must include IPCPacketHeader");

    if (packet == nullptr)
    {
        return;
    }

    initializeIPCPacketHeader(
        &packet->header,
        type,
        static_cast<uint32_t>(sizeof(Packet) - sizeof(IPCPacketHeader)),
        seqId,
        pid,
        version);
}

template <typename Packet>
bool isValidIPCPacket(const Packet &packet, IPCMessageType expectedType)
{
    static_assert(std::is_standard_layout<Packet>::value, "Packet must use standard layout");
    static_assert(sizeof(Packet) >= sizeof(IPCPacketHeader), "Packet must include IPCPacketHeader");

    return isValidIPCPacketHeader(
        packet.header,
        expectedType,
        sizeof(Packet) - sizeof(IPCPacketHeader));
}

#pragma pack(pop)

#undef DXRT_IPC_PACKET_STRUCT_LIST

}  // namespace dxrt
