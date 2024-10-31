
// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "../../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "socket_sync/ipc_client_linux.h"
#include "socket/ipc_socket_client_linux.h"
#include "message_queue/ipc_mq_client_linux.h"
#include "dxrt/ipc_wrapper/ipc_message.h"

using namespace dxrt;

namespace dxrt{
    int ipc_callBack(dxrt::IPCServerMessage& outResponseServerMessage, void* usrData);
}


IPCClientWrapper::IPCClientWrapper(IPC_TYPE type, long msgType)
{
    // implementation for linux
    if ( type == IPC_TYPE::SOCKET_CB )
    {
        _ipcClient = std::make_shared<IPCSocketClientLinux>();
    }
    else if ( type == IPC_TYPE::MESSAE_QUEUE )
    {
        _ipcClient = std::make_shared<IPCMessageQueueClientLinux>(msgType);
    }  
    else if ( type == IPC_TYPE::SOCKET_SYNC )
    {
        _ipcClient = std::make_shared<IPCClientLinux>();
    }
    else 
    {
        LOG_DXRT_I_ERR("[ERROR] IPCClientWrapper No implementation");
    }
    
    //_ipcClient = std::make_shared<IPCClientWindows>();
    
}

IPCClientWrapper::~IPCClientWrapper()
{
    _ipcClient = nullptr;
}

// Intitialize IPC
int32_t IPCClientWrapper::Initialize()
{

    int32_t ret = _ipcClient->Initialize();
    if (ret == 0)
    {
        RegisterReceiveCB(ipc_callBack, nullptr);
    }
    return ret;
}
//std::mutex sendLock;
// Send to server
int32_t IPCClientWrapper::SendToServer(IPCClientMessage& clientMessage)
{
    //std::unique_lock<std::mutex> lock(sendLock);
    if(_ipcClient == nullptr) return -1;
    return _ipcClient->SendToServer(clientMessage);
}

int32_t IPCClientWrapper::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& inClientMessage)
{
    if(_ipcClient == nullptr) return -1;
    //std::unique_lock<std::mutex> lock(sendLock);
    return _ipcClient->SendToServer(outServerMessage, inClientMessage);
}

// Receive message from server
int32_t IPCClientWrapper::ReceiveFromServer(IPCServerMessage& serverMessage)
{
    if(_ipcClient == nullptr) return -1;
    return _ipcClient->ReceiveFromServer(serverMessage);
}

// register receive message callback function
int32_t IPCClientWrapper::RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData)
{
    if(_ipcClient == nullptr) return -1;
    return _ipcClient->RegisterReceiveCB(receiveCB, usrData);
}

int32_t IPCClientWrapper::ClearMessages()
{
    // no need callback, only initialize 
    return _ipcClient->Initialize();
}

int32_t IPCClientWrapper::Close()
{
    //IPCClientMessage clientMessage;
    //clientMessage.code = REQUEST_CODE::CLOSE;
    //SendToServer(clientMessage);
    return _ipcClient->Close();
}
