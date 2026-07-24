/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "ipc_channel.hpp"

namespace dxrt {

IPCChannel::IPCChannel() = default;

IPCChannel::IPCChannel(IPCNativeHandle handle)
{
    _handle.store(handle, std::memory_order_release);
}

IPCChannel::~IPCChannel()
{
    close();
}

bool IPCChannel::isValid() const
{
    return _handle.load(std::memory_order_acquire) != kInvalidIPCNativeHandle;
}

int IPCChannel::getFd() const
{
    return static_cast<int>(_handle.load(std::memory_order_acquire));
}

IPCNativeHandle IPCChannel::getNativeHandle() const
{
    return _handle.load(std::memory_order_acquire);
}

void IPCChannel::reset(IPCNativeHandle handle)
{
    if (_handle.load(std::memory_order_acquire) == handle)
    {
        return;
    }

    close();
    _handle.store(handle, std::memory_order_release);
}

IPCNativeHandle IPCChannel::release()
{
    return _handle.exchange(kInvalidIPCNativeHandle, std::memory_order_acq_rel);
}

}  // namespace dxrt
