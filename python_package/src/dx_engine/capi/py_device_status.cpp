#include <vector>
#include <string>
#include <iostream> 
#include <numeric>  
#include <stdexcept>

#include "dxrt/dxrt_cxx_api.h"

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

    double pyDeviceStatus_GetCoreUtilization(DeviceStatus &deviceStatus, int coreId)
    {
        return deviceStatus.GetCoreUtilization(coreId);
    }

    uint64_t pyDeviceStatus_GetMemoryUsed(DeviceStatus &deviceStatus)
    {
        return deviceStatus.GetMemoryUsed();
    }

    uint64_t pyDeviceStatus_GetMemoryFree(DeviceStatus &deviceStatus)
    {
        return deviceStatus.GetMemoryFree();
    }

    bool pyDeviceStatus_IsValid(DeviceStatus &deviceStatus)
    {
        return deviceStatus.IsValid();
    }

    std::string pyDeviceStatus_GetDriverVersion(DeviceStatus &deviceStatus)
    {
        return deviceStatus.DriverVersionStr();
    }

} // namespace dxrt
