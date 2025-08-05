// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <cstdint>

namespace dxrt {

    class NpuDeviceFormatter
    {
    public:

        static std::string FormatDeviceType(const uint32_t type);
        static std::string FormatDeviceVariant(const uint32_t variant);
        static std::string FormatFirmwareVersion(const uint16_t fw_version);
        static std::string FormatDDRType(const uint16_t ddr_type);
        static std::string FormatRTDriverVersion(const uint32_t rt_driver_version);
        static std::string FormatPCIeDriverVersion(const uint32_t pcie_driver_version);
        static std::string FormatCapacity(uint64_t bytes);

    private:
        //no constructor
        NpuDeviceFormatter() = delete; 
    };

}