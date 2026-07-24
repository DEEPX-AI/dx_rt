/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>

#include "dxrt/common.h"
#include "ipc_channel.hpp"

namespace dxrt {

class IPCClientConnector
{
 public:
    static int connect(
    const std::string &endpoint,
        IPCChannel *channel,
        int retryCount = 0,
        int retryDelayMs = 100);
};

}  // namespace dxrt
