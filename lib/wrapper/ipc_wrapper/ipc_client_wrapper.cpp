
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include <cerrno>
#ifdef __linux__
#include "message_queue/ipc_mq_client_linux.h"
#elif _WIN32
#include "windows_pipe/ipc_pipe_client_windows.h"
#endif
#include "dxrt/ipc_wrapper/ipc_message.h"

namespace dxrt{

constexpr long IPCClientWrapper::MAX_PID = 0x20000000;  // default max pid value


IPCClientWrapper::IPCClientWrapper(IPC_TYPE type, long msgType)
{
#ifdef __linux__
    // Linux: Legacy message-queue IPC is disabled. Use Dynamic IPC via ServiceLayer.
    LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper legacy message queue is not supported on Linux. Use ServiceLayer Dynamic IPC.");
    _ipcClient = nullptr;
    (void)type;
    (void)msgType;

#elif _WIN32
    // Windows: Legacy IPC pipe is disabled. Use ServiceLayer with Dynamic IPC instead.
    LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper legacy pipe not supported on Windows. Use ServiceLayer Dynamic IPC.");
    _ipcClient = nullptr;
    (void)type;
    (void)msgType;

#else
    LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper No implementation");
    (void)type;
    (void)msgType;
#endif
}

IPCClientWrapper::~IPCClientWrapper()
{
    _ipcClient = nullptr;
}

// Intitialize IPC
int32_t IPCClientWrapper::Initialize(bool enableInternalCB)  // NOSONAR:S5817
{
    std::ignore = enableInternalCB;

    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    int32_t ret = _ipcClient->Initialize();
    return ret;
}

// Send to server
int32_t IPCClientWrapper::SendToServer(IPCClientMessage& clientMessage) const
{
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }
    return _ipcClient->SendToServer(clientMessage);
}

int32_t IPCClientWrapper::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& inClientMessage) const
{
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }
    return _ipcClient->SendToServer(outServerMessage, inClientMessage);
}

// Receive message from server
int32_t IPCClientWrapper::ReceiveFromServer(IPCServerMessage& serverMessage) const
{
    LOG_DXRT_I_DBG << serverMessage.code << std::endl;
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }
    return _ipcClient->ReceiveFromServer(serverMessage);
}

// register receive message callback function
int32_t IPCClientWrapper::RegisterReceiveCB(std::function<int32_t(const IPCServerMessage&, void*)> receiveCB, void* usrData) const
{
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }
    return _ipcClient->RegisterReceiveCB(receiveCB, usrData);
}

int32_t IPCClientWrapper::ClearMessages() const
{
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    // no need callback, only initialize
    return _ipcClient->Initialize();
}

int32_t IPCClientWrapper::Close() const
{
    if (_ipcClient == nullptr)
    {
        errno = ENOTSUP;
        return -1;
    }

    return _ipcClient->Close();
}

}  // namespace dxrt
