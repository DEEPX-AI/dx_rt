/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_server_acceptor.hpp"

namespace dxrt {

IPCServerListenSocket::IPCServerListenSocket() = default;

IPCServerListenSocket::~IPCServerListenSocket()
{
    close();
}

int IPCServerListenSocket::getListenFd() const
{
    return static_cast<int>(_listenHandle);
}

IPCNativeHandle IPCServerListenSocket::getListenNativeHandle() const
{
    return _listenHandle;
}

}  // namespace dxrt
