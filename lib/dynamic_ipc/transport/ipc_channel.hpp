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
#include <stddef.h>

#include <atomic>

#include "dxrt/common.h"

namespace dxrt {

using IPCNativeHandle = intptr_t;
static constexpr IPCNativeHandle kInvalidIPCNativeHandle = static_cast<IPCNativeHandle>(-1);

class IPCChannel
{
 public:
    IPCChannel();
  explicit IPCChannel(IPCNativeHandle handle);
    ~IPCChannel();

    IPCChannel(const IPCChannel &) = delete;
    IPCChannel &operator=(const IPCChannel &) = delete;

    int sendMessage(const uint8_t *data, size_t size, int fd) const;
    int receiveMessage(uint8_t *buffer, size_t bufferSize, int *fd) const;

    bool isValid() const;
    int getFd() const;
    IPCNativeHandle getNativeHandle() const;
    void reset(IPCNativeHandle handle = kInvalidIPCNativeHandle);

    void close();

 private:
    IPCNativeHandle release();
  std::atomic<IPCNativeHandle> _handle{kInvalidIPCNativeHandle};
};

}  // namespace dxrt

