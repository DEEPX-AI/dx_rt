// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "../include/dxrt/exception/server_err.h"


namespace dxrt{

std::ostream& operator<<(std::ostream& os, const dxrt_server_err_t& err) {
    switch (err) {
        case dxrt_server_err_t::S_ERR_SCHEDULE_REQ:
            os << "NPU Request Error";
            break;
        case dxrt_server_err_t::S_ERR_NEED_DEV_RECOVERY:
            os << "Device need to reset";
            break;
        case dxrt_server_err_t::S_ERR_DEVICE_RESPONSE_FAULT:
            os << "Device not response";
            break;
        case dxrt_server_err_t::S_ERR_SERVICE_TERMINATION:
            os << "Service terminated";
            break;
        default:
            os << "Unknown error";
    }
    return os;
}

}  // namespace dxrt
