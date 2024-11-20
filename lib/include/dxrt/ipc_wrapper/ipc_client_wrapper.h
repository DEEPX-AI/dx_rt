// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

#include "ipc_message.h"
#include "ipc_client.h"

namespace dxrt 
{

    class IPCClientWrapper 
    {

    private:
        std::shared_ptr<IPCClient> _ipcClient;

    public:

        IPCClientWrapper(IPC_TYPE type = IPC_TYPE::SOCKET_SYNC, long msgType = 0);  // msgType only for MessageQueue
        virtual ~IPCClientWrapper();

        // Intitialize IPC
        int32_t Initialize();

        // Send message to server
        int32_t SendToServer(IPCClientMessage& clientMessage);

        // Send message to server
        int32_t SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage);

        // Receive message from server
        int32_t ReceiveFromServer(IPCServerMessage& serverMessage);

        // register receive message callback function
        int32_t RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData);

        // clear all messages
        int32_t ClearMessages();

        // close the connection
        int32_t Close();
    };

}  // namespace dxrt