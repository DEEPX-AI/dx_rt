/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/ipc_wrapper/ipc_server_wrapper.h"

#include <cerrno>

#ifdef __linux__
	#include "message_queue/ipc_mq_server_linux.h"
#elif _WIN32
	#include "windows_pipe/ipc_pipe_server_windows.h"
#endif

namespace dxrt{

IPCServerWrapper::IPCServerWrapper(IPC_TYPE type)
{
#ifdef __linux__
    // Linux: Legacy message-queue IPC is disabled. Use Dynamic IPC service (v2).
    LOG_DXRT_I_ERR("[ERROR] IPCServerWrapper legacy message queue is not supported on Linux. Use Dynamic IPC service.");
    _ipcServer = nullptr;
    (void)type;
#elif _WIN32
    // Windows: Legacy named-pipe IPC is disabled. Use Dynamic IPC service (v2).
    LOG_DXRT_I_ERR("[ERROR] IPCServerWrapper legacy named pipe is not supported on Windows. Use Dynamic IPC service.");
    _ipcServer = nullptr;
    (void)type;
#endif
}



IPCServerWrapper::~IPCServerWrapper() = default;

// Intitialize IPC Server
// return error code
int32_t IPCServerWrapper::Initialize() const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->Initialize();
}

// listen
int32_t IPCServerWrapper::Listen() const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->Listen();
}

int32_t IPCServerWrapper::Select(int64_t& connectedFd) const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->Select(connectedFd);
}

// ReciveFromClient
int32_t IPCServerWrapper::ReceiveFromClient(IPCClientMessage& clientMessage) const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->ReceiveFromClient(clientMessage);
}

// SendToClient
int32_t IPCServerWrapper::SendToClient(IPCServerMessage& serverMessage) const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->SendToClient(serverMessage);
}

// register receive message callback function
int32_t IPCServerWrapper::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&, void*, int32_t)> receiveCB, void* usrData) const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->RegisterReceiveCB(receiveCB, usrData);
}

int32_t IPCServerWrapper::RemoveClient(long msgType) const // Only for Message Queue (POSIX)
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->RemoveClient(msgType);
}

// Close
int32_t IPCServerWrapper::Close() const
{
    if (_ipcServer == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcServer->Close();
}

}  // namespace dxrt
