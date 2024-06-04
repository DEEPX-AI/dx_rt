// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt {

class Device;
using DevicePtr = std::shared_ptr<Device>;

using DevicePtr = std::shared_ptr<Device>;
typedef struct device_status
{
    uint32_t voltage[4];
    uint32_t clock[4];
    uint32_t temperature[4];
    uint32_t dvfs_enable;
    uint32_t dvfs_maxfreq;
    uint32_t count[4];
    uint32_t boot_state;
} dxrt_device_status_t;

class DxrtDeviceInfoWithStatus
{
public:
    static DxrtDeviceInfoWithStatus getStatusInfo(DevicePtr device);

    int getId() const{return _id;}

    std::string deviceTypeStr() const; //ACC, STD
    std::string deviceTypeWord() const; //Accelator, Standalone
    std::string deviceVariantStr() const; //L1, L2, L3, M1, M1A...
    std::string boardTypeStr() const; //SOM, M.2, H1...
    std::string memoryTypeStr() const; //LPDDR4, LPDDR5...
    std::string interfaceTypeStr() const; //ASIC, FPGA
    int64_t memorySize() const{return _info.mem_size;}
    std::string memorySizeStrBinaryPrefix() const; // 1.98GiB
    std::string memorySizeStrWithComma() const; //2,130,706,432Byte
    std::string allMemoryInfoStr() const; //Type:LPDDR4, Addr:0xc01000000, size: 1.98GiB(2,130,706,432Byte), clock: 4200dMHz
    std::string dvfsStateInfoStr() const; //dvfs Enabled(Max Frequency:900)
    std::string npuStatusStr(int no) const; //NPU 0: voltage 825 mV, clock 800 MHz, temperature 46'C
    std::string fwVersionStr() const; //1.1.3

    std::ostream& infoToStream(std::ostream& os) const;
    std::ostream& statusToStream(std::ostream& os) const;
    std::string getInfoString() const;
    std::string getStatusString() const;
    
    const dxrt_device_status_t& status() const{return _status;}
    const dxrt_device_info_t& info() const{return _info;}

private:
    int _id;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;

    DxrtDeviceInfoWithStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status);

};
std::ostream& operator<<(std::ostream& os, const DxrtDeviceInfoWithStatus& d);

} //namespace dxrt