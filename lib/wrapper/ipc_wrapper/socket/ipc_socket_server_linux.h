// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>
#include <mutex>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_server.h"

namespace dxrt 
{

    class IPCSocketServerLinux : public IPCServer
    {

    private:
        int _socketFd;
        int _epollFd;
        void* _usrData;
        fd_set _allFds, _readFds;
        int _max_sock;

        std::thread _thread;
        std::atomic<bool> _threadRunning = {false};
        std::atomic<bool> _stop = {false};
        std::function<int32_t(IPCClientMessage&,void*,int32_t)> _receiveCB;

    public:

        IPCSocketServerLinux();
        IPCSocketServerLinux(uint64_t fd);
        virtual ~IPCSocketServerLinux();

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

        static void ThreadFunc(IPCSocketServerLinux* socketServer);
    };

}  // namespace dxrt