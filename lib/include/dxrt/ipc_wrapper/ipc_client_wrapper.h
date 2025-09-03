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
#include "ipc_client.h"

namespace dxrt
{

class DXRT_API IPCClientWrapper
{
 public:
    static const long MAX_PID;

 private:
    std::shared_ptr<IPCClient> _ipcClient;

 public:
    explicit IPCClientWrapper(IPC_TYPE type = IPC_TYPE::MESSAE_QUEUE, long msgType = 0);  // msgType only for MessageQueue
    virtual ~IPCClientWrapper();

    // Intitialize IPC
    int32_t Initialize(bool enableInternalCB = true);

    // Send message to server
    int32_t SendToServer(IPCClientMessage& clientMessage);

    // Send message to server
    int32_t SendToServer(IPCServerMessage& outResponseServerMessage, IPCClientMessage& inRequestClientMessage);

    // Receive message from server
    int32_t ReceiveFromServer(IPCServerMessage& serverMessage);

    // register receive message callback function
    int32_t RegisterReceiveCB(std::function<int32_t(const IPCServerMessage&,void*)> receiveCB, void* usrData);

    // clear all messages
    int32_t ClearMessages();

    // close the connection
    int32_t Close();
};

}  // namespace dxrt
