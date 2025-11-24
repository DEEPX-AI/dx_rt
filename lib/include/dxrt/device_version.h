/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <stdint.h>

#include "dxrt/common.h"
#include "dxrt/device_core.h"

#if _WIN32
#define RT_DRV_VERSION_CHECK (1800) // 1.3.1 --> 1.8.0 for windows signed driver
#else
#define RT_DRV_VERSION_CHECK (1800) // 1.7.1 --> 1.8.0
#endif
#define PCIE_VERSION_CHECK   (1501) // 1.4.1 --> 1.5.1
#define FW_VERSION_CHECK     (240)  // 2.1.0 --> 2.4.0

const std::string ONNX_RUNTIME_VERSION_CHECK {"1.20.1"};

namespace dxrt {

class Device;

class DXRT_API DxDeviceVersion
{
public:
    DxDeviceVersion(DeviceCore* device, uint16_t fw_ver, int type, int interface_value, uint32_t variant);
    dxrt_dev_info_t GetVersion(void);
    void CheckVersion(void);

protected:
    DeviceCore* _dev;
    dxrt_dev_info_t         devInfo;
    uint16_t                _fw_ver;
    dxrt_device_type_t      _type;
    dxrt_device_interface_t _interface;
    uint32_t                _variant; /* 100: L1, 101: L2, 102: L3, 103: L4, 104: V3,
                                         200: M1, 202: M1A */
};


bool IsVersionEqualOrHigher(const std::string& currentVersion, const std::string& minVersion);
bool IsVersionHigher(const std::string& currentVersion, const std::string& minVersion);

} /* namespace dxrt */
