// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "npu_device.h"
#include "ipc/dxtop_ipc_client.h"
namespace dxrt {

NpuDevice::NpuDevice(uint8_t deviceNumber, dxrt::DevicePtr devicePtr, DXTopIPCClient& dxtopIPCClient)
:_deviceNumber(deviceNumber), _devicePtr(devicePtr), _dramUsage(0)
{
    _coreCount = CORE_COUNT;
    _devicePtr = devicePtr;
    
    //Init Device
    _info = _devicePtr->info();

    //Init Core 
    _status = _devicePtr->status();

    for(uint8_t i = 0; i < _coreCount; ++i)
    {
        // float utilization = (float)(rand() % 100);
        uint32_t voltage = _status.voltage[i];
        uint32_t clock = _status.clock[i];
        uint32_t temperature = _status.temperature[i];
        
        auto core = std::make_shared<dxrt::NpuCore>(i, _deviceNumber);
        core->UpdateData(dxtopIPCClient, voltage, clock, temperature);
        _cores.push_back(core);
    }
}

void NpuDevice::UpdateDeviceInfoData()
{
    _info = _devicePtr->info();
    std::cout << "fw version = " << _info.fw_ver << std::endl;
}

void NpuDevice::UpdateCoreData(DXTopIPCClient& dxtopIPCClient)
{
    _status = _devicePtr->status();

    for(uint8_t core_num = 0; core_num < _coreCount; ++core_num)
    {
        // float utilization = (float)(rand() % 100);
        uint32_t voltage = _status.voltage[core_num];
        uint32_t clock = _status.clock[core_num];
        uint32_t temperature = _status.temperature[core_num];
        _cores[core_num]->UpdateData(dxtopIPCClient, voltage, clock, temperature);
    }
}

uint64_t NpuDevice::UpdateDramUsageByIPC(DXTopIPCClient& dxtopIPCClient)
{
    try
    {
        _dramUsage = dxtopIPCClient.SendRequest(
            dxrt::REQUEST_CODE::VIEW_USED_MEMORY,
            _deviceNumber,
            10
        );

        return _dramUsage;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[NpuDevice] IPC error while getting DRAM usage: " << e.what() << std::endl;
        return 0;
    }

}

dxrt_dev_info_t NpuDevice::GetDevInfo()
{
    return _devicePtr->devInfo();
}

uint8_t NpuDevice::GetDeviceNumber() const
{
    return _deviceNumber;
}

uint32_t NpuDevice::GetDeviceType() const
{
    return _info.type;
}

uint32_t NpuDevice::GetDeviceVariant() const
{
    
    return _info.variant;
}

uint16_t NpuDevice::GetDDRFrequency() const
{
    return _info.ddr_freq;
}

uint16_t NpuDevice::GetDDRType() const
{
    return _info.ddr_type;
}

uint64_t NpuDevice::GetTotalUsableMemory() const
{
    return _info.mem_size;
}

uint64_t NpuDevice::GetDramUsage() const
{
    return _dramUsage;
}

uint16_t NpuDevice::GetFirmwareVersion() const
{
    return _info.fw_ver;
}

uint16_t NpuDevice::GetBoardType() const
{
    return _info.bd_type;
}

const std::vector<std::shared_ptr<dxrt::NpuCore>>& NpuDevice::GetCores() const
{
    return _cores;
}

}