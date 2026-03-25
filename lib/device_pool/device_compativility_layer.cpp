/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/common.h"
#include "dxrt/device.h"
#include "dxrt/device_task_layer.h"
#include "dxrt/device_core.h"
#include "dxrt/service_abstract_layer.h"
#include "dxrt/device_pool.h"
#include "dxrt/cli_support.h"




namespace dxrt {



DeviceType Device::getDeviceType() const
{
    return GetCore()->GetDeviceType();
}


void Device::Reset(int opt) const
{
    DisplayCountdown(2, "Please wait until the device reset is complete.");
    DevicePool::GetInstance().GetServiceLayer()->SignalDeviceReset(_id);
    GetCore()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RESET, &opt, 4);
    LOG_DXRT << "Device reset is complete!" << std::endl;

}

Device::Device(int id) : _id(id)
{
    // Initialize device core and task layer here
}

std::shared_ptr<DeviceCore> Device::GetCore() const
{
    return DevicePool::GetInstance().GetDeviceCores(_id);
}

std::shared_ptr<DeviceTaskLayer> Device::GetTaskLayer() const
{
    return DevicePool::GetInstance().GetDeviceTaskLayer(_id);
}


static std::vector<std::shared_ptr<Device> > devices;  // NOSONAR
static void SetCheckDevices()
{
    DevicePool& pool = DevicePool::GetInstance();
    pool.InitCores();
    pool.InitTaskLayers();

    auto count = static_cast<int>(pool.GetDeviceCount());
    for (int i = 0; i < count; i++)
    {
        devices.push_back(std::make_shared<Device>(i));
    }
}


std::vector<std::shared_ptr<Device> >& CheckDevices()
{
    static std::once_flag flag;
    std::call_once(flag, SetCheckDevices);
    return devices;
}

DeviceStatus Device::GetCurrentStatus() const
{
    return DeviceStatus::GetCurrentStatus(GetCore());
}

void Device::DoCustomCommand(void *data, uint32_t subCmd, uint32_t size) const
{
    GetCore()->DoCustomCommand(data, subCmd, size);
}

}  // namespace dxrt
