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

#include <array>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../transport/ipc_client_endpoint.hpp"
#include "../protocol/ipc_packet.hpp"
#include "../protocol/ipc_protocol_limits.hpp"
#include "dxrt/exception/server_err.h"
#include "../protocol/memory_type.hpp"
#include "../../device_pool/inference_context.h"

namespace dxrt {

class DXRT_INTERNAL_API IPCPacketClient
{
 public:
    static constexpr size_t kMaxMessageSize = kIPCMaxMessageSize;

    struct AllocateResult
    {
        int result{0};
        uint64_t bufferAddress{0};
        uint64_t bufferSize{0};
        int64_t blockId{0};
        int deviceId{0};
    };

    struct InferenceResult
    {
        int result{0};
        dxrt_response_t response{};
        int deviceId{0};
    };

    struct DeviceTelemetryResult
    {
        int result{0};
        uint32_t coreCount{0};
        uint64_t usedMemoryBytes{0};
        uint64_t freeMemoryBytes{0};
        std::array<uint32_t, 4> utilizationPermille{{0, 0, 0, 0}};
        std::array<int32_t, 4> temperature{{0, 0, 0, 0}};
        std::array<uint32_t, 4> clock{{0, 0, 0, 0}};
        std::array<uint32_t, 4> voltage{{0, 0, 0, 0}};
        std::vector<IPCDeviceTelemetryCoreData> cores;
    };

    using ThrottleNotificationHandler = std::function<int(int deviceId, const dx_pcie_dev_ntfy_throt_t &)>;
    using ErrorNotificationHandler = std::function<int(int deviceId, dxrt_server_err_t, const dx_pcie_dev_err_t &)>;
    using InferenceResponseHandler = std::function<int(const InferenceResult &)>;
    using RecoveryStartNotificationHandler = std::function<bool(int deviceId, uint32_t recoveryId)>;

    IPCPacketClient() = default;
    ~IPCPacketClient() = default;

    IPCPacketClient(const IPCPacketClient &) = delete;
    IPCPacketClient &operator=(const IPCPacketClient &) = delete;

    int connectToServer(const std::string &socketPath, int retryCount = 0, int retryDelayMs = 100);
    void close();

    void setThrottleNotificationHandler(ThrottleNotificationHandler handler);
    void setErrorNotificationHandler(ErrorNotificationHandler handler);
    void setInferenceResponseHandler(InferenceResponseHandler handler);
    void setRecoveryStartNotificationHandler(RecoveryStartNotificationHandler handler);

    int sendRecoveryStartAck(
        int deviceId,
        uint32_t recoveryId,
        int result);

    int receiveAndDispatch(int *receivedFd = nullptr) const;

    int Allocate(
        int deviceId,
        uint32_t bufferSize,
        int taskId,
        MemoryType memoryType,
        AllocateResult *result,
        intptr_t *responseFd = nullptr);

    int Deallocate(
        int deviceId,
        uint64_t bufferAddress);

    int InferenceRequest(
        int deviceId,
        int taskId,
        const dxrt_request_acc_t &request,
        InferenceResult *result);

    int InferenceRequestAsync(
        int deviceId,
        int taskId,
        const dxrt_request_acc_t &request);

    int DMARead(
        int deviceId,
        int64_t  blockId,
        uint64_t blockOffset,
        uint64_t size);

    int DMAWrite(
        int deviceId,
        int64_t  blockId,
        uint64_t blockOffset,
        uint64_t size);

    int TaskInit(
        int deviceId,
        int taskId,
        int bound,
        uint64_t modelMemorySize,
        const TaskStaticConfig &config);

    int TaskDeInit(
        int deviceId,
        int taskId,
        int bound);

    int ViewFreeMemoryLegacy(
        int deviceId,
        uint64_t *outBytes);

    int ViewUsedMemoryLegacy(
        int deviceId,
        uint64_t *outBytes);

    int GetUsageLegacy(
        int deviceId,
        int channel,
        double *outUsage);

    int GetDeviceTelemetry(
        int deviceId,
        DeviceTelemetryResult *result);
    int DMAReadWithFaultInjection(
        int deviceId,
        int64_t blockId,
        uint64_t blockOffset,
        uint64_t size);

 private:
    template <typename RequestPacket, typename ResponsePacket>
    int sendRequestAndWaitForResponse(
        RequestPacket *request,
        IPCMessageType requestType,
        ResponsePacket *response,
        int deviceId,
        int requestFd,
        intptr_t *responseFd);

    template <typename RequestPacket>
    int sendRequestAndWaitForResponseBytes(
        RequestPacket *request,
        IPCMessageType requestType,
        int deviceId,
        int requestFd,
        intptr_t *responseFd,
        std::vector<uint8_t> *responseBytes);

    int dispatchIncomingPacket(
        const uint8_t *message,
        size_t messageSize,
        int receivedFd,
        bool *dispatched) const;

    // Notification/Response handler methods
    int handleThrottleNotification(
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize,
        int fd) const;

    int handleErrorNotification(
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize,
        int fd) const;

    int handleInferenceResponse(
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize,
        int fd) const;

    int handleRecoveryStartNotification(
        const IPCPacketHeader &header,
        const uint8_t *payload,
        size_t payloadSize,
        int fd) const;

    void onClientReceived(const IPCClientReceivedMessage &message) const;

    IPCClientEndpoint _client;

    // Notification/Response handlers stored as members
    ThrottleNotificationHandler _throttleHandler;
    ErrorNotificationHandler _errorHandler;
    InferenceResponseHandler _inferenceHandler;
    RecoveryStartNotificationHandler _recoveryStartNotificationHandler;
};

}  // namespace dxrt
