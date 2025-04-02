
// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "../../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"
#ifdef __linux__
#include "message_queue/ipc_mq_client_linux.h"
#elif _WIN32
#include "windows_pipe/ipc_pipe_client_windows.h"
#endif
#include "dxrt/ipc_wrapper/ipc_message.h"

using namespace dxrt;

const long IPCClientWrapper::MAX_PID = 0x20000000; // default max pid value

namespace dxrt{
    int ipc_callBack(dxrt::IPCServerMessage& outResponseServerMessage, void* usrData);
}


IPCClientWrapper::IPCClientWrapper(IPC_TYPE type, long msgType)
{
#ifdef __linux__
    if (type == IPC_TYPE::MESSAE_QUEUE)
    {
        _ipcClient = std::make_shared<IPCMessageQueueClientLinux>(msgType);
    }
    
#elif _WIN32
    if (type == IPC_TYPE::WIN_PIPE)
    {
        _ipcClient = std::make_shared<IPCPipeClientWindows>(msgType);
    }
#endif
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
int32_t IPCClientWrapper::Initialize(bool enableInternalCB)
{

    int32_t ret = _ipcClient->Initialize();

    if ( enableInternalCB ) 
    {
        if (ret == 0)
        {
            RegisterReceiveCB(ipc_callBack, nullptr);
        }
    } // register internal callback
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
    LOG_DXRT_I_DBG << serverMessage.code << std::endl;
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
