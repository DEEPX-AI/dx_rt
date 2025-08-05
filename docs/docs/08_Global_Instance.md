# Global Instance
## Configuration
The `Configuration` class is a central component for managing global application settings for the DXRT library. It provides a consistent and thread-safe point of access for querying and modifying configuration parameters.

This guide covers usage for both **C++** and **Python**. The class is designed as a **singleton**, meaning only one instance of the configuration manager exists. The Python class acts as a wrapper around the core C++ singleton, so all instances in C++ and Python share the same state.

**Key Features:**

  * **Singleton Pattern**: Guarantees a single, globally accessible configuration instance.
  * **Dynamic Configuration**: Allows enabling/disabling features and setting attributes at runtime.
  * **Version Reporting**: Provides methods to retrieve library and driver versions.
  * **Language Support**: Available in both C++ and Python.

-----

### Getting an Instance

How you get the configuration object differs slightly between C++ and Python.

***C++***

In C++, you must access the single instance through the static `GetInstance()` method. The constructor is private to enforce the singleton pattern.

```cpp
#include "dxrt/common.h"

// Correct: Get the single, global instance
dxrt::Configuration& config = dxrt::Configuration::GetInstance();

// Incorrect: The following line will cause a compile error
// dxrt::Configuration myConfig; // Error: constructor is private
```

***Python***

In Python, you create an instance using the standard constructor. Internally, this constructor retrieves the single, underlying C++ instance. All Python `Configuration` objects will therefore refer to the same settings.

```python
from dx_engine.configuration import Configuration

# Create a Configuration object.
# This holds a reference to the global settings instance.
config = Configuration()
```

-----

### Configuration Scopes: ITEM and ATTRIBUTE

Configuration is organized around two enumerations, `ITEM` and `ATTRIBUTE`, which are used in both C++ and Python.

#### ITEM

An `ITEM` represents a major feature or module that can be enabled or disabled.

| Item | Description |
| :--- | :--- |
| `DEBUG` | Enables general debug mode. |
| `PROFILER` | Enables profiler functionality. |
| `SERVICE` | Configures service-related operations. |
| `DYNAMIC_CPU_THREAD` | Manages dynamic CPU thread settings. |
| `TASK_FLOW` | Controls task flow management features. |
| `SHOW_THROTTLING` | Enables the display of throttling information. |
| `SHOW_PROFILE` | Enables the display of profile results. |
| `SHOW_MODEL_INFO` | Enables the display of detailed model information. |

#### ATTRIBUTE

An `ATTRIBUTE` represents a specific property of an `ITEM`, usually set with a string value like a file path.

| Attribute | Associated `ITEM` | Description |
| :--- | :--- | :--- |
| `PROFILER_SHOW_DATA` | `PROFILER` | Attribute for showing profiler data. |
| `PROFILER_SAVE_DATA` | `PROFILER` | Attribute for saving profiler data to a file. |

-----

### Key Operations and Usage

This section details the main operations with examples for both languages.

#### Enabling and Disabling Features

Use these methods to turn features on or off and check their current status.

***C++***

```cpp
// Enable the profiler
config.SetEnable(dxrt::Configuration::ITEM::PROFILER, true);

// Check if the profiler is enabled
if (config.GetEnable(dxrt::Configuration::ITEM::PROFILER)) {
    std::cout << "Profiler is enabled." << std::endl;
}
```

***Python***

```python
# Enable showing model information
config.set_enable(Configuration.ITEM.SHOW_MODEL_INFO, True)

# Check if showing model info is enabled
is_enabled = config.get_enable(Configuration.ITEM.SHOW_MODEL_INFO)
print(f"SHOW_MODEL_INFO is enabled: {is_enabled}")
```

#### Working with Attributes

For more fine-grained control, use attributes to set and get string-based values.

***C++***

```cpp
// First, ensure the parent item is enabled
config.SetEnable(dxrt::Configuration::ITEM::PROFILER, true);

// Set the path where profiler data should be saved
std::string profile_path = "/var/log/my_app_profile.json";
config.SetAttribute(dxrt::Configuration::ITEM::PROFILER,
                      dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA,
                      profile_path);

// Retrieve the attribute value later
std::string saved_path = config.GetAttribute(dxrt::Configuration::ITEM::PROFILER,
                                              dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA);
```

***Python***

```python
# First, ensure the parent item is enabled
config.set_enable(Configuration.ITEM.PROFILER, True)

# Set the path for saving profiler data
profile_log_path = "/var/log/dx_profile.json"
config.set_attribute(Configuration.ITEM.PROFILER,
                     Configuration.ATTRIBUTE.PROFILER_SAVE_DATA,
                     profile_log_path)

# Retrieve the path later
saved_path = config.get_attribute(Configuration.ITEM.PROFILER,
                                  Configuration.ATTRIBUTE.PROFILER_SAVE_DATA)
print(f"Profiler data will be saved to: {saved_path}")
```

#### Retrieving Version Information

These methods are critical for debugging, logging, and ensuring system compatibility.

***C++***

```cpp
#include <vector>
#include <utility>
#include <string>

try {
    std::cout << "DXRT Library Version: " << config.GetVersion() << std::endl;
    std::cout << "Driver Version: " << config.GetDriverVersion() << std::endl;

    // Get firmware versions for all detected devices
    std::vector<std::pair<int, std::string>> fw_versions = config.GetFirmwareVersions();
    for (const auto& fw : fw_versions) {
        std::cout << "Device " << fw.first << " Firmware Version: " << fw.second << std::endl;
    }
} catch (const std::runtime_error& e) {
    std::cerr << "Error retrieving version information: " << e.what() << std::endl;
}
```

***Python***

```python
print(f"Library Version: {config.get_version()}")
print(f"Driver Version: {config.get_driver_version()}")
print(f"PCIe Driver Version: {config.get_pcie_driver_version()}")
```

-----

## DeviceStatus

The `DeviceStatus` class is designed to provide a **snapshot** of a device's state. When you obtain a `DeviceStatus` object, it captures the device's properties (like model and memory) and real-time metrics (like temperature and clock speed) at that specific moment.

The general workflow is:

  - Use a **static/class method** to find the number of available devices.
  - Use another **static/class method** to get a status object for a specific device ID.
  - Use **instance methods** on that object to retrieve the data you need.

-----

### Getting Started: Accessing Devices

The first step is always to find out how many devices are available and then create a status object for the one you want to inspect.

#### Step 1: Get the Device Count

Use the static methods below to determine how many NPU devices are recognized by the system.

***C++***

```cpp
#include "dxrt/dxrt_api.h" // Main C++ header

int deviceCount = dxrt::DeviceStatus::GetDeviceCount();
std::cout << "Found " << deviceCount << " devices." << std::endl;
```

***Python***

```python
from dx_engine.dev_status import DeviceStatus # Main Python class

device_count = DeviceStatus.get_device_count()
print(f"Found {device_count} devices.")
```

#### Step 2: Get the Status Object

Once you have the count, you can get a status object for any valid device ID (from `0` to `device_count - 1`).

***C++***
It's crucial to use a `try...catch` block, as requesting an invalid ID will throw an exception.

```cpp
try {
    // Get a status snapshot for device with ID 0
    dxrt::DeviceStatus status = dxrt::DeviceStatus::GetCurrentStatus(0);
    std::cout << "Successfully created status object for device " << status.GetId() << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

***Python***
The factory method `get_current_status()` returns a `DeviceStatus` object.

```python
if device_count > 0:
    # Get the status object for the first device (ID 0)
    status_obj = DeviceStatus.get_current_status(0)
    print(f"Successfully created status object for device ID: {status_obj.get_id()}")
```

-----

### Querying Device Information

With a `DeviceStatus` object, you can access a wealth of information.

#### Formatted Summary Strings (C++ Only)

For quick logging or command-line display, the C++ class offers powerful helper methods that return a pre-formatted, human-readable string summary. These are equivalent to the `dxrt-cli` tool's output.

  - **`GetInfoString()`**: Returns static hardware info (model, memory, board, firmware).
  - **`GetStatusString()`**: Returns dynamic real-time status (NPU voltage, clock, temp, DVFS state).

<!-- end list -->

```cpp
// Print static hardware information
std::cout << "--- Device Info ---\n" << status.GetInfoString() << std::endl;

// Print dynamic, real-time status
std::cout << "--- Real-time Status ---\n" << status.GetStatusString() << std::endl;
```

#### Accessing Specific Attributes (C++ and Python)

For programmatic access, use the instance methods to get individual data points.

| Metric | C++ Method | Python Method | Return Value |
| :--- | :--- | :--- | :--- |
| **Device ID** | `GetId()` | `get_id()` | `int` |
| **Temperature** | `GetTemperature(ch)` | `get_temperature(ch)` | `int` (Celsius) |
| **NPU Voltage** | `GetNpuVoltage(ch)` | `get_npu_voltage(ch)` | `uint32_t` / `int` (mV) |
| **NPU Clock** | `GetNpuClock(ch)` | `get_npu_clock(ch)` | `uint32_t` / `int` (MHz) |

*Note: The C++ API provides a richer set of methods for querying static hardware details like memory, board type, and device variants.*

-----

### Complete Usage Examples 📋

Here is a complete example for each language, showing how to iterate through all devices and print their status.

#### C++ Example

This example uses the formatted string helpers for a concise report.

```cpp
#include <iostream>
#include "dxrt/dxrt_api.h" // DXRT API header file

/**
 * @brief Prints the detailed status for each NPU core of a specific device.
 * @param device_id The ID of the device to query.
 */
void print_detailed_device_status(int device_id) {
    try {
        // Get a snapshot of the current status for the specified device.
        dxrt::DeviceStatus status = dxrt::DeviceStatus::GetCurrentStatus(device_id);

        std::cout << "--- Device ID: " << device_id << " ---" << std::endl;

        // Assuming 2 NPU cores per device, like in the Python example.
        // In a real application, it's better to get the core count dynamically from the API.
        for (int core_ch = 0; core_ch < 2; ++core_ch) {
            // Individually query the temperature, voltage, and clock speed for each core.
            int temp = status.GetTemperature(core_ch);
            uint32_t voltage = status.GetNpuVoltage(core_ch);
            uint32_t clock = status.GetNpuClock(core_ch);

            // Print in the same format as the Python example.
            std::cout << "  Core " << core_ch
                      << ": Temp=" << temp << "'C"
                      << ", Voltage=" << voltage << "mV"
                      << ", Clock=" << clock << "MHz" << std::endl;
        }
        std::cout << std::endl; // Add a newline for readability

    } catch (const dxrt::Exception& e) {
        std::cerr << "Error getting report for device " << device_id << ": " << e.what() << std::endl;
    }
}

int main() {
    int deviceCount = dxrt::DeviceStatus::GetDeviceCount();
    if (deviceCount == 0) {
        std::cout << "No DEEPX devices found." << std::endl;
        return 1;
    }

    std::cout << "Querying status for " << deviceCount << " device(s)...\n" << std::endl;

    // Iterate through all devices and print their detailed status.
    for (int i = 0; i < deviceCount; ++i) {
        print_detailed_device_status(i);
    }

    return 0;
}
```

#### Python Example

This example iterates through each device and NPU core to print specific metrics.

```python
from dx_engine.dev_status import DeviceStatus

def main():
    """Checks for all available devices and prints their real-time status."""
    try:
        device_count = DeviceStatus.get_device_count()
        if device_count == 0:
            print("No devices found.")
            return

        print(f"Querying status for {device_count} device(s)...\n")
        # Iterate through each device by its ID
        for i in range(device_count):
            print(f"--- Device ID: {i} ---")
            status = DeviceStatus.get_current_status(i)

            # Assuming 2 NPU cores per device for this example
            for core_ch in range(2):
                temp = status.get_temperature(core_ch)
                voltage = status.get_npu_voltage(core_ch)
                clock = status.get_npu_clock(core_ch)
                print(f"  Core {core_ch}: Temp={temp}°C, Voltage={voltage}mV, Clock={clock}MHz")
            print("")

    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
```