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
#include "ipc_server.h"

namespace dxrt 
{

    class DXRT_API IPCServerWrapper 
    {

    private:
        std::shared_ptr<IPCServer> _ipcServer;

    public:

        IPCServerWrapper(IPC_TYPE type = IPC_TYPE::MESSAE_QUEUE);
        virtual ~IPCServerWrapper();

        // Intitialize IPC Server
        // return error code
        int32_t Initialize();

        // listen
        int32_t Listen();

        // Select
        int32_t Select(int64_t& connectedFd);

        // ReceiveFromClient
        int32_t ReceiveFromClient(IPCClientMessage& clientMessage);

        // SendToClient
        int32_t SendToClient(IPCServerMessage& serverMessage);

        // register receive message callback function
        // reciveCB(message, usrData, result)
        // result: -1:error else recevied data size
        int32_t RegisterReceiveCB(std::function<int32_t(IPCClientMessage&,void*,int32_t)> receiveCB, void* usrData);

        // remove client connection (link)
        int32_t RemoveClient(long msgType); // Only for Message Queue (POSIX)

        // Close
        int32_t Close();
            
    };

}  // namespace dxrt