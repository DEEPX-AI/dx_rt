// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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
#include <memory>
#include <mutex>
#include <atomic>

#include "ipc_mq_client_linux.h"
#include "../../../include/dxrt/common.h"

using namespace dxrt;


IPCMessageQueueClientLinux::IPCMessageQueueClientLinux(long msgType)
: _usrData(nullptr), _msgType(msgType), _receiveCB(nullptr)
{
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Constructor (msgType=" << msgType << ")" << std::endl;
}

IPCMessageQueueClientLinux::~IPCMessageQueueClientLinux()
{
    //LOG_DXRT_DBG << "IPCMessageQueueClientLinux::Destructor" << std::endl;
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Destructor" << std::endl;
    
    Close();
}


// Intitialize IPC
int32_t IPCMessageQueueClientLinux::Initialize()
{
    // review
    //std::lock_guard<std::mutex> lock(_funcLock);

    //LOG_DXRT_DBG << "IPCMessageQueueClientLinux::Initialize" << std::endl;
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Initialize" << std::endl;

    return _messageQueue.Initialize(_msgType);
}
//std::mutex IPCMessageQueueClientLinux::_lock;

//static std::atomic<int> seq_increment = {1};
int32_t IPCMessageQueueClientLinux::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& clientMessage)
{
    //std::lock_guard<std::mutex> lock(_funcLock);

    if ( _receiveCB == nullptr )
    {
        clientMessage.seqId = 0; //seq_increment++; // review 
        SendToServer(clientMessage);
        ReceiveFromServer(outServerMessage);
    }
    else 
    {

        /*std::unique_lock<std::mutex> lk(_futureLock);
        int seq_id = seq_increment++;
        _waitingCall[seq_id] = std::make_shared<std::promise<IPCServerMessage> >();
        //LOG_DXRT_I_DBG << "set future "<< _waitingCall << "\n";
        DXRT_ASSERT(_waitingCall[seq_id] != nullptr, "_waitingcall make failed");
        auto future = _waitingCall[seq_id]->get_future();
        lk.unlock();
        clientMessage.seqId = seq_id;
        SendToServer(clientMessage);
        //LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: sent client message\n";
        outServerMessage = future.get();
        //LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: future end of waiting\n";
        */
       // if there is a callback, this function does not work
       return -1;

    }

    return 0;
}

int32_t IPCMessageQueueClientLinux::SendToServer(IPCClientMessage& clientMessage)
{
    // review
    std::lock_guard<std::mutex> lock(_funcLock);

    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::SendToServer" << std::endl;

    IPCMessageQueueLinux::Message mq_message;
    mq_message.msgType = IPCMessageQueueLinux::SERVER_MSG_TYPE;

    clientMessage.msgType = _msgType;
    memcpy(mq_message.data, &clientMessage, sizeof(clientMessage));

    return _messageQueue.Send(mq_message, sizeof(clientMessage));
}

int32_t IPCMessageQueueClientLinux::ReceiveFromServer(IPCServerMessage& serverMessage)
{
    IPCMessageQueueLinux::Message mq_message;
    
    if ( _messageQueue.Receive(mq_message, sizeof(serverMessage), _msgType) == 0 )
    {
        memcpy(&serverMessage, mq_message.data, sizeof(serverMessage));
    }
    else
    {
        return -1;
    }
    
    return 0;
}

// register receive message callback function
int32_t IPCMessageQueueClientLinux::RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData)
{
    // review
    //std::lock_guard<std::mutex> lock(_funcLock);
        
    if ( _threadRunning )
    {

        _threadRunning = false;

        IPCServerMessage serverMessage;
        serverMessage.code = dxrt::RESPONSE_CODE::CLOSE;
        serverMessage.msgType = _msgType;
        IPCMessageQueueLinux::Message mq_message;
        mq_message.msgType = serverMessage.msgType;
        memcpy(mq_message.data, &serverMessage, sizeof(serverMessage));
        _messageQueue.Send(mq_message, sizeof(serverMessage));
        
        if ( _thread.joinable() )
        {
            _thread.join();
        }
        //_thread.detach();

        _receiveCB = nullptr;
        LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Detached Callback Thread" << std::endl;
    }

    if ( _messageQueue.IsAvailable() ) 
    {
        _receiveCB = receiveCB;
        _usrData = usrData;

        if ( _receiveCB != nullptr ) 
        {
            _threadRunning = true;
            _thread = std::thread(IPCMessageQueueClientLinux::ThreadFunc, this);
            LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Created Callback Thread" << std::endl;
        }
    } // socketFd
    
    return 0;
}

// close the connection
int32_t IPCMessageQueueClientLinux::Close()
{
    // review
    //std::lock_guard<std::mutex> lock(_funcLock);

    if ( _threadRunning )
    {
        RegisterReceiveCB(nullptr, nullptr);
    }

    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Close" << std::endl;
    
    return 0;
}


void IPCMessageQueueClientLinux::ThreadFunc(IPCMessageQueueClientLinux* mqClient)
{

    while(mqClient->_threadRunning)
    {
        IPCServerMessage serverMessage;
        serverMessage.msgType = getpid();
        if (mqClient->ReceiveFromServer(serverMessage) != -1) {
            LOG_DXRT_I_DBG << "Thread Running by " << serverMessage.code << std::endl;
            if ( mqClient->_receiveCB != nullptr )
            {
                mqClient->_receiveCB(serverMessage, mqClient->_usrData);

                if ( serverMessage.code == dxrt::RESPONSE_CODE::CLOSE ) break;

            }
        } else {
            LOG_DXRT_I_ERR("ReceiveFromServer fail");
        }

    }

    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Thread Finished" << std::endl;
}
