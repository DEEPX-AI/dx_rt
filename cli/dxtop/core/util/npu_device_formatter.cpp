/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "npu_device_formatter.h"

namespace dxrt {

std::string NpuDeviceFormatter::FormatDeviceType(const uint32_t type)
{
    switch (type)
    {
    case 0:
        return "ACC";
    case 1:
        return "STD";
    default:

        return "Invalid Type";
    }
}

std::string NpuDeviceFormatter::FormatDeviceVariant(const uint32_t variant)
{
    switch (variant)
    {
    case 100:
        return "L1";
    case 101:
        return "L2";
    case 102:
        return "L3";
    case 103:
        return "L4";
    case 200:
    case 201:
    case 202:
        return "M1";
    // case 200:
    //     return "M1";
    // case 201:
    //     return "M1A";
    // case 202:
    //     return "M1B";
    default:
        return "Invalid Variant";
    }
}

std::string NpuDeviceFormatter::FormatFirmwareVersion(const uint16_t fw_version)
{
    uint16_t major = fw_version / 100;
    uint16_t minor = (fw_version / 10) % 10;
    uint16_t patch = fw_version % 10;

    return  "v" +
            std::to_string(major) + "." +
            std::to_string(minor) + "." +
            std::to_string(patch);
}

std::string NpuDeviceFormatter::FormatDDRType(const uint16_t ddr_type)
{
    switch (ddr_type)
    {
    case 1:
        return "lpddr4";
    case 2:
        return "lpddr5";
    default:
        return "Invalid Type";
    }
}

std::string NpuDeviceFormatter::FormatRTDriverVersion(const uint32_t rt_driver_version)
{
    uint32_t major = rt_driver_version / 1000;
    uint32_t minor = (rt_driver_version / 100) % 10;
    uint32_t patch = rt_driver_version % 100;

    return  std::to_string(major) + "." +
            std::to_string(minor) + "." +
            std::to_string(patch);
}

std::string NpuDeviceFormatter::FormatPCIeDriverVersion(const uint32_t pcie_driver_version)
{
    uint32_t major = pcie_driver_version / 1000;
    uint32_t minor = (pcie_driver_version / 100) % 10;
    uint32_t patch = pcie_driver_version % 100;

    return  std::to_string(major) + "." +
            std::to_string(minor) + "." +
            std::to_string(patch);
}

std::string NpuDeviceFormatter::FormatCapacity(const uint64_t n)
{
    constexpr uint64_t kilo = 1024;
    constexpr uint64_t mega = kilo * kilo;
    constexpr uint64_t giga = mega * kilo;
    constexpr uint64_t tera = giga * kilo;

    double value = static_cast<double>(n);
    std::string postfix = " B";

    if (n >= tera)
    {
        value = static_cast<double>(n) / tera;
        postfix = " TiB";
    }
    else if (n >= giga)
    {
        value = static_cast<double>(n) / giga;
        postfix = " GiB";
    }
    else if (n >= mega)
    {
        value = static_cast<double>(n) / mega;
        postfix = " MiB";
    }
    else if (n >= kilo)
    {
        value = static_cast<double>(n) / kilo;
        postfix = " KiB";
    }

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.3g", value);

    return std::string(buffer) + postfix;
}

}
