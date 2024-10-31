// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <map>

#include "dxrt/common.h"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"

namespace dxrt {

enum class dxrt_server_err_t : long {
    S_ERR_NONE = 0,
    S_ERR_SCHEDULE_REQ = 10,
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
        default:
            os << "Unknown error";
    }
    return os;
}

} //namespace dxrt
