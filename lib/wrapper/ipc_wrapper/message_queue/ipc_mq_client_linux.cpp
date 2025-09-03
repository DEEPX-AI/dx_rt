/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#ifdef __linux__ // all or nothing

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

namespace dxrt {

IPCMessageQueueClientLinux::IPCMessageQueueClientLinux(long msgType)
: _usrData(nullptr), _msgType(msgType), _receiveCB(nullptr)
{
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Constructor (msgType=" << msgType << ")" << std::endl;
}

IPCMessageQueueClientLinux::~IPCMessageQueueClientLinux()
{
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Destructor" << std::endl;

    // Ensure thread is stopped before destruction
    if (_threadRunning.load()) {
        LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Thread still running in destructor, stopping..." << std::endl;
        Close();
    }
}


// Intitialize IPC
int32_t IPCMessageQueueClientLinux::Initialize()
{
    // review
    // std::lock_guard<std::mutex> lock(_funcLock);

    // LOG_DXRT_DBG << "IPCMessageQueueClientLinux::Initialize" << std::endl;
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Initialize" << std::endl;
    int ret = _messageQueueToClient.Initialize(_msgType, IPCMessageQueueDirection::TO_CLIENT);
    if (ret != 0)
    {
        return ret;
    }
    return _messageQueueToServer.Initialize(_msgType, IPCMessageQueueDirection::TO_SERVER);

}
// std::mutex IPCMessageQueueClientLinux::_lock;

// static std::atomic<int> seq_increment = {1};
int32_t IPCMessageQueueClientLinux::SendToServer(IPCServerMessage& outServerMessage, IPCClientMessage& clientMessage)
{
    // std::lock_guard<std::mutex> lock(_funcLock);

    if ( _receiveCB == nullptr )
    {
        clientMessage.seqId = 0;  // seq_increment++; // review
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
    memset(&mq_message, 0, sizeof(mq_message));
    mq_message.msgType = IPCMessageQueueLinux::SERVER_MSG_TYPE;

    clientMessage.msgType = _msgType;
    memcpy(mq_message.data, &clientMessage, sizeof(clientMessage));

    return _messageQueueToServer.Send(mq_message, sizeof(clientMessage));
}

int32_t IPCMessageQueueClientLinux::ReceiveFromServer(IPCServerMessage& serverMessage)
{
    IPCMessageQueueLinux::Message mq_message;

    if ( _messageQueueToClient.Receive(mq_message, sizeof(serverMessage), _msgType) == 0 )
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
int32_t IPCMessageQueueClientLinux::RegisterReceiveCB(
        std::function<int32_t(IPCServerMessage&, void*)> receiveCB,
        void* usrData)
{
    // 1) nullptr means callback release, thread remains
    if (receiveCB == nullptr) {
        _receiveCB = nullptr;
        _usrData   = nullptr;
        LOG_DXRT_I_DBG << "RegisterReceiveCB: callback cleared (thread alive)" << std::endl;
        return 0;
    }

    // 2) If the thread is running, terminate (dump CLOSE)
    if (_threadRunning.load()) {
        _threadRunning.store(false);
        _dummyClosePending.store(true);

        IPCServerMessage dummy;
        dummy.code    = dxrt::RESPONSE_CODE::CLOSE;
        dummy.msgType = _msgType;

        IPCMessageQueueLinux::Message msg;
        msg.msgType = dummy.msgType;
        memcpy(msg.data, &dummy, sizeof(dummy));
        _messageQueueToClient.Send(msg, sizeof(dummy));

        if (_thread.joinable()) {
            _thread.join();
        }

        _receiveCB = nullptr;
        LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Detached Callback Thread" << std::endl;
    }

    // 3) Register new callback and start thread
    if (_messageQueueToClient.IsAvailable()) {
        _receiveCB = receiveCB;
        _usrData   = usrData;

        _threadRunning.store(true);
        _dummyClosePending.store(false);
        _thread = std::thread(IPCMessageQueueClientLinux::ThreadFunc, this);
        LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Created Callback Thread" << std::endl;
    }

    return 0;
}

// close the connection
int32_t IPCMessageQueueClientLinux::Close()
{
    // Stop thread first if running
    if (_threadRunning.load()) {
        _threadRunning.store(false);
        _dummyClosePending.store(true);

        // Send dummy CLOSE message to wake up thread
        IPCServerMessage dummy;
        dummy.code    = dxrt::RESPONSE_CODE::CLOSE;
        dummy.msgType = _msgType;

        IPCMessageQueueLinux::Message msg;
        msg.msgType = dummy.msgType;
        memcpy(msg.data, &dummy, sizeof(dummy));
        _messageQueueToClient.Send(msg, sizeof(dummy));

        // Wait for thread to finish
        if (_thread.joinable()) {
            _thread.join();
        }

        _receiveCB = nullptr;
        LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux: Thread stopped in Close()" << std::endl;
    }
    
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Close" << std::endl;
    return 0;
}


void IPCMessageQueueClientLinux::ThreadFunc(IPCMessageQueueClientLinux* mqClient)
{
    bool closeReceived = false;
    while (mqClient->_threadRunning.load() || !closeReceived) {
        IPCServerMessage serverMessage;
        serverMessage.msgType = getpid();
        LOG_DXRT_I_DBG << "Waiting for message with msgType: " << serverMessage.msgType << std::endl;
        
        if (mqClient->ReceiveFromServer(serverMessage) != -1) {
            LOG_DXRT_I_DBG << "Received message - code: " << static_cast<int>(serverMessage.code) 
                           << ", msgType: " << serverMessage.msgType 
                           << ", deviceId: " << serverMessage.deviceId << std::endl;
            
            if (mqClient->_receiveCB == nullptr)
            {
                LOG_DXRT_I_ERR("Receive callback is null, skipping message");
                continue;
            }

            bool isDummyClose = mqClient->_dummyClosePending.load() &&
                                (serverMessage.code == dxrt::RESPONSE_CODE::CLOSE);
            if (isDummyClose) {
                LOG_DXRT_I_DBG << "Dummy CLOSE message detected, skipping callback" << std::endl;
                mqClient->_dummyClosePending.store(false);
                closeReceived = true;
                continue;
            }

            LOG_DXRT_I_DBG << "Calling receive callback" << std::endl;
            mqClient->_receiveCB(serverMessage, mqClient->_usrData);

            if (serverMessage.code == dxrt::RESPONSE_CODE::CLOSE)
            {
                LOG_DXRT_I_DBG << "Received CLOSE message, will break after queue is empty" << std::endl;
                closeReceived = true;
            }

        } else {
            // Check if thread should stop
            if (!mqClient->_threadRunning.load()) {
                LOG_DXRT_I_DBG << "Thread stopped by _threadRunning flag" << std::endl;
                break;
            }
            LOG_DXRT_I_ERR("ReceiveFromServer fail, errno = "+ std::to_string(errno));
            break;
        }
    }
    LOG_DXRT_I_DBG << "IPCMessageQueueClientLinux::Thread Finished" << std::endl;
}

}  // namespace dxrt
#endif  // __linux__

