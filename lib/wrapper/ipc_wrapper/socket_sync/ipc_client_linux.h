// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_client.h"

namespace dxrt 
{

    class IPCClientLinux : public IPCClient
    {
    public:
        static const char* SOCKET_NAME;

    private:
        int _socketFd;
        //int _deviceId;
        std::shared_ptr<std::promise<IPCServerMessage> >_waitingCall;
        std::thread _thread;
        std::atomic<bool> _stop = {false};
        std::function<int32_t(IPCServerMessage&,void*)> _receiveCB;

    public:

        IPCClientLinux();
        virtual ~IPCClientLinux();

        // Intitialize IPC
        virtual int32_t Initialize();

        // Send message to server
        virtual int32_t SendToServer(IPCClientMessage& clientMessage);

        // Send message to server
        virtual int32_t SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage);

        // Receive message from server
        virtual int32_t ReceiveFromServer(IPCServerMessage& serverMessage);

        // register receive message callback function
        virtual int32_t RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData);

        // close the connection
        virtual int32_t Close();

        void threadFunc();
    };

}  // namespace dxrt