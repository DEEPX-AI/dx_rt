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
    std::string pcieInfoStr(int spd, int wd, int bus, int dev, int func) const;
    /** @brief return pcie infomation with speed, gen... as string
     *  @return pcie infomation with speed, gen... as string
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
    std::string ddrStatusStr(int ch) const;
    /**
     * @brief LPDDR ch[0, 1, 2, 3] status as a line
     * @return string contains lpddr channel status (mr register relevant to lpddr temperature)
     */
    std::string npuStatusStr(int no) const;  // NPU 0: voltage 825 mV, clock 800 MHz, temperature 46'C
    /** @brief a NPU FW version
     *  @return version number as string(ex: 1.2.3)
     */
    std::string fwVersionStr() const;

    std::ostream& infoToStream(std::ostream& os) const;
    std::ostream& statusToStream(std::ostream& os) const;
    std::ostream& debugStatusToStream(std::ostream& os) const;
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

    /** @brief return NPU voltage for given NPU
     *  @return voltage as mV value
     */
    uint32_t voltage(int ch) const;
    /** @brief return NPU clock for given NPU
     *  @return clock as MHz value
     */
    uint32_t clock(int ch) const;
    /** @brief return NPU temperature for given NPU
     *  @return temperature as celcious value
     */
    int temperature(int ch) const;
    /** @brief return NPU memory size
     *  @return memory size as byte
     */
    uint64_t memory_size() const { return _info.mem_size; }
    /** @brief return NPU dma channel count 
     *  @return NPU dma channel count
     */
    uint64_t dma_channel() const { return _info.num_dma_ch;  }
    /** @brief return NPU memory clock
     *  @return memory clock as MHz
     */
    uint64_t memory_clock() const { return _info.ddr_freq; }

 private:
    int _id;
    dxrt_device_info_t   _info;
    dxrt_device_status_t _status;
    dxrt_dev_info_t      _devInfo;

    DxrtDeviceInfoWithStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status, dxrt_dev_info_t devInfo);
};
DXRT_API std::ostream& operator<<(std::ostream& os, const DxrtDeviceInfoWithStatus& d);

}  // namespace dxrt
