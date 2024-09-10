// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <memory>
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt {

class Device;
using DevicePtr = std::shared_ptr<Device>;

#ifdef __linux      // / TODO: Duplicate declaration (dxrt/device_struct.h)
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
#endif

class DXRT_API DxrtDeviceInfoWithStatus
{
 public:
    static DxrtDeviceInfoWithStatus getStatusInfo(DevicePtr device);

    /** @brief return device id
     *  @return devicd ID as int
     */    int getId() const{return _id;}

    /** @brief return device type as three leater
     *  @return string "ACC" or "STD"
     */
    std::string deviceTypeStr() const;
    /** @brief return device type as plain word
     *  @return string "Accelator" or "Standalone"
     */
    std::string deviceTypeWord() const;
    /** @brief return device chip variant type.
     *  @return string "L1", "L2", "L3", "M1"...
     */
    std::string deviceVariantStr() const;
    /** @brief return device board type.
     *  @return string "SOM", "M.2" or "H1"
     */
    std::string boardTypeStr() const;
    /** @brief return device memory type.
     *  @return string "LPDDR4", "LPDDR5"
     */
    std::string memoryTypeStr() const;
    /** @brief return device memory size as integer
     *  @return memory size as 64-bit integer
     */
    int64_t memorySize() const{return _info.mem_size;}
    /** @brief return device memory Frequency (MHz)
     *  @return memory frequency as MegaHertz
     */
    int memoryFrequency() const{return _info.ddr_freq;}
    /** @brief return device memory size as string using binary units
     *  @return memory size as string
     *  @return ex: "1.98GiB", "512Mib"
     */
    std::string memorySizeStrBinaryPrefix() const;
    /** @brief return device memory size as string, full integer 
     *  @return memory size as string
     *  @return ex: "2,130,706,432Byte", "536,870,912 Byte"
     */
    std::string memorySizeStrWithComma() const;
    /** @brief return device memory information as a line 
     *  @return memory size as string
     *  @return ex: "Memory: LPDDR4 4200 MHz, 1.98GiB"
     */
    std::string allMemoryInfoStr() const;
    /** @brief get device Dynamic Voltage Frequency State   
     *  @return dvfs Enabled(Max Frequency:900)
     *  @return dvfs Disabled
     *  
     */
    std::string dvfsStateInfoStr() const;
    /** @brief a NPU state information as a line 
     *  @return string contains voltage, clock, temp of device (ex: "NPU 0: voltage 825 mV, clock 800 MHz, temperature 46'C")
     *  
     */
    std::string npuStatusStr(int no) const;  // NPU 0: voltage 825 mV, clock 800 MHz, temperature 46'C
    /** @brief a NPU FW version
     *  @return version number as string(ex: 1.2.3)
     */
    std::string fwVersionStr() const;

    std::ostream& infoToStream(std::ostream& os) const;
    std::ostream& statusToStream(std::ostream& os) const;
    /** @brief get device info (dxrt-cli -i)
     * @return device info string in following format:
     * @verbatim
     Device 0: M1, Accelator type
     Memory: LPDDR4 4200 MHz, 1.98GiB
     Board: SOM ASIC, Rev 0.2
     2.0.3
     @endverbatim
     */
    std::string getInfoString() const;
    /** @brief get device status (dxrt-cli -s)
     * @return device info string in following format:
     * @verbatim
     NPU 0: voltage 825 mV, clock 1000 MHz, temperature 50'C
     NPU 1: voltage 800 mV, clock 600 MHz, temperature 52'C
     dvfs Disabled
     @endverbatim
     */
    std::string getStatusString() const;

    const dxrt_device_status_t& status() const{return _status;}
    const dxrt_device_info_t& info() const{return _info;}

 private:
    int _id;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;

    DxrtDeviceInfoWithStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status);
};
DXRT_API std::ostream& operator<<(std::ostream& os, const DxrtDeviceInfoWithStatus& d);

}  // namespace dxrt
