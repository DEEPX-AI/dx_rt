/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef _WIN32 // all or nothing

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>
#include <mutex>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_server.h"
#include "ipc_pipe_windows.h"
#include <map>
#include <set>

namespace dxrt
{

    class IPCPipeServerWindows : public IPCServer
    {
    private:
        std::atomic<bool> _stop{ false };

        IPCPipeWindows _pipe;
        std::map<pid_t, HANDLE> _msgType2handle;
        std::mutex _handleMapMutex;  // Protect _msgType2handle from race conditions
        std::queue<IPCClientMessage> _que;
        std::condition_variable _queCv;
        std::mutex _queMt;

    public:
        void enQue(IPCClientMessage& m);
        int32_t deQue(IPCClientMessage& clientMessage);

    public:
        void ThreadAtServerMainForListen();
        void ThreadAtServerByClient(HANDLE hPipe);

    public:
        IPCPipeServerWindows();
        IPCPipeServerWindows(uint64_t fd);
        virtual ~IPCPipeServerWindows();

        // Intitialize IPC Server : return error code
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

        // static void ThreadFunc(IPCPipeServerWindows* socketServer);
    };

}  // namespace dxrt

#endif
