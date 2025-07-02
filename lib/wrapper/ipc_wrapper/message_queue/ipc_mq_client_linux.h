// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>
#include <mutex>
#include <map>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_client.h"
#include "ipc_mq_linux.h"

namespace dxrt 
{

    class IPCMessageQueueClientLinux : public IPCClient
    {
    private:
        IPCMessageQueueLinux _messageQueueToServer;
        IPCMessageQueueLinux _messageQueueToClient;
        void* _usrData;
        long _msgType;
        std::thread _thread;
        std::atomic<bool> _threadRunning{false};
        std::atomic<bool> _stop{false};
        std::function<int32_t(IPCServerMessage&,void*)> _receiveCB;
        std::map<int, std::shared_ptr<std::promise<IPCServerMessage> > >_waitingCall;
        std::mutex _futureLock;
        std::mutex _funcLock;

    public:

        IPCMessageQueueClientLinux(long msgType);
        virtual ~IPCMessageQueueClientLinux();

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

        static void ThreadFunc(IPCMessageQueueClientLinux* socketClient);
    };

}  // namespace dxrt