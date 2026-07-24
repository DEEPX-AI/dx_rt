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

#include <cstddef>
#include <functional>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "dxrt/common.h"
#include "../transport/ipc_server_acceptor.hpp"
#include "../protocol/ipc_protocol_limits.hpp"

namespace dxrt {

class IPCPacketHandlerRegistry;

class DXRT_INTERNAL_API IPCServerEndpoint
{
 public:
    using OnClientConnectedHandler = std::function<void(int clientFd)>;
    using OnClientDisconnectedHandler = std::function<void(int clientFd, pid_t pid)>;

    using OnReceiveHandler = std::function<int(
        int clientFd,
        const uint8_t *message,
        size_t messageSize,
        int receivedFd)>;

    IPCServerEndpoint();
    ~IPCServerEndpoint();

    IPCServerEndpoint(const IPCServerEndpoint &) = delete;
    IPCServerEndpoint &operator=(const IPCServerEndpoint &) = delete;

    int start(const std::string &endpoint, int backlog = 16, bool removeExisting = true);
    void stop() noexcept;
    void requestStop();

    bool isRunning() const;
    size_t getClientCount() const;

    void setOnReceive(OnReceiveHandler handler);
    void setOnClientConnected(OnClientConnectedHandler handler);
    void setOnClientDisconnected(OnClientDisconnectedHandler handler);

    // Process one platform-specific poll iteration.
    // timeoutMs: -1 blocks indefinitely, 0 returns immediately.
    int pollOnce(int timeoutMs = 1000, size_t maxMessageSize = kIPCMaxMessageSize);

    // Keep polling until requestStop()/stop() is called or an unrecoverable error occurs.
    int run(int timeoutMs = 1000, size_t maxMessageSize = kIPCMaxMessageSize);

    int sendToClient(int clientFd, const uint8_t *data, size_t size, int fd = -1);
    int sendToPid(pid_t pid, const uint8_t *data, size_t size, int fd = -1);

    void bindPidToClient(pid_t pid, int clientFd);
    pid_t getPidForClient(int clientFd) const;
    void unbindClient(int clientFd);

    void removeClient(int clientFd);

#ifdef _WIN32
    IPCNativeHandle getWindowsIocpNativeHandle() const;
    int ensureAcceptPending();
    int completeAcceptedClient(int *clientFd);
#endif
    int processReceivedClientMessage(int clientFd, const uint8_t *message, size_t messageSize, int receivedFd);
    std::shared_ptr<IPCChannel> getClientChannel(int clientFd) const;
    std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> getClientSnapshot() const;

 private:
    int acceptNewClient();
    int processClientMessage(int clientFd, size_t maxMessageSize);
    int getListenFd() const;
    IPCNativeHandle getListenNativeHandle() const;
    std::vector<int> getClientFds() const;
    bool isStopRequested() const;

    int preparePlatformStart();
    void cleanupPlatformStop() noexcept;
    int allocateClientFd(const IPCChannel &channel);

#ifdef __linux__
    int pollPlatform(int timeoutMs, size_t maxMessageSize);
    int ensureLinuxEpollFd();
    int updateLinuxEpollRegistration(int fd, uint32_t events, bool add);
    int syncLinuxRegisteredFds();
    int findLinuxClientIdByNativeFd(int nativeFd) const;
#elif defined(_WIN32)
    int pollPlatform(int timeoutMs, size_t maxMessageSize);
#endif

    IPCServerListenSocket _acceptor;
    mutable std::mutex _clientsMutex;
    std::unordered_map<int, std::shared_ptr<IPCChannel>> _clients;
    std::unordered_map<int, pid_t> _pidByClientFd;
    std::unordered_map<pid_t, int> _clientFdByPid;
#ifdef _WIN32
    IPCNativeHandle _windowsIocpHandle{kInvalidIPCNativeHandle};
#endif
#ifdef __linux__
    int _epollFd{-1};
    int _registeredListenFd{-1};
    std::unordered_set<int> _registeredClientFds;
#endif
    OnClientConnectedHandler _onClientConnected;
    OnClientDisconnectedHandler _onClientDisconnected;
    OnReceiveHandler _onReceive;
    int _nextClientId{1};
    bool _running{false};
    bool _stopRequested{false};
};

}  // namespace dxrt
