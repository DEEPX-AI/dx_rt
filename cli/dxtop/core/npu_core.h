/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <stdexcept>

#include "ipc/dxtop_ipc_client.h"

namespace dxrt {

    class NpuCore
    {
    public:
        explicit NpuCore(uint8_t coreNumber, uint8_t deviceNumber);
        virtual ~NpuCore() = default;

        void UpdateData(DXTopIPCClient& dxtopIPCClient, uint32_t voltage, uint32_t clock, uint32_t temperature);
        
        uint8_t GetCoreNumber() const;
        uint64_t GetUtilization() const;
        uint32_t GetVoltage() const;
        uint32_t GetClock() const;
        int32_t GetTemperature() const;
        
    private:
        uint8_t _coreNumber;
        uint8_t _deviceNumber;
        uint64_t _utilization;
        uint32_t _voltage;
        uint32_t _clock;
        int32_t _temperature;

    private:
        uint64_t updateUtilizationByIPC(DXTopIPCClient& dxtopIPCClient);
    };

}