/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_client_endpoint.hpp"

#include "../protocol/ipc_protocol_limits.hpp"

#include "dxrt/common.h"

#include <chrono>
#include <exception>
#include <future>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "ipc_connector.hpp"
#include "dxrt/safe_cast.h"
#include "dxrt/dynamic_ipc_endpoint.h"

namespace dxrt {

struct IPCClientEndpoint::PendingResponse
{
    std::promise<IPCClientReceivedMessage> promise;
    bool completed{false};
};

IPCClientEndpoint::IPCClientEndpoint() = default;

IPCClientEndpoint::~IPCClientEndpoint()
{
    close();
}

int IPCClientEndpoint::connectToServer(const std::string &socketPath, int retryCount, int retryDelayMs)
{
    close();

    LOG_DXRT_I_DBG << "connectToServer endpoint=" << GetDynamicIpcEndpointForLog(socketPath)
                   << ", retryCount=" << retryCount
                   << ", retryDelayMs=" << retryDelayMs << std::endl;

    const int rc = IPCClientConnector::connect(socketPath, &_channel, retryCount, retryDelayMs);
    if (rc != 0)
    {
        LOG_DXRT_I_DBG << "connectToServer failed errno=" << errno << std::endl;
        return rc;
    }

    LOG_DXRT_I_DBG << "connectToServer success, starting receive loop" << std::endl;
    return startReceiveLoop();
}

int IPCClientEndpoint::sendMessageAsync(uint8_t *data, size_t size, int fd)
{
    if (!_channel.isValid() || !_receiveLoopRunning.load())
    {
        errno = ENOTCONN;
        return -1;
    }

    if (data == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (size < sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        return -1;
    }

    IPCPacketHeader *header = dxrt::SafeCast::BytePtrToPtr<IPCPacketHeader*>(data);
    // Sequence ID is always assigned here - caller must not pre-set it.
    header->seqId = getNextIPCPacketSequenceId();

    LOG_DXRT_I_DBG << "sendMessageAsync seqId=" << header->seqId
                   << ", size=" << size
                   << ", fd=" << fd << std::endl;
    const int sent = _channel.sendMessage(data, size, fd);
    if (sent < 0)
    {
        LOG_DXRT_I_DBG << "sendMessageAsync failed seqId=" << header->seqId
                       << ", errno=" << errno << std::endl;
        return -1;
    }

    LOG_DXRT_I_DBG << "sendMessageAsync queued seqId=" << header->seqId << std::endl;
    return header->seqId;
}

int IPCClientEndpoint::sendMessageTrackResponse(
    uint8_t *data,
    size_t size,
    int fd,
    std::future<IPCClientReceivedMessage> *responseFuture)
{
    if (!_channel.isValid() || !_receiveLoopRunning.load())
    {
        errno = ENOTCONN;
        return -1;
    }

    if (data == nullptr || responseFuture == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (size < sizeof(IPCPacketHeader))
    {
        errno = EMSGSIZE;
        return -1;
    }

    IPCPacketHeader *header = SafeCast::BytePtrToPtr<IPCPacketHeader*>(data);
    // Sequence ID is always assigned here - caller must not pre-set it.
    header->seqId = getNextIPCPacketSequenceId();

    const int registerRc = registerPendingResponse(header->seqId, responseFuture);
    if (registerRc != 0)
    {
        LOG_DXRT_I_DBG << "registerPendingResponse failed seqId=" << header->seqId << std::endl;
        return -1;
    }

    LOG_DXRT_I_DBG << "sendMessageAsync seqId=" << header->seqId
                   << ", size=" << size
                   << ", fd=" << fd << std::endl;
    const int sent = _channel.sendMessage(data, size, fd);
    if (sent < 0)
    {
        std::lock_guard<std::mutex> lock(_pendingMutex);
        _pendingResponses.erase(header->seqId);
        LOG_DXRT_I_DBG << "sendMessageAsync failed seqId=" << header->seqId
                       << ", errno=" << errno << std::endl;
        return -1;
    }

    LOG_DXRT_I_DBG << "sendMessageAsync queued seqId=" << header->seqId << std::endl;
    return header->seqId;
}

int IPCClientEndpoint::sendMessageSync(
    uint8_t *data,
    size_t size,
    int fd,
    IPCClientReceivedMessage *response,
    int timeoutMs)
{
    if (!_channel.isValid() || !_receiveLoopRunning.load())
    {
        errno = ENOTCONN;
        return -1;
    }

    if (response == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    std::future<IPCClientReceivedMessage> responseFuture;
    const int sequenceId = sendMessageTrackResponse(data, size, fd, &responseFuture);
    if (sequenceId < 0)
    {
        LOG_DXRT_I_DBG << "sendMessageSync failed before wait" << std::endl;
        return -1;
    }

    try
    {
        if (timeoutMs < 0)
        {
            *response = responseFuture.get();
            return sequenceId;
        }

        const auto waitResult = responseFuture.wait_for(std::chrono::milliseconds(timeoutMs));
        if (waitResult != std::future_status::ready)
        {
            errno = ETIMEDOUT;
            LOG_DXRT_I_DBG << "sendMessageSync timed out seqId=" << sequenceId << std::endl;
            return -1;
        }

        *response = responseFuture.get();
    }
    catch (const std::exception &ex)
    {
        errno = ECONNRESET;
        LOG_DXRT_I_DBG << "sendMessageSync future failed seqId=" << sequenceId
                       << ": " << ex.what() << std::endl;
        return -1;
    }

    LOG_DXRT_I_DBG << "sendMessageSync received response seqId=" << sequenceId
                   << ", bytes=" << response->data.size()
                   << ", fd=" << response->fd << std::endl;
    return sequenceId;
}

int IPCClientEndpoint::onReceiveMessage(uint8_t *buffer, size_t bufferSize, int *fd) const
{
    return _channel.receiveMessage(buffer, bufferSize, fd);
}

bool IPCClientEndpoint::isConnected() const
{
    return _channel.isValid();
}

void IPCClientEndpoint::setOnReceive(OnReceiveHandler handler)
{
    std::lock_guard<std::mutex> lock(_pendingMutex);
    _onReceive = std::move(handler);
}

void IPCClientEndpoint::close()
{
    stopReceiveLoop();
    _channel.close();
    failPendingResponses();
}

int IPCClientEndpoint::startReceiveLoop()
{
    std::lock_guard<std::mutex> lock(_receiveThreadMutex);

    if (!_channel.isValid())
    {
        errno = ENOTCONN;
        return -1;
    }

    if (_receiveLoopRunning.load())
    {
        LOG_DXRT_I_DBG << "startReceiveLoop skipped: already running" << std::endl;
        return 0;
    }

    if (_receiveThread.joinable())
    {
        if (_receiveThread.get_id() == std::this_thread::get_id())
        {
            _receiveThread.detach();  // NOSONAR:S5962
        }
        else
        {
            _receiveThread.join();
        }
    }

    _receiveLoopStopRequested.store(false);
    _receiveLoopRunning.store(true);
    LOG_DXRT_I_DBG << "startReceiveLoop" << std::endl;
    _receiveThread = std::thread(&IPCClientEndpoint::receiveLoop, this);
    return 0;
}

void IPCClientEndpoint::stopReceiveLoop()
{
    std::unique_lock<std::mutex> lock(_receiveThreadMutex);

    const bool wasRunning = _receiveLoopRunning.exchange(false);
    _receiveLoopStopRequested.store(true);

    if (wasRunning)
    {
        LOG_DXRT_I_DBG << "stopReceiveLoop" << std::endl;
    }
    else
    {
        LOG_DXRT_I_DBG << "stopReceiveLoop skipped: already stopped" << std::endl;
    }

    _channel.close();

    if (_receiveThread.joinable())
    {
        if (_receiveThread.get_id() == std::this_thread::get_id())
        {
            // Self-stop path: avoid deadlock; thread exits naturally after return.
            _receiveThread.detach();  // NOSONAR:S5962
        }
        else
        {
            std::thread threadToJoin = std::move(_receiveThread);
            lock.unlock();
            threadToJoin.join();
            return;
        }
    }
}

void IPCClientEndpoint::receiveLoop()
{
    while (_receiveLoopRunning.load())
    {
        std::vector<uint8_t> buffer(kIPCMaxMessageSize);
        int receivedFd = -1;
        const int bytes = _channel.receiveMessage(buffer.data(), buffer.size(), &receivedFd);
        if (bytes <= 0)
        {
            LOG_DXRT_I_DBG << "receiveLoop terminating bytes=" << bytes
                           << ", errno=" << errno
                           << ", stopRequested=" << _receiveLoopStopRequested.load() << std::endl;
            break;
        }

        LOG_DXRT_I_DBG << "receiveLoop received bytes=" << bytes
                       << ", fd=" << receivedFd << std::endl;

        IPCClientReceivedMessage message;
        message.data.assign(buffer.begin(), buffer.begin() + bytes);
        message.fd = receivedFd;

        bool matchedPendingResponse = false;
        if (static_cast<size_t>(bytes) < sizeof(IPCPacketHeader))
        {
            LOG_DXRT_I_ERR("Received message too small for IPCPacketHeader, bytes=" << bytes);
            continue;
        }

        const IPCPacketHeader *header = SafeCast::BytePtrToPtr<const IPCPacketHeader*>(message.data.data());
        if (isValidIPCPacketHeader(*header) == false)
        {
            LOG_DXRT_I_ERR("Received message with invalid IPCPacketHeader");
            continue;
        }

        if (header->seqId > 0)
        {
            matchedPendingResponse = tryMatchPendingResponse(*header, message);
        }

        if (matchedPendingResponse)
        {
            LOG_DXRT_I_DBG << "receiveLoop matched pending response" << std::endl;
            continue;
        }

        OnReceiveHandler handler;
        {
            std::lock_guard<std::mutex> lock(_pendingMutex);
            handler = _onReceive;
        }
        if (handler)
        {
            handler(message);
        }
    }

    _receiveLoopRunning.store(false);
    _receiveLoopStopRequested.store(true);
    _channel.close();
    LOG_DXRT_I_DBG << "receiveLoop stopped" << std::endl;
    // Unblock any threads blocked in sendMessageSync that are still waiting.
    failPendingResponses();
}

bool IPCClientEndpoint::tryMatchPendingResponse(
    const IPCPacketHeader &header,
    const IPCClientReceivedMessage &message)
{
    // handle response to sync request
    LOG_DXRT_I_DBG << "receiveLoop seqId=" << header.seqId
                   << ", type=" << header.type
                   << ", msgSize=" << header.messageSize << std::endl;

    std::lock_guard<std::mutex> lock(_pendingMutex);
    const auto it = _pendingResponses.find(header.seqId);
    if (it == _pendingResponses.end())
    {
        return false;
    }

    std::shared_ptr<PendingResponse> pending = it->second;
    _pendingResponses.erase(it);
    if (pending != nullptr && !pending->completed)
    {
        pending->completed = true;
        pending->promise.set_value(message);
        LOG_DXRT_I_DBG << "receiveLoop matched seqId=" << header.seqId << std::endl;
        return true;
    }

    return false;
}

int IPCClientEndpoint::registerPendingResponse(
    int32_t sequenceId,
    std::future<IPCClientReceivedMessage> *responseFuture)
{
    if (sequenceId <= 0 || responseFuture == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    auto pending = std::make_shared<PendingResponse>();
    *responseFuture = pending->promise.get_future();

    std::lock_guard<std::mutex> lock(_pendingMutex);
    _pendingResponses[sequenceId] = pending;
    return 0;
}

void IPCClientEndpoint::completePendingResponse(
    int32_t sequenceId,
    const IPCClientReceivedMessage &message)
{
    std::lock_guard<std::mutex> lock(_pendingMutex);
    const auto it = _pendingResponses.find(sequenceId);
    if (it == _pendingResponses.end() || it->second == nullptr || it->second->completed)
    {
        return;
    }

    it->second->completed = true;
    it->second->promise.set_value(message);
    _pendingResponses.erase(it);
}

void IPCClientEndpoint::failPendingResponses()
{
    std::lock_guard<std::mutex> lock(_pendingMutex);
    for (const auto &entry : _pendingResponses)
    {
        if (entry.second != nullptr && !entry.second->completed)
        {
            entry.second->completed = true;
            entry.second->promise.set_exception(
                std::make_exception_ptr(std::runtime_error("IPC client closed")));
        }
    }
    _pendingResponses.clear();
}

}  // namespace dxrt
