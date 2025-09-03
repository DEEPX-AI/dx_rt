
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
 
#ifdef __linux__ // all or nothing

#include "ipc_mq_linux.h"
#include "dxrt/common.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <unistd.h>

using namespace dxrt;

const int IPCMessageQueueLinux::QUEUE_KEY = 63;
const long IPCMessageQueueLinux::SERVER_MSG_TYPE = 101;

IPCMessageQueueLinux::IPCMessageQueueLinux()
{

}

IPCMessageQueueLinux::~IPCMessageQueueLinux()
{

}

// Intitialize IPC (Message Queue)
int32_t IPCMessageQueueLinux::Initialize(long msgType, IPCMessageQueueDirection direction)
{

    // create key
    key_t key;
    errno = 0;
    if (direction == IPCMessageQueueDirection::TO_SERVER)
    {
        key = 0x2a020467;
    }
    else
    {
        key = 0x54020467;
    }
    if (errno != 0)
    {
        LOG_DXRT_I_ERR("error ftok" << errno);
        return -1;
    }
    // connect 
    //_msgId = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    _msgId = msgget(key, IPC_CREAT | 0666);
    if (_msgId == -1) {
        LOG_DXRT_I_ERR("[IPCMessageQueueLinux] msgget failed" << errno);
        return -1;
    }

    LOG_DXRT_I_DBG << "[IPCMessageQueueLinux] msgget key=" << key << " msgId=" << _msgId << std::endl;

    // check remained message
    Message message;
    int result = 0;
    while(true)
    {
        result = msgrcv(_msgId, &message, sizeof(message.data), msgType, IPC_NOWAIT);
        if ( result == -1 )
        {
            if ( errno == ENOMSG )
            {
                LOG_DXRT_I_DBG << "[IPCMessageQueueLinux] no remained message(s) msgType=" << msgType << std::endl;
                break;
            }
            else 
            {
                LOG_DXRT_I_ERR("[IPCMessageQueueLinux] msgrcv failed" << errno);
                return -1;
            }
        }
        else 
        {
            LOG_DXRT_I_DBG << "[IPCMessageQueueLinux] dequeue remained message(s) msgType=" << msgType << std::endl;

        }
        usleep(1000);
    }

    return 0;
}

// send message
int32_t IPCMessageQueueLinux::Send(const Message& message, size_t size)
{
    if ( _msgId >= 0 )
    {
        // std::cout << "[IPCMessageQueueLinux] Send:" << _msgId << ", " << message.msgType << std::endl;

        // send except message type
        if ( msgsnd(_msgId, &message, size, 0) == -1 )
        {
            LOG_DXRT_I_ERR("[IPCMessageQueueLinux] msgsnd failed");
            return -1;
        }
    }
    else 
    {
        return -1;
    }

    return 0;
}

// receive message
int32_t IPCMessageQueueLinux::Receive(Message& message, size_t size, long msgType)
{
    if ( _msgId >= 0 )
    {

        // receive except message type
        //if (msgrcv(_msgId, &message, sizeof(Message) - sizeof(long), msgType, 0) == -1)
        if ( msgrcv(_msgId, &message, size, msgType, 0) == -1 )
        {
            LOG_DXRT_I_ERR("[IPCMessageQueueLinux] msgrcv 1 failed" << errno);
            return -1;
        }
    }
    else 
    {
        return -1;
    }
    return 0;
}

int32_t IPCMessageQueueLinux::Delete()
{
    if ( _msgId >= 0 )
    {
        if (msgctl(_msgId, IPC_RMID, NULL) == -1) {
            LOG_DXRT_I_ERR("[IPCMessageQueueLinux] fail to delete");
            return -1;
        }
        _msgId = -1;
    }
    return 0;
}

#endif // linux
