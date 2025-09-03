/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/device_util.h"
#include <string>

using std::to_string;

namespace dxrt
{

std::string GetDrvVersionWithDot(uint32_t ver)
{
    uint32_t major, minor, patch;
    major = ver / 1000;
    minor = (ver % 1000) / 100;
    patch = ver % 100;
    return to_string(major) + "." + to_string(minor) + "." + to_string(patch);
}

std::string GetFwVersionWithDot(uint32_t ver)
{
    uint32_t major, minor, patch;
    major = ver / 100;
    minor = (ver % 100) / 10;
    patch = ver % 10;
    return to_string(major) + "." + to_string(minor) + "." + to_string(patch);
}

} // namespace dxrt