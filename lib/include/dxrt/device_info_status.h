/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <memory>
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt {

class Device;
using DevicePtr = std::shared_ptr<Device>;



/**
 * @brief A class that provides an abstraction of device information.
 * 
 * @details This class encapsulates various details about the device, including 
 * specifications, clock speeds, and temperature. It allows users to 
 * easily retrieve device status and characteristics.
 * 
 * Example usage:
 * @code
 * // Retrieve device status information
 * auto statInfo = GetCurrentStatus(DevicePtr device);
 * cout << GetInfoString() << endl << GetStatusString() << endl;
 * @endcode
 * 
 * @headerfile "dxrt/device_struct.h"
 */
class DXRT_API DeviceStatus
{
 public:
    static DeviceStatus GetCurrentStatus(DevicePtr& device);
    /**
     * @brief Retrieves the real-time status information for a specified device.
     * 
     * @details This function queries the system for the current operational status of a device 
     * identified by the given device ID. The status includes key metrics such as power state, 
     * temperature, clock speed, memory usage, and other device-specific parameters. 
     * 
     * If the provided device ID is invalid or does not correspond to an existing device, 
     * the function throws an exception to signal an error.
     * 
     * **Usage Example:**
     * @code
     * try {
     *     DeviceStatus status = DeviceManager::GetCurrentStatus(0);
     *     std::cout << "Device 0 Status: " << status.ToString() << std::endl;
     * } catch (const InvalidArgumentException& e) {
     *     LOG_DXRT_ERR("Error: " << e.what());
     * }
     * @endcode
     * 
     * @param id The unique identifier of the device whose status is being queried.
     * @return A `DeviceStatus` object containing various real-time operational parameters.
     * @throws InvalidArgumentException if the specified device ID is invalid or does not exist.
     */
    static DeviceStatus GetCurrentStatus(int id);

    /**
     * @brief Retrieves the total number of currently available devices.
     * 
     * @details This function returns the count of devices currently recognized and accessible 
     * by the system. This includes devices that are initialized and ready for operation.
     * 
     * **Usage Example:**
     * @code
     * int deviceCount = DeviceManager::GetDeviceCount();
     * std::cout << "Number of available devices: " << deviceCount << std::endl;
     * @endcode
     * 
     * @return An integer representing the total number of available devices.
     */
    static int GetDeviceCount();



    /**
     * @brief Retrieves the unique identifier of the device.
     * 
     * This function returns the device ID, which is a unique integer 
     * assigned to each device instance.
     * 
     * @return The device ID as an integer.
     */
    int GetId() const { return _id; }

    /**
     * @brief Retrieves the device type as a three-letter abbreviation.
     * 
     * This function returns a short string representation of the device type.
     * The possible return values are:
     * - "ACC" for Accelerator devices.
     * - "STD" for Standalone devices.
     * 
     * @return A string representing the device type abbreviation.
     */
     std::string DeviceTypeStr() const;

    /**
     * @brief Retrieves the full name of the device type.
     * 
     * This function provides a more descriptive string representation 
     * of the device type. The possible return values are:
     * - "Accelerator" for Accelerator devices.
     * - "Standalone" for Standalone devices.
     * 
     * @return A string representing the full name of the device type.
     */
    std::string DeviceTypeWord() const;
    /** @brief return device chip variant type.
     *  @return string "L1", "L2", "L3", "M1"...
     */
    std::string DeviceVariantStr() const;
    /** @brief return device board type.
     *  @return string "SOM", "M.2" or "H1"
     */
    std::string BoardTypeStr() const;
    /**
     * @brief Retrieves the type of memory used in the device.
     * 
     * @details This function returns a string indicating the type of memory technology 
     * used in the device.
     * @return A string representing the memory type (e.g., "LPDDR4" or "LPDDR5").
     */
    std::string MemoryTypeStr() const;

    /**
     * @brief Retrieves the total memory size available for the NPU.
     * 
     * This function returns the total amount of memory allocated for the Neural Processing Unit (NPU), 
     * measured in bytes. The memory size determines the capacity for storing models, intermediate 
     * computations, and input data.
     * 
     * @return The total memory size of the NPU in bytes.
     */
    int64_t MemorySize() const{ return _info.mem_size; }
    /** @brief return pcie infomation with speed, gen... as string
     *  @return pcie infomation with speed, gen... as string
     */
    std::string PcieInfoStr(int spd, int wd, int bus, int dev, int func) const;

    /**
     * @brief Retrieves the memory operating frequency of the device.
     * 
     * This function returns the clock speed of the device's memory in megahertz (MHz),
     * which determines the rate at which data can be read from or written to memory.
     * Higher frequencies typically result in better memory performance.
     * 
     * @return The memory frequency in MHz.
     */
    int MemoryFrequency() const { return _info.ddr_freq; }

    /**
     * @brief Retrieves the total memory size of the device as a string using binary units.
     * 
     * This function formats the memory size using binary prefixes (IEC standard), 
     * such as GiB (Gibibyte) or MiB (Mebibyte), where:
     * - 1 GiB = 1,073,741,824 bytes (1024^3)
     * - 1 MiB = 1,048,576 bytes (1024^2)
     * 
     * Example return values:
     * - "1.98 GiB" (for 2,130,706,432 bytes)
     * - "512 MiB" (for 536,870,912 bytes)
     * 
     * @return A string representing the memory size using binary units.
     */
    std::string MemorySizeStrBinaryPrefix() const;

    /**
     * @brief Retrieves the total memory size of the device as a string, formatted with commas.
     * 
     * This function returns the memory size as a full integer value in bytes, 
     * formatted with thousands separators for better readability. 
     * The result includes the "Byte" unit explicitly.
     * 
     * Example return values:
     * - "2,130,706,432 Byte" (for 2 GiB of memory)
     * - "536,870,912 Byte" (for 512 MiB of memory)
     * 
     * @return A string representing the memory size in bytes with comma separators.
     */
    std::string MemorySizeStrWithComma() const;

    /**
     * @brief Retrieves a summary of the device's memory specifications in a single line.
     * 
     * This function returns a formatted string that combines key memory details, including:
     * - Memory type (e.g., "LPDDR4", "LPDDR5")
     * - Operating frequency in MHz
     * - Total memory size with binary units
     * 
     * Example return values:
     * - "Memory: LPDDR4 4200 MHz, 1.98 GiB"
     * - "Memory: LPDDR5 5500 MHz, 4.00 GiB"
     * 
     * @return A formatted string containing memory type, frequency, and size.
     */
    std::string AllMemoryInfoStr() const;
    /**
     * @brief Retrieves the current Dynamic Voltage and Frequency Scaling (DVFS) state of the device.
     * 
     * @details This function provides information about whether DVFS is enabled or disabled. 
     * When enabled, it dynamically adjusts voltage and clock speed to optimize power consumption 
     * and performance.
     * 
     * The returned string follows one of the following formats:
     * - **"dvfs Enabled (Max Frequency: 900)"** -> DVFS is active, with a maximum clock frequency of 900 MHz.
     * - **"dvfs Disabled"** -> DVFS is turned off, meaning the voltage and clock speed remain fixed.
     * 
     * This function is useful for power management analysis and performance tuning.
     * 
     * @return A string indicating the DVFS state and maximum frequency (if enabled).
     */
    std::string DvfsStateInfoStr() const;

    /**
     * @brief Retrieves the status of a specific NPU (Neural Processing Unit) as a formatted string.
     * 
     * @details This function provides real-time operational parameters of the specified NPU, 
     * including voltage, clock speed, and temperature. 
     * 
     * Example output format:
     * @verbatim
     NPU 0: voltage 825 mV, clock 800 MHz, temperature 46'C
     @endverbatim
     * 
     * **Explanation of the format:**
     * - **NPU 0** -> Identifies the NPU index.
     * - **voltage 825 mV** -> Displays the operating voltage.
     * - **clock 800 MHz** -> Shows the current clock frequency.
     * - **temperature 46'C** -> Reports the real-time temperature.
     * 
     * @param ch The NPU index (starting from 0).
     * @return A formatted string containing the voltage, clock speed, and temperature of the selected NPU.
     */
    std::string NpuStatusStr(int ch) const;

    /**
     * @brief Retrieves the status of a specified LPDDR memory channel.
     * 
     * @details This function provides information about an LPDDR channel, including its configuration 
     * and real-time status. The information is based on memory register values (MR registers) 
     * that are relevant to LPDDR temperature and operation.
     * 
     * **Example Output:**
     * @verbatim
     LPDDR Channel 0: MR4=0x1A, Temperature Normal
     @endverbatim
     * 
     * **Explanation of the format:**
     * - **LPDDR Channel 0** -> Indicates the specific LPDDR memory channel (0, 1, 2, or 3).
     * - **MR4=0x1A** -> Shows the MR4 register value, which relates to temperature monitoring.
     * - **Temperature Normal** -> Provides an interpretation of the temperature status.
     * 
     * This function is useful for debugging memory performance and monitoring thermal conditions.
     * 
     * @param ch The LPDDR memory channel index (0 to 3).
     * @return A formatted string containing the status of the specified LPDDR memory channel.
     */
    std::string DdrStatusStr(int ch) const;

    /**
     * @brief Retrieves the count of lpddr Double-bit & Single-bit Error
     */
    std::string DdrBitErrStr(void) const;

    /**
     * @brief Retrieves the firmware version of the NPU.
     * 
     * @details This function returns the firmware version of the Neural Processing Unit (NPU) 
     * as a string, following the standard versioning format.
     * 
     * **Example Output:**
     * @verbatim
     1.2.3
     @endverbatim
     * 
     * This versioning format typically follows:
     * - **Major Version** -> Indicates major updates or breaking changes.
     * - **Minor Version** -> Represents feature enhancements.
     * - **Patch Version** -> Denotes bug fixes or minor improvements.
     * 
     * This function is useful for verifying firmware compatibility and ensuring the device is running 
     * the expected software version.
     * 
     * @return A string representing the firmware version (e.g., "1.2.3").
     */
    std::string FirmwareVersionStr() const;


    std::ostream& InfoToStream(std::ostream& os) const;
    std::ostream& StatusToStream(std::ostream& os) const;
    std::ostream& DebugStatusToStream(std::ostream& os) const;
    /**
     * @brief Retrieves detailed information about the device.
     * 
     * @details This function provides a formatted string containing key device specifications, 
     * including model name, memory type, board type, and firmware version. 
     * It is equivalent to running the `dxrt-cli -i` command.
     * 
     * The returned string follows this format:
     * @verbatim
     Device 0: M1, Accelerator type
     Memory: LPDDR4 4200 MHz, 1.98 GiB
     Board: SOM ASIC, Rev 0.2
     2.0.3
     @endverbatim
     * 
     * **Explanation of the format:**
     * - **Device 0: M1, Accelerator type** -> Displays the device index, model name (e.g., M1), and type (e.g., Accelerator).
     * - **Memory: LPDDR4 4200 MHz, 1.98 GiB** -> Provides memory type, frequency, and size.
     * - **Board: SOM ASIC, Rev 0.2** -> Indicates the board type (e.g., System-on-Module) and revision number.
     * - **2.0.3** -> Represents the firmware or software version of the device.
     * 
     * This function is useful for obtaining static device properties and verifying 
     * hardware configurations.
     * 
     * @return A formatted string containing device specifications.
     */
    std::string GetInfoString() const;

    /**
     * @brief Retrieves the real-time status of the device.
     * 
     * @details This function returns a formatted string with dynamic device status 
     * information, including voltage, clock speed, and temperature for each NPU 
     * (Neural Processing Unit), as well as the DVFS (Dynamic Voltage and Frequency Scaling) state. 
     * It is equivalent to running the `dxrt-cli -s` command.
     * 
     * The returned string follows this format:
     * @verbatim
     NPU 0: voltage 825 mV, clock 1000 MHz, temperature 50'C
     NPU 1: voltage 800 mV, clock 600 MHz, temperature 52'C
     dvfs Disabled
     @endverbatim
     * 
     * **Explanation of the format:**
     * - **NPU 0: voltage 825 mV, clock 1000 MHz, temperature 50'C** -> 
     *   Displays the voltage, clock frequency, and temperature of NPU 0.
     * - **NPU 1: voltage 800 mV, clock 600 MHz, temperature 52'C** -> 
     *   Displays the same status information for NPU 1.
     * - **dvfs Disabled** -> Indicates whether DVFS (Dynamic Voltage and Frequency Scaling) is enabled or disabled.
     * 
     * This function is useful for monitoring the device real-time operational status, 
     * diagnosing thermal or performance issues, and optimizing power management.
     * 
     * @return A formatted string containing real-time device status information.
     */
    std::string GetStatusString() const;


    const dxrt_device_status_t& Status() const{return _status;}
    const dxrt_device_info_t& Info() const{return _info;}

    /**
     * @brief Retrieves the voltage level of the specified NPU channel.
     * 
     * This function returns the operating voltage of the Neural Processing Unit (NPU) 
     * for a given channel, measured in millivolts (mV). 
     * The voltage level can vary depending on power management settings and workload.
     * 
     * @param ch The NPU channel index for which voltage is to be retrieved.
     * @return The voltage level of the specified NPU channel in millivolts (mV).
     */
    uint32_t Voltage(int ch) const;

    /**
     * @brief Retrieves the voltage level of the specified NPU channel.
     * 
     * This function returns the operating voltage of the Neural Processing Unit (NPU) 
     * for a given channel, measured in millivolts (mV). 
     * The voltage level can vary depending on power management settings and workload.
     * 
     * @param ch The NPU channel index for which voltage is to be retrieved.
     * @return The voltage level of the specified NPU channel in millivolts (mV).
     */
    uint32_t GetNpuVoltage(int ch) const
    {
      return Voltage(ch);
    }

    /**
     * @brief Retrieves the clock frequency of the specified NPU channel.
     * 
     * This function returns the current clock speed of the Neural Processing Unit (NPU) 
     * for a given channel, measured in megahertz (MHz). 
     * The clock frequency may change dynamically depending on performance scaling settings.
     * 
     * @param ch The NPU channel index for which clock speed is to be retrieved.
     * @return The clock frequency of the specified NPU channel in megahertz (MHz).
     */
    uint32_t NpuClock(int ch) const;


    /**
     * @brief Retrieves the clock frequency of the specified NPU channel.
     * 
     * This function returns the current clock speed of the Neural Processing Unit (NPU) 
     * for a given channel, measured in megahertz (MHz). 
     * The clock frequency may change dynamically depending on performance scaling settings.
     * 
     * @param ch The NPU channel index for which clock speed is to be retrieved.
     * @return The clock frequency of the specified NPU channel in megahertz (MHz).
     */
    uint32_t GetNpuClock(int ch) const
    {
         return NpuClock(ch);
    }

    /**
     * @brief Retrieves the temperature of the specified NPU channel.
     * 
     * This function returns the operating temperature of the Neural Processing Unit (NPU) 
     * for a given channel, measured in degrees Celsius. Monitoring the temperature is crucial 
     * for ensuring the NPU operates within safe thermal limits.
     * 
     * @param ch The NPU channel index for which temperature is to be retrieved.
     * @return The temperature of the specified NPU channel in degrees Celsius.
     */
    int Temperature(int ch) const;

   /**
     * @brief Retrieves the temperature of the specified NPU channel.
     * 
     * This function returns the operating temperature of the Neural Processing Unit (NPU) 
     * for a given channel, measured in degrees Celsius. Monitoring the temperature is crucial 
     * for ensuring the NPU operates within safe thermal limits.
     * 
     * @param ch The NPU channel index for which temperature is to be retrieved.
     * @return The temperature of the specified NPU channel in degrees Celsius.
     */
    int GetTemperature(int ch) const
    {
      return Temperature(ch);
    }

    /**
     * @brief Retrieves the number of DMA (Direct Memory Access) channels available for the NPU.
     * 
     * This function returns the total count of DMA channels that the NPU can use for transferring data 
     * between memory and processing units efficiently. A higher number of DMA channels allows better 
     * parallel data movement, improving performance.
     * 
     * @return The number of DMA channels available for the NPU.
     */
    uint64_t DmaChannel() const { return _info.num_dma_ch; }

    /**
     * @brief Retrieves the memory clock frequency of the NPU.
     * 
     * This function returns the clock speed of the memory used by the NPU, measured in megahertz (MHz). 
     * The memory clock speed affects data transfer rates and overall processing efficiency.
     * 
     * @return The memory clock frequency of the NPU in megahertz (MHz).
     */
    uint64_t MemoryClock() const { return _info.ddr_freq; }

    DeviceType GetDeviceType() const {return static_cast<DeviceType>(_info.type);}

 private:
    int _id;
    dxrt_device_info_t   _info;
    dxrt_device_status_t _status;
    dxrt_dev_info_t      _devInfo;

    DeviceStatus(int id, dxrt_device_info_t info, dxrt_device_status_t status, dxrt_dev_info_t devInfo);
};
DXRT_API std::ostream& operator<<(std::ostream& os, const DeviceStatus& d);

}  // namespace dxrt
