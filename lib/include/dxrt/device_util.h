// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <iostream>

#include "dxrt/common.h"
#include "dxrt/datatype.h"

namespace dxrt {

DXRT_API std::string GetDrvVersionWithDot(uint32_t ver);
DXRT_API std::string GetFwVersionWithDot(uint32_t ver);

} // namespace dxrt