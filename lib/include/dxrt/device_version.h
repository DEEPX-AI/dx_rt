// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>

#include "dxrt/common.h"
#include "dxrt/device.h"

#define RT_DRV_VERSION_CHECK (1300)
#define FW_VERSION_CHECK     (159)
#define PCIE_VERSION_CHECK   (1200)

namespace dxrt {

class Device;

class DXRT_API DxDeviceVersion
{
public:
    DxDeviceVersion(Device *device, uint16_t fw_ver, int type, int interface_value, uint32_t variant);
    dxrt_dev_info_t GetVersion(void);
    void CheckVersion(void);

protected:
    Device                 *_dev;
    dxrt_dev_info_t         devInfo;
    uint16_t                _fw_ver;
    dxrt_device_type_t      _type;
    dxrt_device_interface_t _interface;
    uint32_t                _variant; /* 100: L1, 101: L2, 102: L3, 103: L4,
                                         200: M1, 202: M1A */
};

} /* namespace dxrt */