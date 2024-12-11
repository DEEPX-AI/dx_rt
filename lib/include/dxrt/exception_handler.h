// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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
