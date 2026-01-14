/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "npu_core.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdint>
#include "util/unicode_literal_support.h"

namespace dxrt {

NpuCore::NpuCore(uint8_t coreNumber, uint8_t deviceNumber)
:_coreNumber(coreNumber), _deviceNumber(deviceNumber), _utilization(0), _voltage(0), _clock(0), _temperature(0)
{

}

void NpuCore::UpdateData(DXTopIPCClient& dxtopIPCClient, uint32_t voltage, uint32_t clock, uint32_t temperature)
{
    int32_t signed_temperature = static_cast<int32_t>(temperature);

    if(signed_temperature >= -40 && signed_temperature <= 125)
    {
        _temperature = signed_temperature;
    }
    else
    {
        throw std::out_of_range(convertLiteralUTF8(u8"Temperature value out of valid range ( -40 ~ 125\u00B0C)"));
    }

    _voltage = voltage;
    _clock = clock;

    // Update Utilization By IPC
    this->updateUtilizationByIPC(dxtopIPCClient);
}

uint64_t NpuCore::updateUtilizationByIPC(DXTopIPCClient& dxtopIPCClient)
{
    try
    {
        _utilization = dxtopIPCClient.SendRequest(
            dxrt::REQUEST_CODE::GET_USAGE,
            _deviceNumber,
            _coreNumber
        );

        return _utilization;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[NpuDevice] IPC error while getting Utilitation: " << e.what() << std::endl;
        return -1;
    }

}

uint8_t NpuCore::GetCoreNumber() const
{
    return _coreNumber;
}

uint64_t NpuCore::GetUtilization() const
{
    return _utilization;
}

uint32_t NpuCore::GetVoltage() const
{
    return _voltage;
}

uint32_t NpuCore::GetClock() const
{
    return _clock;
}

int32_t NpuCore::GetTemperature() const
{
    return _temperature;
}

}
