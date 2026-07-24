/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "../transport/ipc_connector.hpp"

#ifdef _WIN32

#include <windows.h>

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
        case ERROR_PIPE_BUSY:
        case ERROR_SEM_TIMEOUT:
        case ERROR_TIMEOUT:
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

    const std::string pipeName = normalizePipeName(endpoint);

    for (int attempt = 0; attempt <= retryCount; ++attempt)
    {
        HANDLE pipe = ::CreateFileA(
            pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_OVERLAPPED,
            nullptr);
        if (pipe != INVALID_HANDLE_VALUE)
        {
            DWORD mode = PIPE_READMODE_MESSAGE;
            if (::SetNamedPipeHandleState(pipe, &mode, nullptr, nullptr) == FALSE)
            {
                setErrnoFromWindowsError(::GetLastError());
                LOG_DXRT_ERR(formatErrnoMessage("SetNamedPipeHandleState", errno));
            }
            channel->reset(reinterpret_cast<IPCNativeHandle>(pipe));
            return 0;
        }

        const DWORD winErr = ::GetLastError();
        if (attempt == retryCount)
        {
            setErrnoFromWindowsError(winErr);
            LOG_DXRT_ERR(formatErrnoMessage("CreateFileA", errno));
            return -1;
        }

        if (retryDelayMs > 0)
        {
            const DWORD waitMs = static_cast<DWORD>(retryDelayMs);
            (void)::WaitNamedPipeA(pipeName.c_str(), waitMs);
            ::Sleep(waitMs);
        }
    }

    errno = ECONNREFUSED;
    return -1;
}

}  // namespace dxrt

#endif
