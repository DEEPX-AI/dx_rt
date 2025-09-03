/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"

namespace dxrt {
class DXRT_API ExceptionHandler
{
public:
    ExceptionHandler();

private:
};

extern ExceptionHandler exceptionHandler;
} // namespace dxrt
