#include <vector>
#include <string>
#include <iostream> 
#include <numeric>  
#include <stdexcept>

#include "dxrt/dxrt_api.h" 
#include "dxrt/device_info_status.h"

namespace dxrt
{

    int pyDeviceStatus_GetTemperature(DeviceStatus &deviceStatus, int ch)
    {
        return deviceStatus.Temperature(ch);
    }

    int pyDeviceStatus_GetId(DeviceStatus &deviceStatus)
    {
        return deviceStatus.GetId();
    }

    int pyDeviceStatus_GetNpuVoltage(DeviceStatus &deviceStatus, int ch)
    {
        return deviceStatus.Voltage(ch);
    }

    int pyDeviceStatus_GetNpuClock(DeviceStatus &deviceStatus, int ch)
    {
        return deviceStatus.NpuClock(ch);
    }
    
} // namespace dxrt