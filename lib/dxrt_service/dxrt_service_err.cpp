// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "service_error.h"

dxrt::DxrtServiceErr::DxrtServiceErr(dxrt::IPCServerWrapper *ipcServerWrapper)
: _ipcServerWrapper(ipcServerWrapper)
{

}

void dxrt::DxrtServiceErr::ErrorReportToClient(dxrt_server_err_t err, long procId, uint32_t errCode)
{
    dxrt::IPCServerMessage serverMessage;

    serverMessage.code = dxrt::RESPONSE_CODE::ERROR_REPORT;
    serverMessage.data = static_cast<uint64_t>(err);
    serverMessage.result = errCode;
    serverMessage.msgType = procId;

    _ipcServerWrapper->SendToClient(serverMessage);
}
