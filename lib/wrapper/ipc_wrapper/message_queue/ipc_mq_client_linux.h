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
#include <map>
#include <memory>

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
    std::function<int32_t(IPCServerMessage&, void*)> _receiveCB;
    std::map<int, std::shared_ptr<std::promise<IPCServerMessage> > >_waitingCall;
    std::mutex _futureLock;
    std::mutex _funcLock;
    std::atomic<bool> _dummyClosePending{false};

 public:
    explicit IPCMessageQueueClientLinux(long msgType);
    virtual ~IPCMessageQueueClientLinux();

    // Intitialize IPC
    int32_t Initialize() override;

    // Send message to server
    int32_t SendToServer(IPCClientMessage& clientMessage) override;

    // Send message to server
    int32_t SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage) override;

    // Receive message from server
    int32_t ReceiveFromServer(IPCServerMessage& serverMessage) override;

    // register receive message callback function
    int32_t RegisterReceiveCB(std::function<int32_t(IPCServerMessage&,void*)> receiveCB, void* usrData) override;

    // close the connection
    int32_t Close() override;

    static void ThreadFunc(IPCMessageQueueClientLinux* socketClient);
};

}  // namespace dxrt
