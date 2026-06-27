/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_endpoint.hpp"

#ifdef _WIN32

#include <windows.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "dxrt/common.h"

namespace dxrt {

namespace {

struct WindowsReadContext
{
    IPCWindowsIocpContext iocp;
    IPCServerEndpoint *endpoint{nullptr};
    IPCNativeHandle nativeHandle{kInvalidIPCNativeHandle};
    bool pending{false};
};

using ReadContextList = std::vector<std::unique_ptr<WindowsReadContext>>;

static_assert(offsetof(WindowsReadContext, iocp) == 0, "iocp must be the first read context member");

std::unordered_map<IPCServerEndpoint *, ReadContextList> &endpointReadContexts()
{
    static std::unordered_map<IPCServerEndpoint *, ReadContextList> contexts;
    return contexts;
}

ReadContextList &getReadContexts(IPCServerEndpoint *endpoint)
{
    return endpointReadContexts()[endpoint];
}

void eraseEndpointReadContexts(IPCServerEndpoint *endpoint)
{
    endpointReadContexts().erase(endpoint);
}

WindowsReadContext *findReadContext(
    const ReadContextList &readContexts,
    IPCServerEndpoint *endpoint,
    int clientFd)
{
    for (const auto &context : readContexts)
    {
        if (context->endpoint == endpoint && context->iocp.clientFd == clientFd)
        {
            return context.get();
        }
    }

    return nullptr;
}

void eraseReadContext(ReadContextList *readContexts, WindowsReadContext *target)
{
    for (auto it = readContexts->begin(); it != readContexts->end(); ++it)
    {
        if (it->get() == target)
        {
            readContexts->erase(it);
            return;
        }
    }
}

int postClientRead(WindowsReadContext *context, size_t maxMessageSize)
{
    if (context == nullptr || context->pending)
    {
        return 0;
    }

    context->iocp.buffer.resize(maxMessageSize);
    resetWindowsIocpContext(
        &context->iocp,
        IPCWindowsIocpOperation::Read,
        context->endpoint,
        context->iocp.clientFd,
        reinterpret_cast<HANDLE>(context->nativeHandle));

    DWORD readBytes = 0;
    const BOOL ok = ::ReadFile(
        reinterpret_cast<HANDLE>(context->nativeHandle),
        context->iocp.buffer.data(),
        static_cast<DWORD>(context->iocp.buffer.size()),
        &readBytes,
        &context->iocp.overlapped);
    LOG_DXRT_S_DBG << "WIN ReadFile rc=" << ok
                   << " bytes=" << readBytes
                   << " err=" << (ok ? 0 : ::GetLastError())
                   << " fd=" << context->iocp.clientFd
                   << " handle=" << context->nativeHandle << std::endl;
    if (ok == TRUE)
    {
        const IPCNativeHandle iocpHandle = context->endpoint != nullptr
            ? context->endpoint->getWindowsIocpNativeHandle()
            : kInvalidIPCNativeHandle;
        if (iocpHandle == kInvalidIPCNativeHandle)
        {
            errno = EBADF;
            return -1;
        }

        context->pending = true;
        if (::PostQueuedCompletionStatus(
                reinterpret_cast<HANDLE>(iocpHandle),
                readBytes,
                static_cast<ULONG_PTR>(context->nativeHandle),
                &context->iocp.overlapped) == FALSE)
        {
            context->pending = false;
            errno = EIO;
            return -1;
        }
        return 0;
    }

    if (ok == FALSE)
    {
        const DWORD err = ::GetLastError();
        if (err != ERROR_IO_PENDING)
        {
            errno = (err == ERROR_BROKEN_PIPE || err == ERROR_PIPE_NOT_CONNECTED || err == ERROR_NO_DATA) ? EPIPE : EIO;
            return -1;
        }
    }

    context->pending = true;
    return 0;
}

int ensureEndpointReadRequests(
    ReadContextList *readContexts,
    IPCServerEndpoint *endpoint,
    size_t maxMessageSize)
{
    const std::vector<std::pair<int, std::shared_ptr<IPCChannel>>> clientSnapshot = endpoint->getClientSnapshot();
    for (const auto &entry : clientSnapshot)
    {
        if (entry.second == nullptr || !entry.second->isValid() ||
            findReadContext(*readContexts, endpoint, entry.first) != nullptr)
        {
            continue;
        }

        std::unique_ptr<WindowsReadContext> context(new WindowsReadContext());
        context->endpoint = endpoint;
        context->nativeHandle = entry.second->getNativeHandle();
        context->iocp.clientFd = entry.first;
        WindowsReadContext *rawContext = context.get();
        readContexts->emplace_back(std::move(context));

        if (postClientRead(rawContext, maxMessageSize) != 0)
        {
            endpoint->removeClient(entry.first);
            eraseReadContext(readContexts, rawContext);
        }
    }

    return 0;
}

int processAcceptCompletion(
    ReadContextList *readContexts,
    IPCWindowsIocpContext *context,
    DWORD error,
    int *ready,
    size_t maxMessageSize)
{
    LOG_DXRT_S_DBG << "WIN accept completion err=" << error << std::endl;
    IPCServerEndpoint *endpoint = static_cast<IPCServerEndpoint *>(context->owner);
    if (endpoint == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    if (error != ERROR_SUCCESS)
    {
        errno = EIO;
        return -1;
    }

    int clientFd = -1;
    if (endpoint->completeAcceptedClient(&clientFd) != 0)
    {
        return -1;
    }

    ++(*ready);
    if (endpoint->ensureAcceptPending() != 0)
    {
        return -1;
    }

    return ensureEndpointReadRequests(readContexts, endpoint, maxMessageSize);
}

int processReadCompletion(
    ReadContextList *readContexts,
    WindowsReadContext *context,
    DWORD transferredBytes,
    DWORD error,
    int *ready,
    size_t maxMessageSize)
{
    LOG_DXRT_S_DBG << "WIN read completion err=" << error
                   << " bytes=" << transferredBytes
                   << " fd=" << (context ? context->iocp.clientFd : -1) << std::endl;
    if (context == nullptr || context->endpoint == nullptr)
    {
        errno = EINVAL;
        return -1;
    }

    context->pending = false;
    IPCServerEndpoint *endpoint = context->endpoint;
    const int clientFd = context->iocp.clientFd;

    if (error != ERROR_SUCCESS || transferredBytes == 0)
    {
        endpoint->removeClient(clientFd);
        eraseReadContext(readContexts, context);
        ++(*ready);
        return 0;
    }

    endpoint->processReceivedClientMessage(
        clientFd,
        context->iocp.buffer.data(),
        static_cast<size_t>(transferredBytes),
        -1);
    ++(*ready);

    std::shared_ptr<IPCChannel> channel = endpoint->getClientChannel(clientFd);
    if (channel == nullptr || channel->getNativeHandle() != context->nativeHandle)
    {
        eraseReadContext(readContexts, context);
        return 0;
    }

    if (postClientRead(context, maxMessageSize) != 0)
    {
        endpoint->removeClient(clientFd);
        eraseReadContext(readContexts, context);
    }

    return 0;
}

int processIocpCompletion(
    ReadContextList *readContexts,
    IPCServerEndpoint *endpoint,
    int timeoutMs,
    size_t maxMessageSize,
    int *ready)
{
    const IPCNativeHandle iocpHandle = endpoint->getWindowsIocpNativeHandle();
    if (iocpHandle == kInvalidIPCNativeHandle)
    {
        errno = EBADF;
        return -1;
    }

    DWORD transferredBytes = 0;
    ULONG_PTR completionKey = 0;
    LPOVERLAPPED completedOverlapped = nullptr;
    const DWORD waitMs = (timeoutMs < 0) ? INFINITE : static_cast<DWORD>(timeoutMs);
    const BOOL ok = ::GetQueuedCompletionStatus(
        reinterpret_cast<HANDLE>(iocpHandle),
        &transferredBytes,
        &completionKey,
        &completedOverlapped,
        waitMs);
    if (completedOverlapped == nullptr)
    {
        if (ok == FALSE && ::GetLastError() == WAIT_TIMEOUT)
        {
            return 0;
        }

        errno = EIO;
        return -1;
    }

    (void)completionKey;
    IPCWindowsIocpContext *context = reinterpret_cast<IPCWindowsIocpContext *>(completedOverlapped);
    const DWORD error = ok ? ERROR_SUCCESS : ::GetLastError();
    LOG_DXRT_S_DBG << "WIN GQCS ok=" << ok
                   << " bytes=" << transferredBytes
                   << " key=0x" << std::hex << completionKey << std::dec
                   << " op=" << static_cast<int>(context->operation)
                   << " err=" << error << std::endl;
    if (context->operation == IPCWindowsIocpOperation::Accept)
    {
        return processAcceptCompletion(readContexts, context, error, ready, maxMessageSize);
    }

    return processReadCompletion(
        readContexts,
        reinterpret_cast<WindowsReadContext *>(context),
        transferredBytes,
        error,
        ready,
        maxMessageSize);
}

}  // namespace

int IPCServerEndpoint::preparePlatformStart()
{
    if (_windowsIocpHandle == kInvalidIPCNativeHandle)
    {
        HANDLE iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
        if (iocp == nullptr)
        {
            errno = EIO;
            return -1;
        }

        _windowsIocpHandle = reinterpret_cast<IPCNativeHandle>(iocp);
    }

    _acceptor.setWindowsIocpHandle(_windowsIocpHandle);
    return 0;
}

void IPCServerEndpoint::cleanupPlatformStop() noexcept
{
    eraseEndpointReadContexts(this);
    if (_windowsIocpHandle != kInvalidIPCNativeHandle)
    {
        ::CloseHandle(reinterpret_cast<HANDLE>(_windowsIocpHandle));
        _windowsIocpHandle = kInvalidIPCNativeHandle;
    }
}

int IPCServerEndpoint::allocateClientFd(const IPCChannel &channel)
{
    (void)channel;
    return _nextClientId++;
}

IPCNativeHandle IPCServerEndpoint::getWindowsIocpNativeHandle() const
{
    return _windowsIocpHandle;
}

int IPCServerEndpoint::ensureAcceptPending()
{
    return _acceptor.ensureAcceptPending(this);
}

int IPCServerEndpoint::completeAcceptedClient(int *clientFdOut)
{
    auto newChannel = std::make_shared<IPCChannel>();
    if (_acceptor.completeAccept(newChannel.get()) != 0)
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

    if (clientFdOut != nullptr)
    {
        *clientFdOut = clientFd;
    }

    LOG_DXRT_S_DBG << "IPCServerEndpoint::acceptNewClient clientFd=" << clientFd << std::endl;
    if (_onClientConnected)
    {
        _onClientConnected(clientFd);
    }
    return 0;
}

int IPCServerEndpoint::pollPlatform(int timeoutMs, size_t maxMessageSize)
{
    if (!isRunning())
    {
        errno = EBADF;
        return -1;
    }

    if (maxMessageSize == 0 || maxMessageSize > static_cast<size_t>(0xFFFFFFFFu))
    {
        errno = EINVAL;
        return -1;
    }

    int ready = 0;
    if (ensureAcceptPending() != 0)
    {
        return -1;
    }

    ReadContextList &readContexts = getReadContexts(this);
    if (ensureEndpointReadRequests(&readContexts, this, maxMessageSize) != 0)
    {
        return -1;
    }

    if (processIocpCompletion(&readContexts, this, timeoutMs, maxMessageSize, &ready) != 0)
    {
        return ready > 0 ? ready : -1;
    }

    return ready;
}

}  // namespace dxrt

#endif
