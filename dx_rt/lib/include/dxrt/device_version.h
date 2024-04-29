// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>

#include "dxrt/common.h"
#include "dxrt/device.h"

#define RT_DRV_VERSION_CHECK (1000)
#define FW_VERSION_CHECK     (202)
#define PCIE_VERSION_CHECK   (2000)

namespace dxrt {

class Device;

struct deepx_pcie_info {
    uint32_t driver_version;
    uint8_t  bus;
    uint8_t  dev;
    uint8_t  func;
    int      speed; /* GEN1, GEN2...*/
    int      width; /* 1, 2, 4 */
};

typedef struct
{
    uint32_t        rt_drv_ver;
    deepx_pcie_info pcie;
} dxrt_dev_info_t;

class DxDeviceVersion
{
public:
    DxDeviceVersion(Device *device, uint16_t fw_ver, int type, int interface);
    void GetVersion(void);
    void CheckVersion(void);

protected:
    Device                 *_dev;
    dxrt_dev_info_t         devInfo;
    uint16_t                _fw_ver;
    dxrt_device_type_t      _type;
    dxrt_device_interface_t _interface;
};

} /* namespace dxrt */