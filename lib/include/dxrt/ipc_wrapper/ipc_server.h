/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

#include "ipc_message.h"

namespace dxrt 
{

    class DXRT_API IPCServer
    {

    public:

        // Intitialize IPC Server
        // return error code
        virtual int32_t Initialize() = 0;

        // listen
        virtual int32_t Listen() = 0;

        // Select
        virtual int32_t Select(int64_t& connectedFd) = 0;

        // ReceiveFromClient
        virtual int32_t ReceiveFromClient(IPCClientMessage& clientMessage) = 0;

        // SendToClient
        virtual int32_t SendToClient(IPCServerMessage& serverMessage) = 0;

        // register receive message callback function
        virtual int32_t RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData) = 0;

        // remove client connection
        virtual int32_t RemoveClient(long msgType) { (void)msgType; return -1; } // for message queue (POSIX)

        // Close
        virtual int32_t Close() = 0;
    };

}  // namespace dxrt