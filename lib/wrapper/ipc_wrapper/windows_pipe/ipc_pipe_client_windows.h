/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#ifdef _WIN32 // all or nothing

#include <stdint.h>
#include <cstdint>
#include <future>
#include <mutex>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "../../../include/dxrt/ipc_wrapper/ipc_client.h"
#include "ipc_pipe_windows.h"
#include <map>
#include <set>

namespace dxrt 
{
    class IPCPipeClientWindows : public IPCClient
    {
    private:
        std::atomic<bool> _stop{ false };

    	IPCPipeWindows _pipe;
        void* _usrData;
        long _msgType;
        std::thread _thread;
        std::atomic<bool> _threadRunning{false};
        //std::atomic<bool> _stop = {false};
        std::function<int32_t(IPCServerMessage&,void*)> _receiveCB;
        //std::map<int, std::shared_ptr<std::promise<IPCServerMessage> > >_waitingCall;
        //std::mutex _futureLock;
        //std::mutex _funcLock;

#if 0
        std::map<pid_t, HANDLE> _msgType2handle;
        std::queue<IPCServerMessage> _que;
        std::condition_variable _queCv;
        std::mutex _queMt;

    public:
        void enQue(IPCServerMessage& m);
        int32_t deQue(IPCServerMessage& clientMessage);
#endif
    public:

        IPCPipeClientWindows(long msgType);
        virtual ~IPCPipeClientWindows();

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

        static void ThreadFunc(IPCPipeClientWindows* socketClient);
    };

}  // namespace dxrt

#endif
