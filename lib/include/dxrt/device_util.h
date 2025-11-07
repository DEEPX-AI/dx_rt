/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <iostream>

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver.h"

namespace dxrt {

DXRT_API std::string GetDrvVersionWithDot(uint32_t ver);
DXRT_API std::string GetFwVersionWithDot(uint32_t ver);
DXRT_API std::string GetDrvVersionFromRT(const dxrt_rt_drv_version_t& ver);
DXRT_API std::string GetFWVersionFromDeviceInfo(uint32_t ver, const char* suffix);

} // namespace dxrt