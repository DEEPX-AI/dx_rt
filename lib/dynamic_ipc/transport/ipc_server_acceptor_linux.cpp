/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_acceptor.hpp"

#ifdef __linux__

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

#include "dxrt/common.h"
#include "dxrt/dynamic_ipc_endpoint.h"
#include "dxrt/safe_cast.h"

namespace dxrt {

namespace {

std::string formatErrnoMessage(const char *syscallName, int error)
{
    return std::string(syscallName) + " failed: errno=" + std::to_string(error) +
           " (" + std::string(std::strerror(error)) + ")";
}

std::string readNamespaceLink(const char *nsName)
{
    const std::string linkPath = std::string("/proc/self/ns/") + nsName;
    std::string linkValue(127, '\0');

    const ssize_t bytes = ::readlink(linkPath.c_str(), &linkValue[0], linkValue.size());
    if (bytes < 0)
    {
        const int savedErrno = errno;
        return std::string(nsName) + "=<readlink-error:" + std::strerror(savedErrno) + ">";
    }

    linkValue.resize(static_cast<size_t>(bytes));
    return std::string(nsName) + "=" + linkValue;
}

std::string getProcessNamespaceDebugInfo()
{
    return "pid=" + std::to_string(::getpid()) +
           " ppid=" + std::to_string(::getppid()) +
           " " + readNamespaceLink("ipc") +
           " " + readNamespaceLink("net") +
           " " + readNamespaceLink("mnt") +
           " " + readNamespaceLink("pid");
}

int cleanupExistingSocketPathIfNeeded(const std::string &socketPath)
{
    struct stat st;
    if (::lstat(socketPath.c_str(), &st) != 0)
    {
        if (errno == ENOENT)
        {
            return 0;
        }
        return -1;
    }

    if (!S_ISSOCK(st.st_mode))
    {
        errno = EEXIST;
        return -1;
    }

    if (::unlink(socketPath.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}

}  // namespace

int IPCServerListenSocket::bindAndListen(const std::string &socketPath, int backlog, bool removeExisting)
{
    close();

    if (socketPath.empty())
    {
        errno = EINVAL;
        return -1;
    }

    LOG_DXRT_S << "IPC Server bindAndListen START: socketPath='"
               << GetDynamicIpcEndpointForLog(socketPath) << "'" << std::endl;
    LOG_DXRT_S << "  process_ns: " << getProcessNamespaceDebugInfo() << std::endl;

    const bool isAbstractEndpoint = (socketPath[0] == '@' || socketPath[0] == '\0');
    const size_t endpointNameLen = isAbstractEndpoint ? (socketPath.size() - 1) : socketPath.size();
    const size_t maxPathLen = sizeof(sockaddr_un::sun_path) - 1;

    LOG_DXRT_S << "  endpointType=" << (isAbstractEndpoint ? "ABSTRACT" : "FILESYSTEM")
               << " endpointNameLen=" << endpointNameLen
               << " maxPathLen=" << maxPathLen << std::endl;

    if (endpointNameLen > maxPathLen)
    {
        LOG_DXRT_S_ERR("Endpoint name too long");
        errno = ENAMETOOLONG;
        return -1;
    }

    int fd = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0)
    {
        LOG_DXRT_S_ERR("socket creation failed: " + formatErrnoMessage("socket", errno));
        return -1;
    }

    LOG_DXRT_S << "  socket fd created: " << fd << std::endl;

    // Set socket buffer sizes to improve performance consistency between sudo and regular user
    // SOCK_SEQPACKET uses these as maximum datagram sizes for buffering
    const int rcvbuf_size = 256 * 1024;  // 256 KB receive buffer
    const int sndbuf_size = 256 * 1024;  // 256 KB send buffer
    if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) != 0)
    {
        LOG_DXRT_S << "  warning: setsockopt SO_RCVBUF failed: " << formatErrnoMessage("setsockopt", errno) << std::endl;
    }
    if (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, sizeof(sndbuf_size)) != 0)
    {
        LOG_DXRT_S << "  warning: setsockopt SO_SNDBUF failed: " << formatErrnoMessage("setsockopt", errno) << std::endl;
    }
    LOG_DXRT_S << "  socket buffer sizes set: rcvbuf=" << rcvbuf_size
               << " sndbuf=" << sndbuf_size << std::endl;

    struct sockaddr_un addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    socklen_t addrLen = 0;
    if (isAbstractEndpoint)
    {
        if (endpointNameLen > 0)
        {
            std::memcpy(addr.sun_path + 1, socketPath.data() + 1, endpointNameLen);
        }
        addrLen = static_cast<socklen_t>(offsetof(struct sockaddr_un, sun_path) + 1 + endpointNameLen);
        LOG_DXRT_S << "  abstract socket: addrLen=" << addrLen << std::endl;
    }
    else
    {
        if (endpointNameLen > 0)
        {
            std::memcpy(addr.sun_path, socketPath.data(), endpointNameLen);
        }
        addrLen = static_cast<socklen_t>(offsetof(struct sockaddr_un, sun_path) + endpointNameLen + 1);
        LOG_DXRT_S << "  filesystem socket: addrLen=" << addrLen << std::endl;

        if (removeExisting)
        {
            LOG_DXRT_S << "  removing existing socket file if present..." << std::endl;
            if (cleanupExistingSocketPathIfNeeded(socketPath) != 0)
            {
                const int savedErrno = errno;
                LOG_DXRT_S_ERR("existing path check/cleanup failed: " +
                    formatErrnoMessage("cleanupExistingSocketPathIfNeeded", savedErrno));
                (void)::close(fd);
                errno = savedErrno;
                return -1;
            }
            LOG_DXRT_S << "  existing socket cleanup complete" << std::endl;
        }
    }

    LOG_DXRT_S << "  attempting bind with addrLen=" << addrLen << std::endl;
    if (::bind(fd, SafeCast::BytePtrToPtr<const struct sockaddr*>(SafeCast::PtrToBytePtr(&addr)), addrLen) != 0)
    {
        const int savedErrno = errno;
        LOG_DXRT_S_ERR("bind failed: " + formatErrnoMessage("bind", savedErrno));
        (void)::close(fd);
        errno = savedErrno;
        return -1;
    }

    LOG_DXRT_S << "  bind succeeded, attempting listen with backlog=" << backlog << std::endl;
    if (::listen(fd, (backlog > 0) ? backlog : 16) != 0)
    {
        const int savedErrno = errno;
        LOG_DXRT_S_ERR("listen failed: " + formatErrnoMessage("listen", savedErrno));
        (void)::close(fd);
        errno = savedErrno;
        return -1;
    }

    LOG_DXRT_S << "  listen succeeded" << std::endl;

    if (!isAbstractEndpoint)
    {
        // Allow clients from different users/containers to connect to the filesystem socket.
        LOG_DXRT_S << "  setting socket file permissions (0666)" << std::endl;
        if (::chmod(socketPath.c_str(), static_cast<mode_t>(0666)) != 0)
        {
            const int savedErrno = errno;
            LOG_DXRT_S_ERR("chmod failed: " + formatErrnoMessage("chmod", savedErrno));
            (void)::close(fd);
            (void)::unlink(socketPath.c_str());
            errno = savedErrno;
            return -1;
        }
        LOG_DXRT_S << "  chmod succeeded" << std::endl;
    }

    LOG_DXRT_S << "IPC Server bindAndListen SUCCESS: fd=" << fd << std::endl;

    _listenHandle = static_cast<IPCNativeHandle>(fd);
    _socketPath = socketPath;
    _backlog = backlog;
    return 0;
}

int IPCServerListenSocket::accept(IPCChannel *channel) const
{
    if (channel == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    const auto listenFd = static_cast<int>(_listenHandle);
    if (listenFd < 0)
    {
        errno = EBADF;
        return -1;
    }

    int clientFd = -1;
    do
    {
        clientFd = ::accept(listenFd, nullptr, nullptr);
    } while (clientFd < 0 && errno == EINTR);

    if (clientFd < 0)
    {
        return -1;
    }

    channel->reset(static_cast<IPCNativeHandle>(clientFd));
    return 0;
}

void IPCServerListenSocket::close()
{
    if (_listenHandle != kInvalidIPCNativeHandle)
    {
        const auto fd = static_cast<int>(_listenHandle);
        if (::close(fd) != 0)
        {
            LOG_DXRT_ERR(formatErrnoMessage("close", errno));
        }
        _listenHandle = kInvalidIPCNativeHandle;
    }

    if (!_socketPath.empty() && _socketPath[0] != '@' && _socketPath[0] != '\0')
    {
        (void)::unlink(_socketPath.c_str());
    }
    _socketPath.clear();
}

}  // namespace dxrt

#endif
