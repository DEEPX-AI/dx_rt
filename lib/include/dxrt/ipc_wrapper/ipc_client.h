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

    class DXRT_API IPCClient
    {

    public:

        // Intitialize IPC
        virtual int32_t Initialize() = 0;

        // Send message to server
        virtual int32_t SendToServer(IPCClientMessage& clientMessage) = 0;

        // Send message to server
        virtual int32_t SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage) = 0;

        // Receive message from server
        virtual int32_t ReceiveFromServer(IPCServerMessage& serverMessage) = 0;

        // register receive message callback function
        virtual int32_t RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData) = 0;

        // close the connection
        virtual int32_t Close() = 0;
    };

}  // namespace dxrt