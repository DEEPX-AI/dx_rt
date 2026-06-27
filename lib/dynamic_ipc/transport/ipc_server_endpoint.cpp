/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_endpoint.hpp"

#include <cerrno>
#include <cstring>
#include <string>
#include <utility>

#include "dxrt/common.h"

namespace dxrt {

IPCServerEndpoint::IPCServerEndpoint() = default;

IPCServerEndpoint::~IPCServerEndpoint()
{
    stop();
}

int IPCServerEndpoint::start(const std::string &endpoint, int backlog, bool removeExisting)
{
    stop();

    if (preparePlatformStart() != 0)
    {
        return -1;
    }

    const int rc = _acceptor.bindAndListen(endpoint, backlog, removeExisting);
    if (rc != 0)
    {
        LOG_DXRT_ERR(
            "IPCServerEndpoint::start bindAndListen failed: endpoint=" + endpoint +
            ", errno=" + std::to_string(errno) +
            " (" + std::string(std::strerror(errno)) + ")");
        cleanupPlatformStop();
        return rc;
    }

    _running = true;
    _stopRequested = false;
    return 0;
}

void IPCServerEndpoint::stop() noexcept
{
    _stopRequested = true;

    std::vector<std::shared_ptr<IPCChannel>> channelsToClose;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);

        // 1. Collect all channels (including nullptr, filtered later).
        channelsToClose.reserve(_clients.size());
        for (auto &entry : _clients)
        {
            if (entry.second != nullptr)
            {
                channelsToClose.emplace_back(std::move(entry.second));
                // After move, entry.second becomes nullptr.
            }
        }

        // 2. Now safely clear (all values are nullptr or moved).
        _clients.clear();
        _pidByClientFd.clear();
        _clientFdByPid.clear();
        _nextClientId = 1;
    }
    // Lock is released. shared_ptr destructors in channelsToClose perform cleanup automatically.

    // 3. try-catch only for acceptor (the only expected failure point).
    try
    {
        _acceptor.close();
    }
    catch (const std::exception &e)
    {
        LOG_DXRT_WARN("IPCServerEndpoint::stop _acceptor.close() failed: " + std::string(e.what()));
    }

    cleanupPlatformStop();

    _running = false;
}

void IPCServerEndpoint::requestStop()
{
    _stopRequested = true;
}

bool IPCServerEndpoint::isRunning() const
{
    return _running;
}

size_t IPCServerEndpoint::getClientCount() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    return _clients.size();
}

void IPCServerEndpoint::setOnReceive(OnReceiveHandler handler)
{
    _onReceive = std::move(handler);
}

void IPCServerEndpoint::setOnClientConnected(OnClientConnectedHandler handler)
{
    _onClientConnected = std::move(handler);
}

void IPCServerEndpoint::setOnClientDisconnected(OnClientDisconnectedHandler handler)
{
    _onClientDisconnected = std::move(handler);
}

int IPCServerEndpoint::pollOnce(int timeoutMs, size_t maxMessageSize)
{
#if defined(__linux__) || defined(_WIN32)
    return pollPlatform(timeoutMs, maxMessageSize);
#else
    (void)timeoutMs;
    (void)maxMessageSize;
    errno = ENOSYS;
    return -1;
#endif
}

int IPCServerEndpoint::getListenFd() const
{
    return _acceptor.getListenFd();
}

IPCNativeHandle IPCServerEndpoint::getListenNativeHandle() const
{
    return _acceptor.getListenNativeHandle();
}

std::vector<int> IPCServerEndpoint::getClientFds() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    std::vector<int> fds;
    fds.reserve(_clients.size());
    for (const auto &entry : _clients)
    {
        fds.push_back(entry.first);
    }
    return fds;
}

std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> IPCServerEndpoint::getClientSnapshot() const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> snapshot;
    snapshot.reserve(_clients.size());
    for (const auto &entry : _clients)
    {
        snapshot.emplace_back(entry);
    }
    return snapshot;
}

bool IPCServerEndpoint::isStopRequested() const
{
    return _stopRequested;
}

int IPCServerEndpoint::run(int timeoutMs, size_t maxMessageSize)
{
    while (_running && !_stopRequested)
    {
        const int rc = pollOnce(timeoutMs, maxMessageSize);
        if (rc < 0)
        {
            return rc;
        }
    }

    return 0;
}

int IPCServerEndpoint::sendToClient(int clientFd, const uint8_t *data, size_t size, int fd)
{
    std::shared_ptr<IPCChannel> channel;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        const auto it = _clients.find(clientFd);
        if (it == _clients.end() || it->second == nullptr)
        {
            errno = ENOENT;
            return -1;
        }

        channel = it->second;
    }

    return channel->sendMessage(data, size, fd);
}

int IPCServerEndpoint::sendToPid(pid_t pid, const uint8_t *data, size_t size, int fd)
{
    std::shared_ptr<IPCChannel> channel;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        const auto it = _clientFdByPid.find(pid);
        if (it == _clientFdByPid.end())
        {
            errno = ENOENT;
            return -1;
        }

        const auto clientIt = _clients.find(it->second);
        if (clientIt == _clients.end() || clientIt->second == nullptr)
        {
            errno = ENOENT;
            return -1;
        }

        channel = clientIt->second;
    }

    return channel->sendMessage(data, size, fd);
}

void IPCServerEndpoint::bindPidToClient(pid_t pid, int clientFd)
{
    if (pid <= 0 || clientFd < 0)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(_clientsMutex);
    const auto oldClientIt = _clientFdByPid.find(pid);
    if (oldClientIt != _clientFdByPid.end() && oldClientIt->second != clientFd)
    {
        _pidByClientFd.erase(oldClientIt->second);
    }

    const auto oldPidIt = _pidByClientFd.find(clientFd);
    if (oldPidIt != _pidByClientFd.end() && oldPidIt->second != pid)
    {
        _clientFdByPid.erase(oldPidIt->second);
    }

    _pidByClientFd[clientFd] = pid;
    _clientFdByPid[pid] = clientFd;
}

pid_t IPCServerEndpoint::getPidForClient(int clientFd) const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    const auto pidIt = _pidByClientFd.find(clientFd);
    if (pidIt == _pidByClientFd.end())
    {
        return 0;
    }

    return pidIt->second;
}

void IPCServerEndpoint::unbindClient(int clientFd)
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    const auto pidIt = _pidByClientFd.find(clientFd);
    if (pidIt == _pidByClientFd.end())
    {
        return;
    }

    _clientFdByPid.erase(pidIt->second);
    _pidByClientFd.erase(pidIt);
}

int IPCServerEndpoint::acceptNewClient()
{
    auto newChannel = std::make_shared<IPCChannel>();
    if (_acceptor.accept(newChannel.get()) != 0)
    {
        return -1;
    }

    const int clientFd = allocateClientFd(*newChannel);
    if (clientFd < 0)
    {
        errno = EBADF;
        return -1;
    }

    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        _clients[clientFd] = newChannel;
    }
    LOG_DXRT_S_DBG << "IPCServerEndpoint::acceptNewClient clientFd=" << clientFd << std::endl;
    if (_onClientConnected)
    {
        _onClientConnected(clientFd);
    }
    return 0;
}

std::shared_ptr<IPCChannel> IPCServerEndpoint::getClientChannel(int clientFd) const
{
    std::lock_guard<std::mutex> lock(_clientsMutex);
    const auto it = _clients.find(clientFd);
    if (it == _clients.end())
    {
        return nullptr;
    }

    return it->second;
}

int IPCServerEndpoint::processClientMessage(int clientFd, size_t maxMessageSize)
{
    std::shared_ptr<IPCChannel> channel;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        const auto it = _clients.find(clientFd);
        if (it == _clients.end() || it->second == nullptr)
        {
            return -1;
        }
        channel = it->second;
    }

    if (channel == nullptr)
    {
        return -1;
    }

    std::vector<uint8_t> message(maxMessageSize);
    int receivedFd = -1;
    const int bytes = channel->receiveMessage(message.data(), message.size(), &receivedFd);
    if (bytes <= 0)
    {
        LOG_DXRT_S << "IPCServerEndpoint::processClientMessage receive failed clientFd=" << clientFd
                       << ", bytes=" << bytes
                       << ", errno=" << errno << std::endl;
        removeClient(clientFd);
        return -1;
    }

    return processReceivedClientMessage(clientFd, message.data(), static_cast<size_t>(bytes), receivedFd);
}

int IPCServerEndpoint::processReceivedClientMessage(
    int clientFd,
    const uint8_t *message,
    size_t messageSize,
    int receivedFd)
{
    if (getClientChannel(clientFd) == nullptr)
    {
        errno = ENOENT;
        return -1;
    }

    LOG_DXRT_S_DBG << "IPCServerEndpoint::processClientMessage clientFd=" << clientFd
                   << ", bytes=" << messageSize
                   << ", receivedFd=" << receivedFd << std::endl;

    int handlerRc = 0;

    if (_onReceive)
    {
        handlerRc = _onReceive(
            clientFd,
            message,
            messageSize,
            receivedFd);
    }

    // Negative handlerRc means a real failure — drop the client connection.
    if (handlerRc < 0)
    {
        LOG_DXRT_S_ERR(
            "IPCServerEndpoint::processClientMessage handler failed: clientFd=" + std::to_string(clientFd) +
            ", handlerRc=" + std::to_string(handlerRc) +
            ", errno=" + std::to_string(errno));
        removeClient(clientFd);
        return -1;
    }

    return 0;
}

void IPCServerEndpoint::removeClient(int clientFd)
{
    std::shared_ptr<IPCChannel> channel;
    pid_t pidToDisconnect = 0;
    {
        std::lock_guard<std::mutex> lock(_clientsMutex);
        const auto it = _clients.find(clientFd);
        if (it == _clients.end())
        {
            return;
        }

        channel = it->second;
        _clients.erase(it);

        const auto pidIt = _pidByClientFd.find(clientFd);
        if (pidIt != _pidByClientFd.end())
        {
            pidToDisconnect = pidIt->second;
            _clientFdByPid.erase(pidIt->second);
            _pidByClientFd.erase(pidIt);
        }
    }

    LOG_DXRT_S << "IPCServerEndpoint::removeClient clientFd=" << clientFd << std::endl;

    if (channel != nullptr)
    {
        channel->close();
    }
    else
    {
        LOG_DXRT_S << "IPCServerEndpoint::removeClient no channel to close for clientFd=" << clientFd << std::endl;
    }

    if (_onClientDisconnected)
    {
        _onClientDisconnected(clientFd, pidToDisconnect);
    }
    else
    {
        LOG_DXRT_S << "IPCServerEndpoint::removeClient no channel to close for clientFd=" << clientFd << std::endl;
    }
}

}  // namespace dxrt
