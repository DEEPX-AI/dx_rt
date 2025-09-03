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
#include <future>
#include <mutex>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_server.h"
#include "ipc_mq_linux.h"

namespace dxrt 
{

    class IPCMessageQueueServerLinux : public IPCServer
    {

    private:
        IPCMessageQueueLinux _messageQueueToServer;
        IPCMessageQueueLinux _messageQueueToClient;
        void* _usrData;
        std::thread _thread;
        std::atomic<bool> _threadRunning{false};
        std::function<int32_t(IPCClientMessage&,void*,int32_t)> _receiveCB;

    public:

        IPCMessageQueueServerLinux();
        virtual ~IPCMessageQueueServerLinux();

        // Intitialize IPC Server
        // return error code
        virtual int32_t Initialize();

        // listen
        virtual int32_t Listen();

        // Select
        virtual int32_t Select(int64_t& connectedFd);

        // ReceiveFromClient
        virtual int32_t ReceiveFromClient(IPCClientMessage& clientMessage);

        // SendToClient
        virtual int32_t SendToClient(IPCServerMessage& serverMessage);

        // register receive message callback function
        virtual int32_t RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData);

        // Close
        virtual int32_t Close();

        static void ThreadFunc(IPCMessageQueueServerLinux* socketServer);
    };

}  // namespace dxrt