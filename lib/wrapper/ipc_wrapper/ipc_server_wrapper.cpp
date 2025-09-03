/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "../../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"

#ifdef __linux__
	#include "message_queue/ipc_mq_server_linux.h"
#elif _WIN32
	#include "windows_pipe/ipc_pipe_server_windows.h"
#endif

namespace dxrt{

IPCServerWrapper::IPCServerWrapper(IPC_TYPE type)
{
#ifdef __linux__
    if (type == IPC_TYPE::MESSAE_QUEUE)
    {
        _ipcServer = std::make_shared<IPCMessageQueueServerLinux>();
    }
#elif _WIN32
    if (type == IPC_TYPE::WIN_PIPE)
    {
        _ipcServer = std::make_shared<IPCPipeServerWindows>();
    }
#endif
    else
    {
        LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper No implementation");
    }


    // _ipcServer = std::make_shared<IPCServerWindows>();
}



IPCServerWrapper::~IPCServerWrapper()
{

}

// Intitialize IPC Server
// return error code
int32_t IPCServerWrapper::Initialize()
{
    return _ipcServer->Initialize();
}

// listen
int32_t IPCServerWrapper::Listen()
{
    return _ipcServer->Listen();
}

int32_t IPCServerWrapper::Select(int64_t& connectedFd)
{
    return _ipcServer->Select(connectedFd);
}

// ReciveFromClient
int32_t IPCServerWrapper::ReceiveFromClient(IPCClientMessage& clientMessage)
{
    return _ipcServer->ReceiveFromClient(clientMessage);
}

// SendToClient
int32_t IPCServerWrapper::SendToClient(IPCServerMessage& serverMessage)
{
    return _ipcServer->SendToClient(serverMessage);
}

// register receive message callback function
int32_t IPCServerWrapper::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&, void*, int32_t)> receiveCB, void* usrData)
{
    return _ipcServer->RegisterReceiveCB(receiveCB, usrData);
}

int32_t IPCServerWrapper::RemoveClient(long msgType)  // Only for Message Queue (POSIX)
{
    return _ipcServer->RemoveClient(msgType);
}

// Close
int32_t IPCServerWrapper::Close()
{
    return _ipcServer->Close();
}

}  // namespace dxrt
