/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_channel.hpp"

#ifdef __linux__

#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>
#include <string>
#include <vector>

namespace dxrt {

namespace {

constexpr size_t kMinPacketSizeBytes = 4;

int returnNegErrno(int error)
{
    errno = error;
    return -error;
}

std::string formatErrnoMessage(const char *syscallName, int error)
{
    return std::string(syscallName) + " failed: errno=" + std::to_string(error) +
           " (" + std::string(std::strerror(error)) + ")";
}

}  // namespace

int IPCChannel::sendMessage(const uint8_t *data, size_t size, int fd) const
{
    const auto socketFd = static_cast<int>(_handle);
    if (socketFd < 0)
    {
        return returnNegErrno(EBADF);
    }

    if (size < kMinPacketSizeBytes)
    {
        LOG_DXRT_ERR("sendmsg blocked: packet size is smaller than 4 bytes, size=" + std::to_string(size));
        return returnNegErrno(EMSGSIZE);
    }

    int flags = 0;
#ifdef MSG_NOSIGNAL
    flags |= MSG_NOSIGNAL;
#endif

    ssize_t sent = -1;
    if (fd < 0)
    {
        do
        {
            sent = ::send(socketFd, data, size, flags);
        } while (sent < 0 && errno == EINTR);
    }
    else
    {
        std::vector<uint8_t> mutablePayload(data, data + size);
        struct iovec iov;
        iov.iov_base = static_cast<void *>(mutablePayload.data());
        iov.iov_len = size;

        struct msghdr msg;
        std::memset(&msg, 0, sizeof(msg));
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        std::array<char, CMSG_SPACE(sizeof(int))> controlBuffer{};
        msg.msg_control = controlBuffer.data();
        msg.msg_controllen = sizeof(controlBuffer);

        struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg == nullptr)
        {
            return returnNegErrno(EINVAL);
        }

        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        std::memcpy(CMSG_DATA(cmsg), &fd, sizeof(int));
        msg.msg_controllen = cmsg->cmsg_len;

        do
        {
            sent = ::sendmsg(socketFd, &msg, flags);
        } while (sent < 0 && errno == EINTR);
    }

    if (sent < 0)
    {
        LOG_DXRT_ERR(formatErrnoMessage((fd < 0) ? "send" : "sendmsg", errno));
        return -errno;
    }

    if (static_cast<size_t>(sent) != size)
    {
        LOG_DXRT_ERR("sendmsg failed to send a complete packet: expected=" + std::to_string(size) +
            ", actual=" + std::to_string(static_cast<size_t>(sent)));
        return returnNegErrno(EIO);
    }

    return static_cast<int>(sent);
}

int IPCChannel::receiveMessage(uint8_t *buffer, size_t bufferSize, int *fd) const
{
    if (fd != nullptr)
    {
        *fd = -1;
    }

    const auto socketFd = static_cast<int>(_handle);
    if (socketFd < 0)
    {
        return returnNegErrno(EBADF);
    }

    char dummy = 0;
    struct iovec iov;
    iov.iov_base = (bufferSize > 0) ? static_cast<void *>(buffer) : static_cast<void *>(&dummy);
    iov.iov_len = (bufferSize > 0) ? bufferSize : 1;

    struct msghdr msg;
    std::memset(&msg, 0, sizeof(msg));
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    std::array<char, CMSG_SPACE(sizeof(int))> controlBuffer{};
    msg.msg_control = controlBuffer.data();
    msg.msg_controllen = sizeof(controlBuffer);

    ssize_t received = -1;
    do
    {
        received = ::recvmsg(socketFd, &msg, 0);
    } while (received < 0 && errno == EINTR);

    if (received < 0)
    {
        LOG_DXRT_ERR(formatErrnoMessage("recvmsg", errno));
        return -errno;
    }

    if (received == 0)
    {
        return returnNegErrno(ECONNRESET);
    }

    if ((msg.msg_flags & MSG_TRUNC) != 0)
    {
        LOG_DXRT_ERR("recvmsg truncated packet: bufferSize=" + std::to_string(bufferSize));
        return returnNegErrno(EMSGSIZE);
    }

    if ((msg.msg_flags & MSG_CTRUNC) != 0)
    {
        LOG_DXRT_ERR("recvmsg truncated control data");
        return returnNegErrno(EMSGSIZE);
    }

    if (received < static_cast<ssize_t>(kMinPacketSizeBytes))
    {
        LOG_DXRT_ERR("recvmsg blocked: packet size is smaller than 4 bytes, size="
            + std::to_string(static_cast<size_t>(received)));
        return returnNegErrno(EMSGSIZE);
    }

    for (struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg); cmsg != nullptr; cmsg = CMSG_NXTHDR(&msg, cmsg))
    {
        if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS)
        {
            if (fd != nullptr)
            {
                std::memcpy(fd, CMSG_DATA(cmsg), sizeof(int));
            }
            break;
        }
    }

    return static_cast<int>(received);
}

void IPCChannel::close()
{
    const IPCNativeHandle handle = release();
    if (handle != kInvalidIPCNativeHandle)
    {
        const auto fd = static_cast<int>(handle);
        // Unblock any thread blocked in recvmsg() before closing the fd.
        // shutdown(SHUT_RDWR) causes recvmsg to return 0 immediately.
        ::shutdown(fd, SHUT_RDWR);
        if (::close(fd) != 0)
        {
            LOG_DXRT_ERR(formatErrnoMessage("close", errno));
        }
    }
}

}  // namespace dxrt

#endif
