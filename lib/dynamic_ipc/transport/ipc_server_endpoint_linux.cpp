/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_endpoint.hpp"

#ifdef __linux__

#include <sys/epoll.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <unordered_set>
#include <vector>

#include "dxrt/common.h"

namespace dxrt {

namespace {

bool isTransientAcceptError(int err)
{
    return err == EAGAIN || err == EWOULDBLOCK || err == EINTR || err == ECONNABORTED;
}

}  // namespace

int IPCServerEndpoint::preparePlatformStart()
{
    return 0;
}

void IPCServerEndpoint::cleanupPlatformStop() noexcept
{
    if (_epollFd >= 0)
    {
        ::close(_epollFd);
        _epollFd = -1;
    }
    _registeredListenFd = -1;
    _registeredClientFds.clear();
}

int IPCServerEndpoint::allocateClientFd(const IPCChannel &channel)
{
    (void)channel;
    return _nextClientId++;
}

int IPCServerEndpoint::ensureLinuxEpollFd()
{
    if (_epollFd >= 0)
    {
        return 0;
    }

    _epollFd = ::epoll_create1(EPOLL_CLOEXEC);
    if (_epollFd < 0)
    {
        LOG_DXRT_ERR(
            "epoll_create1 failed: errno=" + std::to_string(errno) +
            " (" + std::string(std::strerror(errno)) + ")");
        return -1;
    }

    return 0;
}

int IPCServerEndpoint::updateLinuxEpollRegistration(int fd, uint32_t events, bool add)
{
    struct epoll_event event;
    std::memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;

    const int operation = add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL;
    if (::epoll_ctl(_epollFd, operation, fd, add ? &event : nullptr) != 0)
    {
        if (!add && (errno == EBADF || errno == ENOENT))
        {
            return 0;
        }

        LOG_DXRT_ERR(
            "epoll_ctl failed: errno=" + std::to_string(errno) +
            " (" + std::string(std::strerror(errno)) + ")");
        return -1;
    }

    return 0;
}

int IPCServerEndpoint::syncLinuxRegisteredFds()
{
    const int listenFd = getListenFd();
    if (listenFd < 0)
    {
        errno = EBADF;
        return -1;
    }

    if (_registeredListenFd != listenFd)
    {
        if (_registeredListenFd >= 0 && updateLinuxEpollRegistration(_registeredListenFd, 0, false) != 0)
        {
            return -1;
        }
        if (updateLinuxEpollRegistration(listenFd, EPOLLIN, true) != 0)
        {
            return -1;
        }
        _registeredListenFd = listenFd;
    }

    const std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> snapshot = getClientSnapshot();
    std::unordered_set<int> currentNativeFds;
    currentNativeFds.reserve(snapshot.size());
    for (const auto &entry : snapshot)
    {
        if (entry.second == nullptr)
        {
            continue;
        }

        const int nativeFd = entry.second->getFd();
        if (nativeFd < 0)
        {
            continue;
        }

        currentNativeFds.insert(nativeFd);
        if (_registeredClientFds.find(nativeFd) == _registeredClientFds.end())
        {
            if (updateLinuxEpollRegistration(nativeFd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR, true) != 0)
            {
                return -1;
            }
            _registeredClientFds.insert(nativeFd);
        }
    }

    for (auto it = _registeredClientFds.begin(); it != _registeredClientFds.end();)
    {
        if (currentNativeFds.find(*it) != currentNativeFds.end())
        {
            ++it;
            continue;
        }

        const int staleFd = *it;
        it = _registeredClientFds.erase(it);
        if (updateLinuxEpollRegistration(staleFd, 0, false) != 0)
        {
            return -1;
        }
    }

    return 0;
}

int IPCServerEndpoint::findLinuxClientIdByNativeFd(int nativeFd) const
{
    const std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> snapshot = getClientSnapshot();
    for (const auto &entry : snapshot)
    {
        if (entry.second != nullptr && entry.second->getFd() == nativeFd)
        {
            return entry.first;
        }
    }

    return -1;
}

int IPCServerEndpoint::pollPlatform(int timeoutMs, size_t maxMessageSize)
{
    if (!isRunning())
    {
        errno = EBADF;
        return -1;
    }

    if (maxMessageSize == 0)
    {
        errno = EINVAL;
        return -1;
    }

    if (ensureLinuxEpollFd() != 0 || syncLinuxRegisteredFds() != 0)
    {
        return -1;
    }

    constexpr int kMaxEvents = 64;
    std::array<struct epoll_event, kMaxEvents> events;
    int ready = -1;
    do
    {
        ready = ::epoll_wait(_epollFd, events.data(), static_cast<int>(events.size()), timeoutMs);
    } while (ready < 0 && errno == EINTR);

    if (ready < 0)
    {
        LOG_DXRT_ERR(
            "epoll_wait failed: errno=" + std::to_string(errno) +
            " (" + std::string(std::strerror(errno)) + ")");
        return ready;
    }

    if (ready == 0)
    {
        return ready;
    }

    int processed = 0;
    for (int index = 0; index < ready; ++index)
    {
        const int fd = events[index].data.fd;
        if (fd == _registeredListenFd)
        {
            if (acceptNewClient() != 0 && !isTransientAcceptError(errno))
            {
                return -1;
            }
            ++processed;
            continue;
        }

        if ((events[index].events & (EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0)
        {
            const int clientFd = findLinuxClientIdByNativeFd(fd);
            if (clientFd >= 0)
            {
                processClientMessage(clientFd, maxMessageSize);
                ++processed;
            }
        }
    }

    (void)syncLinuxRegisteredFds();
    return processed;
}

}  // namespace dxrt

#endif
