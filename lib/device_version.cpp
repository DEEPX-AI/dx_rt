/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 * 
 * This file uses ONNX Runtime (MIT License) - Copyright (c) Microsoft Corporation.
 */

#include "dxrt/common.h"
#include <cstring>
#include "dxrt/device_version.h"
#include "dxrt/driver.h"
#include "dxrt/exception/exception.h"
#include "resource/log_messages.h"

#ifdef USE_ORT
#include <onnxruntime_cxx_api.h>
#endif // USE_ORT

using namespace std;

namespace dxrt {

/**
 * Splits a version string by delimiter and returns vector of integers
 * @param version The version string to split (e.g., "1.2.3")
 * @param delimiter The character to split by (default: '.')
 * @return Vector of integers representing version components
 */
std::vector<int> ParseVersion(const std::string& version, char delimiter = '.') {
    std::vector<int> parts;
    std::stringstream ss(version);
    std::string part;
    
    while (std::getline(ss, part, delimiter)) {
        try {
            parts.push_back(std::stoi(part));
        } catch (const std::exception& e) {
            // If conversion fails, treat as 0
            parts.push_back(0);
        }
    }
    
    return parts;
}

/**
 * Compares two version strings to determine if current version is equal or higher than minimum version
 * @param currentVersion The current version string (e.g., "1.2.3")
 * @param minVersion The minimum required version string (e.g., "1.1.1")
 * @return true if currentVersion >= minVersion, false otherwise
 */
bool IsVersionEqualOrHigher(const std::string& currentVersion, const std::string& minVersion) {
    std::vector<int> current = ParseVersion(currentVersion);
    std::vector<int> minimum = ParseVersion(minVersion);
    
    // Make both vectors the same length by padding with zeros
    size_t maxLength = std::max(current.size(), minimum.size());
    current.resize(maxLength, 0);
    minimum.resize(maxLength, 0);
    
    // Compare each component from left to right
    for (size_t i = 0; i < maxLength; ++i) {
        if (current[i] > minimum[i]) {
            return true;  // Current version is higher
        } else if (current[i] < minimum[i]) {
            return false; // Current version is lower
        }
        // If equal, continue to next component
    }
    
    return true; // All components are equal
}

bool IsVersionHigher(const std::string& currentVersion, const std::string& minVersion) {
    std::vector<int> current = ParseVersion(currentVersion);
    std::vector<int> minimum = ParseVersion(minVersion);
    
    // Make both vectors the same length by padding with zeros
    size_t maxLength = std::max(current.size(), minimum.size());
    current.resize(maxLength, 0);
    minimum.resize(maxLength, 0);
    
    // Compare each component from left to right
    for (size_t i = 0; i < maxLength; ++i) {
        if (current[i] > minimum[i]) {
            return true;  // Current version is higher
        } else if (current[i] < minimum[i]) {
            return false; // Current version is lower
        }
        // If equal, continue to next component
    }
    
    return false; // All components are equal
}

DxDeviceVersion::DxDeviceVersion(Device *device, uint16_t fw_ver, int type, int interface_value, uint32_t variant)
{
    LOG_DXRT_DBG << "DeepX version Create " << std::endl;
    _dev       = device;
    _fw_ver    = fw_ver;
    _variant   = variant;
    _type      = static_cast<dxrt_device_type_t>(type);
    _interface = static_cast<dxrt_device_interface_t>(interface_value);
    memset(&devInfo, 0, sizeof(dxrt_dev_info_t));
}

dxrt_dev_info_t DxDeviceVersion::GetVersion(void)
{
    int ret;
    if (_interface == DEVICE_INTERFACE_FPGA)
    {
        ret = _dev->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
            reinterpret_cast<void*>(&devInfo.rt_drv_ver),
            0,
            dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_RT_INFO);
        // DXRT_ASSERT(ret == 0, "failed to get RT driver info");
        if ( ret != 0 ) throw InvalidOperationException(EXCEPTION_MESSAGE("failed to get RT driver info"));
    }
    if ((_interface == DEVICE_INTERFACE_ASIC) && (_type == DEVICE_TYPE_ACCELERATOR))
    {
        ret = _dev->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
            reinterpret_cast<void*>(&devInfo.rt_drv_ver),
            0,
            dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_RT_INFO);

        // DXRT_ASSERT(ret == 0, "failed to get RT driver info");
        if ( ret != 0 ) throw InvalidOperationException(EXCEPTION_MESSAGE("failed to get RT driver info"));

        ret = _dev->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
            reinterpret_cast<void*>(&devInfo.pcie),
            0,
            dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_PCIE_INFO);
        // DXRT_ASSERT(ret == 0, "failed to get PCIE driver info");
        if ( ret != 0 ) throw InvalidOperationException(EXCEPTION_MESSAGE("failed to get PCIE driver info"));
    }
    return devInfo;
}

void DxDeviceVersion::CheckVersion(void)
{
    LOG_DXRT_DBG << " ** DeepX version Check ** " << std::endl;
    {
        (void)GetVersion();

        if (_interface == DEVICE_INTERFACE_FPGA)
        {
            if ( devInfo.rt_drv_ver < RT_DRV_VERSION_CHECK )
            {
                throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::NotSupported_DeviceDriverVersion(devInfo.rt_drv_ver, RT_DRV_VERSION_CHECK)));
            }
        }

        if ((_interface == DEVICE_INTERFACE_ASIC) && (_type == DEVICE_TYPE_ACCELERATOR))
        {
            if ( devInfo.rt_drv_ver < RT_DRV_VERSION_CHECK )
            {
                throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::NotSupported_DeviceDriverVersion(devInfo.rt_drv_ver, RT_DRV_VERSION_CHECK)));
            }

            if ( devInfo.pcie.driver_version < PCIE_VERSION_CHECK )
            {
                throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::NotSupported_PCIEDriverVersion(devInfo.pcie.driver_version, PCIE_VERSION_CHECK)));
            }

            if ( _fw_ver < FW_VERSION_CHECK )
            {
                throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::NotSupported_FirmwareVersion(_fw_ver, FW_VERSION_CHECK)));
            }

            // USE_ORT=ON, ONNX version check
#ifdef USE_ORT
            std::string onnx_version = std::string(OrtGetApiBase()->GetVersionString());
            if ( !IsVersionEqualOrHigher(onnx_version, ONNX_RUNTIME_VERSION_CHECK) )
            {
                throw InvalidOperationException(EXCEPTION_MESSAGE(LogMessages::NotSupported_ONNXRuntimeVersion(onnx_version, ONNX_RUNTIME_VERSION_CHECK)));
            }

#endif // USE_ORT

        }
    }
}


} // namespace dxrt
