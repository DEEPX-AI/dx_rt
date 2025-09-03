/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef __linux__ // all or nothing

#include "ipc_mq_server_linux.h"
#include "ipc_mq_linux.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <csignal>


using namespace dxrt;


IPCMessageQueueServerLinux::IPCMessageQueueServerLinux()
: _usrData(nullptr)
{
    LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Constructor" << std::endl;
}


IPCMessageQueueServerLinux::~IPCMessageQueueServerLinux()
{
    LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Destructor" << std::endl;

    Close();
}

// Intitialize IPC Server
// return error code
int32_t IPCMessageQueueServerLinux::Initialize()
{
    LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Initialize" << std::endl;

    int ret = _messageQueueToServer.Initialize(IPCMessageQueueLinux::SERVER_MSG_TYPE, IPCMessageQueueDirection::TO_SERVER);
    if (ret != 0)
    {
        return ret;
    }
    ret = _messageQueueToServer.Delete();
    if (ret != 0)
    {
        return ret;
    }
    ret = _messageQueueToServer.Initialize(IPCMessageQueueLinux::SERVER_MSG_TYPE, IPCMessageQueueDirection::TO_SERVER);
    if (ret != 0)
    {
        _messageQueueToServer.Delete();
        return ret;
    }
    ret = _messageQueueToClient.Initialize(IPCMessageQueueLinux::SERVER_MSG_TYPE, IPCMessageQueueDirection::TO_CLIENT);
    if (ret != 0)
    {
        _messageQueueToServer.Delete();
        return ret;
    }
    ret = _messageQueueToClient.Delete();
    if (ret != 0)
    {
        _messageQueueToServer.Delete();
        return ret;
    }
    return _messageQueueToClient.Initialize(IPCMessageQueueLinux::SERVER_MSG_TYPE, IPCMessageQueueDirection::TO_CLIENT);

}

// listen
int32_t IPCMessageQueueServerLinux::Listen()
{
    LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Listen" << std::endl;

    return 0;
}

int32_t IPCMessageQueueServerLinux::Select(int64_t& connectedFd)
{
    (void)connectedFd;
    
    return 0;
}


// ReceiveFromClient
// return 0: no data, -1: no connection
int32_t IPCMessageQueueServerLinux::ReceiveFromClient(IPCClientMessage& clientMessage)
{
    IPCMessageQueueLinux::Message mq_message;
    
    if ( _messageQueueToServer.Receive(mq_message, sizeof(clientMessage), IPCMessageQueueLinux::SERVER_MSG_TYPE) == 0 )
    {
        memcpy(&clientMessage, mq_message.data, sizeof(clientMessage));
    }
    else 
    {
        return -1;
    }
    
    return 0;
}

// SendToClient
int32_t IPCMessageQueueServerLinux::SendToClient(IPCServerMessage& serverMessage)
{
    IPCMessageQueueLinux::Message mq_message;
    mq_message.msgType = serverMessage.msgType;
    memcpy(mq_message.data, &serverMessage, sizeof(serverMessage));

    return _messageQueueToClient.Send(mq_message, sizeof(serverMessage));
}

int32_t IPCMessageQueueServerLinux::RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData)
{

    
    if ( _threadRunning.load() )
    {
        _threadRunning.store(false);
        /*if ( _thread.joinable() )
        {
            _thread.join();
        }*/
        _thread.detach();

        _receiveCB = nullptr;
        LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux: Detached Callback Thread" << std::endl;
    }


    if ( _messageQueueToServer.IsAvailable() )
    {
        _receiveCB = receiveCB;
        _usrData = usrData;

        if ( _receiveCB != nullptr ) 
        {
            _threadRunning.store(true);
            _thread = std::thread(IPCMessageQueueServerLinux::ThreadFunc, this);
            LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux: Created Callback Thread" << std::endl;
        }
    }
    
    return 0;
}

// Close
int32_t IPCMessageQueueServerLinux::Close()
{
    if ( _threadRunning.load() )
    {
        RegisterReceiveCB(nullptr, nullptr);
    }


    if ( _messageQueueToServer.IsAvailable() )
    {
        _messageQueueToServer.Delete();
        LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Close" << std::endl;
    }

    if ( _messageQueueToClient.IsAvailable() )
    {
        _messageQueueToClient.Delete();
        LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux::Close" << std::endl;
    }

    return 0;
}


void IPCMessageQueueServerLinux::ThreadFunc(IPCMessageQueueServerLinux* mqServer)
{


    while(mqServer->_threadRunning.load())
    {
        
        IPCClientMessage clientMessage;
        int32_t result = mqServer->ReceiveFromClient(clientMessage);
        mqServer->_receiveCB(clientMessage, mqServer->_usrData, result);

        LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux: Thread Running" << std::endl;
    }

    LOG_DXRT_I_DBG << "IPCMessageQueueServerLinux: Callback Thread Finished" << std::endl;
}

#endif // _linux
