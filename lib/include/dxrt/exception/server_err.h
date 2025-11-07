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

namespace dxrt {

enum class dxrt_server_err_t : long {
    S_ERR_NONE                  = 0,
    S_ERR_SCHEDULE_REQ          = 10,
    S_ERR_SERVICE_TERMINATION   = 100,
    S_ERR_SERVICE_DEV_BOUND_ERR = 200,
    S_ERR_NEED_DEV_RECOVERY     = 300,
    S_ERR_DEVICE_RESPONSE_FAULT = 400,
    S_ERR_DEVICE_EVENT_FAULT    = 500,
    S_ERR_SERVICE_UNKNOWN_ERR   = 999,
};
std::ostream& operator<<(std::ostream& os, const dxrt_server_err_t& err);


} //namespace dxrt
