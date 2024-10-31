// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

#include "../../../include/dxrt/ipc_wrapper/ipc_server.h"

namespace dxrt 
{

    class IPCServerLinux : public IPCServer
    {

    private:
        int _socketFd;
        fd_set _allFds, _readFds;
        int _max_sock;

    public:

        IPCServerLinux();
        IPCServerLinux(uint64_t fd);
        virtual ~IPCServerLinux();

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
    };

}  // namespace dxrt