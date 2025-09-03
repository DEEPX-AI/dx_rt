/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "service_error.h"

dxrt::DxrtServiceErr::DxrtServiceErr(dxrt::IPCServerWrapper *ipcServerWrapper)
: _ipcServerWrapper(ipcServerWrapper)
{

}

void dxrt::DxrtServiceErr::ErrorReportToClient(dxrt_server_err_t err, long procId, uint32_t errCode, int deviceId)
{
    dxrt::IPCServerMessage serverMessage;

    serverMessage.code = dxrt::RESPONSE_CODE::ERROR_REPORT;
    serverMessage.data = static_cast<uint64_t>(err);
    serverMessage.result = errCode;
    serverMessage.msgType = procId;
    serverMessage.deviceId = deviceId;

    _ipcServerWrapper->SendToClient(serverMessage);
}
