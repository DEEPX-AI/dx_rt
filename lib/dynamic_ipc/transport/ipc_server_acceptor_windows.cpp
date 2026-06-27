/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_acceptor.hpp"

#ifdef _WIN32

#include <windows.h>
#include <strsafe.h>
#include <tchar.h>
#include <sddl.h>

#include <cerrno>
#include <cstring>
#include <string>

#include "dxrt/common.h"

namespace dxrt {

namespace {

std::string formatErrnoMessage(const char *syscallName, int error)
{
    return std::string(syscallName) + " failed: errno=" + std::to_string(error) +
           " (" + std::string(std::strerror(error)) + ")";
}

int mapWindowsErrorToErrno(DWORD error)
{
    switch (error)
    {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
        case ERROR_BAD_PATHNAME:
            return ENOENT;
        case ERROR_ACCESS_DENIED:
            return EACCES;
        case ERROR_IO_PENDING:
        case ERROR_PIPE_BUSY:
        case ERROR_SEM_TIMEOUT:
        case ERROR_TIMEOUT:
        case ERROR_NO_DATA:
        case ERROR_PIPE_LISTENING:
            return EAGAIN;
        case ERROR_INVALID_HANDLE:
            return EBADF;
        case ERROR_INVALID_PARAMETER:
            return EINVAL;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
            return ENOMEM;
        default:
            return EIO;
    }
}

void setErrnoFromWindowsError(DWORD error)
{
    errno = mapWindowsErrorToErrno(error);
}

std::string normalizePipeName(const std::string &endpoint)
{
    static const std::string prefix = "\\\\.\\pipe\\";
    if (endpoint.compare(0, prefix.size(), prefix) == 0)
    {
        return endpoint;
    }
    return prefix + endpoint;
}

DWORD toPipeInstanceCount(int backlog)
{
    int instanceCount = backlog;
    if (instanceCount < 1)
    {
        instanceCount = 1;
    }
    if (instanceCount > 254)
    {
        instanceCount = 254;
    }
    return static_cast<DWORD>(instanceCount);
}

HANDLE createPipeInstance(const std::string &pipeName, int backlog, HANDLE iocp)
{
    //prepare secrity attribute

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = false;

    constexpr TCHAR* szSDDL = _T("D:(A;;GA;;;SY)(A;;GRGW;;;AU)");

    if (ConvertStringSecurityDescriptorToSecurityDescriptor(
        szSDDL, SDDL_REVISION_1, &(sa.lpSecurityDescriptor),
        NULL) == NULL)
    {
        LOG_DXRT_I_ERR("SDDL Conversion Failed. GLE=" + std::to_string(GetLastError()));
        return INVALID_HANDLE_VALUE;
    }



    HANDLE pipe = ::CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        toPipeInstanceCount(backlog),
        65536,
        65536,
        0,
        &sa);
    if (pipe == INVALID_HANDLE_VALUE)
    {
        return pipe;
    }

    if (associateHandleWithWindowsIocp(pipe, iocp) != 0)
    {
        ::CloseHandle(pipe);
        return INVALID_HANDLE_VALUE;
    }
    return pipe;
}

int setConnectedPipeMode(HANDLE pipe)
{
    DWORD mode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
    if (::SetNamedPipeHandleState(pipe, &mode, nullptr, nullptr) == FALSE)
    {
        setErrnoFromWindowsError(::GetLastError());
        LOG_DXRT_ERR(formatErrnoMessage("SetNamedPipeHandleState", errno));
        return -1;
    }

    return 0;
}

}  // namespace

int IPCServerListenSocket::bindAndListen(const std::string &socketPath, int backlog, bool removeExisting)
{
    // Preserve IOCP handle across rebinds: it is owned by the endpoint and
    // assigned via setWindowsIocpHandle() before bindAndListen().
    const IPCNativeHandle preservedIocp = _iocpHandle;
    close();
    _iocpHandle = preservedIocp;
    (void)removeExisting;

    if (socketPath.empty())
    {
        errno = EINVAL;
        return -1;
    }

    if (_iocpHandle == kInvalidIPCNativeHandle)
    {
        // The endpoint must create the IOCP and assign it before bindAndListen.
        errno = EBADF;
        LOG_DXRT_ERR("bindAndListen called without IOCP handle assigned");
        return -1;
    }

    _backlog = backlog;
    _socketPath = normalizePipeName(socketPath);

    HANDLE listenPipe = createPipeInstance(_socketPath, _backlog, reinterpret_cast<HANDLE>(_iocpHandle));
    if (listenPipe == INVALID_HANDLE_VALUE)
    {
        const DWORD winErr = ::GetLastError();
        _socketPath.clear();
        setErrnoFromWindowsError(winErr);
        LOG_DXRT_ERR(formatErrnoMessage("CreateNamedPipeA", errno));
        return -1;
    }

    _listenHandle = reinterpret_cast<IPCNativeHandle>(listenPipe);
    return 0;
}

void IPCServerListenSocket::setWindowsIocpHandle(IPCNativeHandle iocpHandle) const
{
    // The endpoint owns the single IOCP for the entire server lifetime.
    // The acceptor only borrows it; it never creates or closes one.
    // A pipe handle can only be associated with a single IOCP for its lifetime,
    // so changing IOCP after a listen pipe exists is invalid.
    if (_iocpHandle != kInvalidIPCNativeHandle && _iocpHandle != iocpHandle &&
        _listenHandle != kInvalidIPCNativeHandle)
    {
        LOG_DXRT_ERR("setWindowsIocpHandle called with different IOCP after listen pipe was created");
    }
    _iocpHandle = iocpHandle;
}

int IPCServerListenSocket::ensureAcceptPending(void *owner) const
{
    if (_listenHandle == kInvalidIPCNativeHandle)
    {
        if (_socketPath.empty())
        {
            errno = EBADF;
            return -1;
        }

        if (_iocpHandle == kInvalidIPCNativeHandle)
        {
            errno = EBADF;
            return -1;
        }

        HANDLE replacementPipe = createPipeInstance(
            _socketPath,
            _backlog,
            reinterpret_cast<HANDLE>(_iocpHandle));
        if (replacementPipe == INVALID_HANDLE_VALUE)
        {
            setErrnoFromWindowsError(::GetLastError());
            return -1;
        }
        _listenHandle = reinterpret_cast<IPCNativeHandle>(replacementPipe);
        _connectPending = false;
        resetWindowsIocpContext(
            &_connectContext,
            IPCWindowsIocpOperation::Accept,
            owner,
            -1,
            replacementPipe);
    }

    if (_connectPending)
    {
        return 0;
    }

    HANDLE listenPipe = reinterpret_cast<HANDLE>(_listenHandle);
    resetWindowsIocpContext(
        &_connectContext,
        IPCWindowsIocpOperation::Accept,
        owner,
        -1,
        listenPipe);

    const BOOL connected = ::ConnectNamedPipe(listenPipe, &_connectContext.overlapped);
    LOG_DXRT_S_DBG << "WIN ConnectNamedPipe rc=" << connected
                   << " err=" << (connected ? 0 : ::GetLastError())
                   << " pipe=" << listenPipe << std::endl;
    if (connected == FALSE)
    {
        const DWORD err = ::GetLastError();
        if (err == ERROR_IO_PENDING)
        {
            _connectPending = true;
            return 0;
        }

        if (err != ERROR_PIPE_CONNECTED)
        {
            setErrnoFromWindowsError(err);
            LOG_DXRT_ERR(formatErrnoMessage("ConnectNamedPipe", errno));
            return -1;
        }
    }

    if (::PostQueuedCompletionStatus(
            reinterpret_cast<HANDLE>(_iocpHandle),
            0,
            reinterpret_cast<ULONG_PTR>(listenPipe),
            &_connectContext.overlapped) == FALSE)
    {
        setErrnoFromWindowsError(::GetLastError());
        LOG_DXRT_ERR(formatErrnoMessage("PostQueuedCompletionStatus", errno));
        return -1;
    }

    _connectPending = true;
    return 0;
}

int IPCServerListenSocket::completeAccept(IPCChannel *channel) const
{
    if (channel == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (_listenHandle == kInvalidIPCNativeHandle)
    {
        errno = EBADF;
        return -1;
    }

    HANDLE listenPipe = reinterpret_cast<HANDLE>(_listenHandle);
    _connectPending = false;

    if (setConnectedPipeMode(listenPipe) != 0)
    {
        ::DisconnectNamedPipe(listenPipe);
        ::CloseHandle(listenPipe);
        _listenHandle = kInvalidIPCNativeHandle;
        return -1;
    }

    channel->reset(reinterpret_cast<IPCNativeHandle>(listenPipe));
    _listenHandle = kInvalidIPCNativeHandle;

    HANDLE nextPipe = createPipeInstance(_socketPath, _backlog, reinterpret_cast<HANDLE>(_iocpHandle));
    if (nextPipe == INVALID_HANDLE_VALUE)
    {
        setErrnoFromWindowsError(::GetLastError());
        return 0;
    }

    _listenHandle = reinterpret_cast<IPCNativeHandle>(nextPipe);
    return 0;
}

int IPCServerListenSocket::accept(IPCChannel *channel) const
{
    if (ensureAcceptPending(nullptr) != 0)
    {
        return -1;
    }

    DWORD transferredBytes = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED completedOverlapped = nullptr;
    const BOOL completed = ::GetQueuedCompletionStatus(
        reinterpret_cast<HANDLE>(_iocpHandle),
        &transferredBytes,
        &completionKey,
        &completedOverlapped,
        0);
    if (completed == FALSE && completedOverlapped == nullptr)
    {
        errno = EAGAIN;
        return -1;
    }

    if (completedOverlapped != &_connectContext.overlapped)
    {
        if (completedOverlapped != nullptr)
        {
            ::PostQueuedCompletionStatus(
                reinterpret_cast<HANDLE>(_iocpHandle),
                transferredBytes,
                completionKey,
                completedOverlapped);
        }
        errno = EAGAIN;
        return -1;
    }

    if (completed == FALSE)
    {
        _connectPending = false;
        setErrnoFromWindowsError(::GetLastError());
        LOG_DXRT_ERR(formatErrnoMessage("ConnectNamedPipe", errno));
        return -1;
    }

    return completeAccept(channel);
}

void IPCServerListenSocket::close()
{
    if (_listenHandle != kInvalidIPCNativeHandle)
    {
        HANDLE listenPipe = reinterpret_cast<HANDLE>(_listenHandle);
        if (_connectPending)
        {
            ::CancelIoEx(listenPipe, &_connectContext.overlapped);
        }
        if (::CloseHandle(listenPipe) == FALSE)
        {
            setErrnoFromWindowsError(::GetLastError());
            LOG_DXRT_ERR(formatErrnoMessage("CloseHandle", errno));
        }
        _listenHandle = kInvalidIPCNativeHandle;
    }
    _connectPending = false;
    // Do not close _iocpHandle here: it is owned by the endpoint.
    _iocpHandle = kInvalidIPCNativeHandle;
    resetWindowsIocpContext(
        &_connectContext,
        IPCWindowsIocpOperation::Accept,
        nullptr,
        -1,
        INVALID_HANDLE_VALUE);
    _socketPath.clear();
}

}  // namespace dxrt

#endif
