/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <vector>
#endif

#include "dxrt/common.h"
#include "../transport/ipc_channel.hpp"

namespace dxrt {

#ifdef _WIN32

enum class IPCWindowsIocpOperation
{
    Accept,
    Read
};

struct IPCWindowsIocpContext
{
    OVERLAPPED overlapped{};
    IPCWindowsIocpOperation operation{IPCWindowsIocpOperation::Accept};
    void *owner{nullptr};
    int clientFd{-1};
    HANDLE handle{INVALID_HANDLE_VALUE};
    std::vector<uint8_t> buffer;
};

inline int associateHandleWithWindowsIocp(HANDLE handle, HANDLE iocp)
{
    if (iocp == nullptr)
    {
        errno = EIO;
        return -1;
    }

    if (::CreateIoCompletionPort(handle, iocp, reinterpret_cast<ULONG_PTR>(handle), 0) == nullptr)
    {
        errno = EIO;
        return -1;
    }

    (void)::SetFileCompletionNotificationModes(handle, FILE_SKIP_COMPLETION_PORT_ON_SUCCESS);
    return 0;
}

inline void resetWindowsIocpContext(
    IPCWindowsIocpContext *context,
    IPCWindowsIocpOperation operation,
    void *owner,
    int clientFd,
    HANDLE handle)
{
    if (context == nullptr)
    {
        return;
    }

    std::memset(&context->overlapped, 0, sizeof(context->overlapped));
    context->operation = operation;
    context->owner = owner;
    context->clientFd = clientFd;
    context->handle = handle;
}

#endif

class IPCServerListenSocket
{
 public:
    IPCServerListenSocket();
    ~IPCServerListenSocket();

    IPCServerListenSocket(const IPCServerListenSocket &) = delete;
    IPCServerListenSocket &operator=(const IPCServerListenSocket &) = delete;

    int bindAndListen(const std::string &socketPath, int backlog = 16, bool removeExisting = true);
    int accept(IPCChannel *channel) const;
#ifdef _WIN32
    void setWindowsIocpHandle(IPCNativeHandle iocpHandle) const;
    int ensureAcceptPending(void *owner) const;
    int completeAccept(IPCChannel *channel) const;
#endif
    int getListenFd() const;
    IPCNativeHandle getListenNativeHandle() const;

    void close();

 private:
    mutable IPCNativeHandle _listenHandle{kInvalidIPCNativeHandle};
#ifdef _WIN32
    mutable IPCNativeHandle _iocpHandle{kInvalidIPCNativeHandle};
    mutable IPCWindowsIocpContext _connectContext{};
    mutable bool _connectPending{false};
#endif
    std::string _socketPath;
    int _backlog{16};
};

}  // namespace dxrt
