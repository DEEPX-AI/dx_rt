// Copyright (c) 2024 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/driver_adapter/driver_adapter.hpp"

namespace dxrt {

dxrt_device_status_t DriverAdapter::getDeviceStatus()
{
    dxrt_device_status_t status;
    IOControl(dxrt::dxrt_cmd_t::DXRT_CMD_GET_STATUS, &status, 0, 0);

    return status;
}

}  // namespace dxrt
