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
#include <cstring>

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

static std::string AddSuffix(const std::string& str, const char* suffix)
{
    std::string suffix_str = suffix ? std::string(suffix) : "";
    std::string retval = str;
    if (!suffix_str.empty())
    {
        bool addSuffix = true;

        if (suffix_str == "\"\"" )
        {
            addSuffix = false;
        }
        if (addSuffix)
        {
            retval += "-";
            retval += std::string(suffix);
        }
    }
    return retval;
}

std::string GetDrvVersionFromRT(const dxrt_rt_drv_version_t& ver)
{
    return AddSuffix(GetDrvVersionWithDot(ver.driver_version), ver.driver_version_suffix);
}

std::string GetFWVersionFromDeviceInfo(uint32_t ver, const char* suffix)
{
    return AddSuffix(GetFwVersionWithDot(ver), suffix);
}

} // namespace dxrt
