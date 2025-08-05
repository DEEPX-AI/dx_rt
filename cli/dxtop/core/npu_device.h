// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "dxrt/device.h"
#include "dxrt/device_info_status.h"
#include "npu_core.h"
#include "ipc/dxtop_ipc_client.h"

namespace dxrt {

class NpuDevice
{
 public:
    static constexpr int CORE_COUNT = 3;

    explicit NpuDevice(uint8_t deviceNumber, dxrt::DevicePtr devicePtr, DXTopIPCClient& DXTopIPCClient);
    virtual ~NpuDevice() = default;

    void UpdateDeviceInfoData();
    void UpdateCoreData(DXTopIPCClient& dxtopIPCClient);
    uint64_t UpdateDramUsageByIPC(DXTopIPCClient& dxtopIPCClient);

    dxrt_dev_info_t GetDevInfo();
    uint8_t GetDeviceNumber() const;

    uint32_t  GetDeviceType() const;
    uint32_t GetDeviceVariant() const;
    uint16_t GetDDRFrequency() const;
    uint16_t GetDDRType() const;
    uint64_t GetTotalUsableMemory() const;

    uint16_t GetFirmwareVersion() const;
    // string GetMemoryInfo() const;
    uint16_t GetBoardType() const;

    uint64_t GetDramUsage() const;

    const std::vector<std::shared_ptr<dxrt::NpuCore>>& GetCores() const;


 private:
    uint8_t _deviceNumber;
    dxrt::DevicePtr _devicePtr;
    uint64_t _dramUsage;

    dxrt_device_status_t _status;
    dxrt_device_info_t _info;

    uint8_t _coreCount;
    std::vector<std::shared_ptr<dxrt::NpuCore>> _cores;

    // string _deviceType;
    // string _memoryInfo;
    // string _pcieInfo;

    // void Initialize();
};

}  // namespace dxrt
