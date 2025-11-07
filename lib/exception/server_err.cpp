/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

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
        case dxrt_server_err_t::S_ERR_SERVICE_DEV_BOUND_ERR:
            os << "Service device bound error";
            break;
        case dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT:
            os << "Device event fault detected";
            break;
        default:
            os << "Unknown error";
    }
    return os;
}

}  // namespace dxrt
