/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_channel.hpp"

#ifdef _WIN32

#include <windows.h>

#include <cerrno>
#include <cstring>
#include <string>

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
        case ERROR_BROKEN_PIPE:
        case ERROR_PIPE_NOT_CONNECTED:
            return EPIPE;
        case ERROR_PIPE_BUSY:
        case ERROR_SEM_TIMEOUT:
        case ERROR_TIMEOUT:
        case ERROR_NO_DATA:
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

HANDLE makeOverlappedEvent()
{
    return ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
}

HANDLE makeCompletionPortSkippedEvent(HANDLE event)
{
    return reinterpret_cast<HANDLE>(reinterpret_cast<ULONG_PTR>(event) | 1u);
}

}  // namespace

int IPCChannel::sendMessage(const uint8_t *data, size_t size, int fd) const
{
    const IPCNativeHandle handle = getNativeHandle();
    if (handle == kInvalidIPCNativeHandle)
    {
        errno = EBADF;
        return -1;
    }

    // fd is not supported on Windows IPC channel (named pipe), so it is ignored. --- IGNORE ---
    std::ignore = fd;

    if (size > static_cast<size_t>(0xFFFFFFFFu))
    {
        errno = EMSGSIZE;
        return -1;
    }

    HANDLE pipe = reinterpret_cast<HANDLE>(handle);
    DWORD written = 0;
    OVERLAPPED ov = {};
    HANDLE event = makeOverlappedEvent();
    if (event == nullptr)
    {
        errno = EIO;
        return -1;
    }
    ov.hEvent = makeCompletionPortSkippedEvent(event);
    BOOL ok = ::WriteFile(
        pipe,
        (size == 0) ? nullptr : static_cast<const void *>(data),
        static_cast<DWORD>(size),
        &written,
        &ov);
    if (!ok) {
        DWORD err = ::GetLastError();
        if (err == ERROR_IO_PENDING) {
            ok = ::GetOverlappedResult(pipe, &ov, &written, TRUE);
        } else {
            ::CloseHandle(event);
            setErrnoFromWindowsError(err);
            LOG_DXRT_ERR(formatErrnoMessage("WriteFile", errno));
            return -1;
        }
    }
    ::CloseHandle(event);
    if (!ok) {
        setErrnoFromWindowsError(::GetLastError());
        LOG_DXRT_ERR(formatErrnoMessage("WriteFile", errno));
        return -1;
    }
    LOG_DXRT_I_DBG << "WIN WriteFile written=" << written
                   << " size=" << size
                   << " pipe=" << pipe << std::endl;
    return static_cast<int>(written);
}

int IPCChannel::receiveMessage(uint8_t *buffer, size_t bufferSize, int *fd) const
{
    std::ignore = fd;  // fd is not supported on Windows IPC channel (named pipe), so it is ignored. --- IGNORE ---

    const IPCNativeHandle handle = getNativeHandle();
    if (handle == kInvalidIPCNativeHandle)
    {
        errno = EBADF;
        return -1;
    }

    if (bufferSize > static_cast<size_t>(0xFFFFFFFFu))
    {
        errno = EMSGSIZE;
        return -1;
    }

    HANDLE pipe = reinterpret_cast<HANDLE>(handle);
    DWORD readBytes = 0;
    OVERLAPPED ov = {};
    HANDLE event = makeOverlappedEvent();
    if (event == nullptr)
    {
        errno = EIO;
        return -1;
    }
    ov.hEvent = makeCompletionPortSkippedEvent(event);
    BOOL ok = ::ReadFile(
        pipe,
        (bufferSize == 0) ? nullptr : static_cast<void *>(buffer),
        static_cast<DWORD>(bufferSize),
        &readBytes,
        &ov);
    if (!ok) {
        DWORD err = ::GetLastError();
        if (err == ERROR_IO_PENDING) {
            ok = ::GetOverlappedResult(pipe, &ov, &readBytes, TRUE);
        } else if (err == ERROR_MORE_DATA) {
            ::CloseHandle(event);
            errno = EMSGSIZE;
            LOG_DXRT_ERR("ReadFile failed: named-pipe message is larger than receive buffer");
            return -1;
        } else {
            ::CloseHandle(event);
            setErrnoFromWindowsError(err);
            LOG_DXRT_ERR(formatErrnoMessage("ReadFile", errno));
            return -1;
        }
    }
    ::CloseHandle(event);
    if (!ok) {
        setErrnoFromWindowsError(::GetLastError());
        LOG_DXRT_ERR(formatErrnoMessage("ReadFile", errno));
        return -1;
    }
    return static_cast<int>(readBytes);
}

void IPCChannel::close()
{
    const IPCNativeHandle handle = release();
    if (handle != kInvalidIPCNativeHandle)
    {
        HANDLE pipe = reinterpret_cast<HANDLE>(handle);
        // Unblock any thread blocked in ReadFile() before closing the handle.
        // CancelIoEx cancels all pending I/O on this handle in the current process.
        ::CancelIoEx(pipe, nullptr);
        if (::CloseHandle(pipe) == FALSE)
        {
            setErrnoFromWindowsError(::GetLastError());
            LOG_DXRT_ERR(formatErrnoMessage("CloseHandle", errno));
        }
    }
}

}  // namespace dxrt

#endif
