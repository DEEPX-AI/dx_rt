// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <map>

#include "dxrt/common.h"
#include "dxrt/exception/server_err.h"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"

namespace dxrt {

class DxrtServiceErr
{
private:
    dxrt::IPCServerWrapper *_ipcServerWrapper;

public:
    DxrtServiceErr(dxrt::IPCServerWrapper *ipcServerWrapper);

    void ErrorReportToClient(dxrt_server_err_t err, long procId, uint32_t errCode, int deviceId);
};

} //namespace dxrt
