// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <map>

#include "dxrt/common.h"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"

namespace dxrt {

enum class dxrt_server_err_t : long {
    S_ERR_NONE                  = 0,
    S_ERR_SCHEDULE_REQ          = 10,
    S_ERR_SERVICE_TERMINATION   = 100,
    S_ERR_SERVICE_DEV_BOUND_ERR = 200,
    S_ERR_NEED_DEV_RECOVERY     = 300,
    S_ERR_SERVICE_UNKNOWN_ERR   = 999,
};

class DxrtServiceErr
{
private:
    dxrt::IPCServerWrapper *_ipcServerWrapper;

public:
    DxrtServiceErr(dxrt::IPCServerWrapper *ipcServerWrapper);

    void ErrorReportToClient(dxrt_server_err_t err, long procId, uint32_t errCode);
};

inline std::ostream& operator<<(std::ostream& os, const dxrt_server_err_t& err) {
    switch (err) {
        case dxrt_server_err_t::S_ERR_SCHEDULE_REQ:
            os << "NPU Request Error";
            break;
        case dxrt_server_err_t::S_ERR_NEED_DEV_RECOVERY:
            os << "Device need to reset";
            break;
        default:
            os << "Unknown error";
    }
    return os;
}

} //namespace dxrt
