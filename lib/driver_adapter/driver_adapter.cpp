/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/driver_adapter/driver_adapter.h"

namespace dxrt {

dxrt_device_status_t DriverAdapter::getDeviceStatus()
{
    dxrt_device_status_t status;
    IOControl(dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS, &status, 0, 0);

    return status;
}

DeviceType DriverAdapter::getDeviceType()
{
    dxrt_device_info_t type;
    IOControl(dxrt::dxrt_cmd_t::DXRT_CMD_IDENTIFY_DEVICE, &type, 0, 0);

    return static_cast<DeviceType>(type.type);
}

}  // namespace dxrt
