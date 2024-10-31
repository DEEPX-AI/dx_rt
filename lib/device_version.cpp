// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/device_version.h"
#include "dxrt/driver.h"

using namespace std;

namespace dxrt {

DxDeviceVersion::DxDeviceVersion(Device *device, uint16_t fw_ver, int type, int interface_value, uint32_t variant)
{
    LOG_DXRT_DBG << "DeepX version Create " << endl;
    _dev       = device;
    _fw_ver    = fw_ver;
    _variant   = variant;
    _type      = static_cast<dxrt_device_type_t>(type);
    _interface = static_cast<dxrt_device_interface_t>(interface_value);
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
        DXRT_ASSERT(ret == 0, "failed to get RT driver info");
    }
    if((_interface == DEVICE_INTERFACE_ASIC) && (_type == DEVICE_TYPE_ACCELERATOR))
    {
        ret = _dev->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
            reinterpret_cast<void*>(&devInfo.rt_drv_ver),
            0,
            dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_RT_INFO);
        DXRT_ASSERT(ret == 0, "failed to get RT driver info");
        ret = _dev->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DRV_INFO,
            reinterpret_cast<void*>(&devInfo.pcie),
            0,
            dxrt::dxrt_drvinfo_sub_cmd_t::DRVINFO_CMD_GET_PCIE_INFO);
        DXRT_ASSERT(ret == 0, "failed to get PCIE driver info");
    }
    return devInfo;
}

void DxDeviceVersion::CheckVersion(void)
{
    LOG_DXRT_DBG << " ** DeepX version Check ** " << endl;
    {
        (void)GetVersion();
        // cout << "[ Fw Ver:" << _fw_ver << ", Rt Ver:"<< devInfo.rt_drv_ver <<", PCIe Ver:" << devInfo.pcie.driver_version << " ]" << endl;
        if (_interface == DEVICE_INTERFACE_FPGA)
        {
            DXRT_ASSERT(!(devInfo.rt_drv_ver < RT_DRV_VERSION_CHECK), 
                "The current device driver version " +  to_string(devInfo.rt_drv_ver) + " must be higher than recommended ( > " + to_string(RT_DRV_VERSION_CHECK) + " ).");
        }
        if((_interface == DEVICE_INTERFACE_ASIC) && (_type == DEVICE_TYPE_ACCELERATOR))
        {
            uint16_t fw_ver = 0;
            DXRT_ASSERT(!(devInfo.pcie.driver_version < PCIE_VERSION_CHECK),
                "The current PCIE driver version " +  to_string(devInfo.pcie.driver_version) + " must be higher than recommended ( > " + to_string(PCIE_VERSION_CHECK) + " ).");
            fw_ver = FW_VERSION_CHECK;
            DXRT_ASSERT(!(_fw_ver < fw_ver),
                "The current firmware version " +  to_string(_fw_ver) + " must be higher than recommended ( > " + to_string(fw_ver) + " )");
        }
    }
}


} // namespace dxrt
