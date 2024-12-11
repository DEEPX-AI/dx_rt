// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

#include "ipc_message.h"

namespace dxrt 
{

    class IPCServer
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