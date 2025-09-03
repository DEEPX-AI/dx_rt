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

namespace dxrt {

DXRT_API std::string GetDrvVersionWithDot(uint32_t ver);
DXRT_API std::string GetFwVersionWithDot(uint32_t ver);

} // namespace dxrt