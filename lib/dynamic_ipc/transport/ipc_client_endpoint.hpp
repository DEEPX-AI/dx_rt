/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "dxrt/common.h"
#include "../transport/ipc_channel.hpp"
#include "../protocol/ipc_packet.hpp"

namespace dxrt {

struct IPCClientReceivedMessage
{
    std::vector<uint8_t> data;
    int fd{-1};
};

class DXRT_INTERNAL_API IPCClientEndpoint
{
 public:
    using OnReceiveHandler = std::function<void(const IPCClientReceivedMessage &message)>;

    IPCClientEndpoint();
    ~IPCClientEndpoint();

    IPCClientEndpoint(const IPCClientEndpoint &) = delete;
    IPCClientEndpoint &operator=(const IPCClientEndpoint &) = delete;

    int connectToServer(const std::string &socketPath, int retryCount = 0, int retryDelayMs = 100);
    int sendMessageAsync(uint8_t *data, size_t size, int fd);
    int sendMessageSync(uint8_t *data, size_t size, int fd, IPCClientReceivedMessage *response, int timeoutMs = -1);

    int onReceiveMessage(uint8_t *buffer, size_t bufferSize, int *fd) const;

    bool isConnected() const;

    void setOnReceive(OnReceiveHandler handler);

    void close();

 private:
    struct PendingResponse;

    int startReceiveLoop();
    void stopReceiveLoop();
    void receiveLoop();
    int sendMessageTrackResponse(
        uint8_t *data,
        size_t size,
        int fd,
        std::future<IPCClientReceivedMessage> *responseFuture);
    int registerPendingResponse(int32_t sequenceId, std::future<IPCClientReceivedMessage> *responseFuture);
    bool tryMatchPendingResponse(const IPCPacketHeader &header, const IPCClientReceivedMessage &message);
    void completePendingResponse(int32_t sequenceId, const IPCClientReceivedMessage &message);
    void failPendingResponses();

    IPCChannel _channel;
    std::atomic<bool> _receiveLoopRunning{false};
    std::atomic<bool> _receiveLoopStopRequested{false};
    std::thread _receiveThread;
    mutable std::mutex _receiveThreadMutex;
    mutable std::mutex _pendingMutex;
    std::unordered_map<int32_t, std::shared_ptr<PendingResponse>> _pendingResponses;
    OnReceiveHandler _onReceive;
};

}  // namespace dxrt
