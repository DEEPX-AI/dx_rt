// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxtop_ipc_client.h"

namespace dxrt {

    DXTopIPCClient::DXTopIPCClient() : _wrapper(dxrt::IPCDefaultType(), getpid())
    {
        _wrapper.Initialize(false);
    }

}