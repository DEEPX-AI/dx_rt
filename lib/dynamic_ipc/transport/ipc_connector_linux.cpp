/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_connector.hpp"

#ifdef __linux__

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <iostream>

#include "dxrt/common.h"
#include "dxrt/dynamic_ipc_endpoint.h"

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

}  // namespace



int IPCClientConnector::connect(const std::string &endpoint, IPCChannel *channel, int retryCount, int retryDelayMs)
{
    if (channel == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (endpoint.empty())
    {
        errno = EINVAL;
        return -1;
    }

    if (retryCount < 0)
    {
        retryCount = 0;
    }

    if (retryDelayMs < 0)
    {
        retryDelayMs = 0;
    }

    LOG_DXRT_DBG << "IPC Client connect START: endpoint='" << GetDynamicIpcEndpointForLog(endpoint)
             << "' retryCount=" << retryCount
             << " retryDelayMs=" << retryDelayMs << std::endl;
    LOG_DXRT_DBG << "  process_ns: " << getProcessNamespaceDebugInfo() << std::endl;

    const bool isAbstractEndpoint = (endpoint[0] == '@' || endpoint[0] == '\0');
    const size_t endpointNameLen = isAbstractEndpoint ? (endpoint.size() - 1) : endpoint.size();
    const size_t maxPathLen = sizeof(sockaddr_un::sun_path) - 1;

    LOG_DXRT_DBG << "  endpointType=" << (isAbstractEndpoint ? "ABSTRACT" : "FILESYSTEM")
             << " endpointNameLen=" << endpointNameLen << " maxPathLen=" << maxPathLen << std::endl;

    if (endpointNameLen > maxPathLen)
    {
        LOG_DXRT_ERR("Endpoint name too long");
        errno = ENAMETOOLONG;
        return -1;
    }

    for (int attempt = 0; attempt <= retryCount; ++attempt)
    {
        LOG_DXRT_DBG << "  connect attempt " << (attempt + 1) << " of " << (retryCount + 1) << std::endl;
        const int fd = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
        if (fd < 0)
        {
            LOG_DXRT_ERR(formatErrnoMessage("socket", errno));
            return -1;
        }

        // Set socket buffer sizes to improve performance consistency between sudo and regular user
        const int rcvbuf_size = 256 * 1024;  // 256 KB receive buffer
        const int sndbuf_size = 256 * 1024;  // 256 KB send buffer
        if (::setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, sizeof(rcvbuf_size)) != 0)
        {
            LOG_DXRT << "  warning: setsockopt SO_RCVBUF failed: " << formatErrnoMessage("setsockopt", errno) << std::endl;
        }
        if (::setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, sizeof(sndbuf_size)) != 0)
        {
            LOG_DXRT << "  warning: setsockopt SO_SNDBUF failed: " << formatErrnoMessage("setsockopt", errno) << std::endl;
        }

        struct sockaddr_un addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        socklen_t len = 0;
        if (isAbstractEndpoint)
        {
            // Abstract namespace socket: sun_path[0] must remain NUL.
            if (endpointNameLen > 0)
            {
                std::memcpy(addr.sun_path + 1, endpoint.data() + 1, endpointNameLen);
            }
            len = static_cast<socklen_t>(
                offsetof(struct sockaddr_un, sun_path) + 1 + endpointNameLen);
            LOG_DXRT_DBG << "    socket created: fd=" << fd
                     << " (abstract, len=" << len << ")" << std::endl;
        }
        else
        {
            if (endpointNameLen > 0)
            {
                std::memcpy(addr.sun_path, endpoint.data(), endpointNameLen);
            }
            len = static_cast<socklen_t>(
                offsetof(struct sockaddr_un, sun_path) + endpointNameLen + 1);
            LOG_DXRT_DBG << "    socket created: fd=" << fd << " (filesystem, len=" << len << ")" << std::endl;
        }
        const auto *rawAddr = static_cast<const void *>(&addr);
        const auto *sockAddr = static_cast<const struct sockaddr *>(rawAddr);

        LOG_DXRT_DBG << "    attempting connect..." << std::endl;
        if (::connect(fd, sockAddr, len) == 0)
        {
            LOG_DXRT_DBG << "  IPC Client connect SUCCESS on attempt " << (attempt + 1) << std::endl;
            channel->reset(fd);
            return 0;
        }

        const int savedErrno = errno;
        LOG_DXRT_DBG << "    connect failed: errno=" << savedErrno << " (" << std::strerror(savedErrno) << ")" << std::endl;
        if (::close(fd) != 0)
        {
            LOG_DXRT_ERR(formatErrnoMessage("close", errno));
        }

        if (attempt == retryCount)
        {
            LOG_DXRT_ERR("IPC Client connect FAILED after " << (retryCount + 1) << " attempts, last errno=" << savedErrno);
            errno = savedErrno;
            return -1;
        }

        if (retryDelayMs > 0)
        {
            LOG_DXRT_DBG << "    waiting " << retryDelayMs << "ms before retry..." << std::endl;
            struct timespec delay;
            delay.tv_sec = retryDelayMs / 1000;
            delay.tv_nsec = (retryDelayMs % 1000) * 1000000L;
            (void)::nanosleep(&delay, nullptr);
        }
    }
    LOG_DXRT_ERR("IPC Client connect FAILED: unexpected loop end");
    errno = ECONNREFUSED;
    return -1;
}

}  // namespace dxrt

#endif
