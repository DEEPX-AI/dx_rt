This chapter introduces global utility classes provided by the **DX-RT** SDK for managing configuration settings and querying device status. These classes are implemented as singletons to ensure consistent state across C++ and Python, enabling centralized control over runtime behavior and hardware monitoring.

---

## Configuration Management

The `Configuration` class serves as the centralized interface for managing global runtime settings in the **DX-RT** library. Designed as a thread-safe singleton, it ensures consistent configuration across both C++ and Python environments. In Python, this class wraps the underlying C++ singleton, maintaining a shared state between the two languages.  

**Key Features:**

  * **Singleton Design**: Guarantees a single, globally accessible configuration instance.
  * **Runtime Configurability**:  Supports dynamic enabling/disabling of features and real-time attribute updates.
  * **Version Access**:  Provides functions to retrieve library, driver, and device version information.
  * **Cross-Language Support**: Fully accessible from both C++ and Python with identical behavior.

---

### Obtaining the Configuration Instance

The method of accessing the global Configuration object differs slightly between C++ and Python, but both ensure interaction with the same underlying singleton.  

**C++**
In C++, the configuration instance **must** be retrieved using the static method `GetInstance()`. The constructor is private to enforce the singleton pattern.  

```cpp
#include "dxrt/common.h"

// Correct: Get the single, global instance
dxrt::Configuration& config = dxrt::Configuration::GetInstance();

// Incorrect: The following line will cause a compile error
// dxrt::Configuration myConfig; // Error: constructor is private
```

**Python**
In Python, the Configuration class can be instantiated directly. Internally, this constructor accesses the shared C++ singleton, ensuring all instances reflect the same state.  

```python
from dx_engine.configuration import Configuration

# Create a Configuration object.
# This holds a reference to the global settings instance.
config = Configuration()
```

Regardless of language, all operations performed on the Configuration instance affect the global runtime state.  

---

### Configuration Scopes: `ITEM` and `ATTRIBUTE`

The Configuration interface organizes runtime settings using two scoped enumerations: `ITEM` and `ATTRIBUTE`. These are supported consistently in both C++ and Python.  

***ITEM***  
An `ITEM` represents a high-level feature or module within the **DX-RT** that can be enabled or disabled. Common examples include runtime profiling, logging, or device tracing.

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
| `CUSTOM_INTRA_OP_THREADS` | Enables custom ONNX Runtime intra-operator thread count configuration. |
| `CUSTOM_INTER_OP_THREADS` | Enables custom ONNX Runtime inter-operator thread count configuration. |


***ATTRIBUTE***  
An `ATTRIBUTE` defines a property associated with a specific ITEM. It is typically used to set or retrieve string-based values such as file paths, flags, or operational modes.   

| Attribute | Associated `ITEM` | Description |
| :--- | :--- | :--- |
| `PROFILER_SHOW_DATA` | `PROFILER` | Attribute for showing profiler data. |
| `PROFILER_SAVE_DATA` | `PROFILER` | Attribute for saving profiler data to a file. |
| `CUSTOM_INTRA_OP_THREADS_NUM` | `CUSTOM_INTRA_OP_THREADS` | Number of threads for ONNX Runtime intra-operator parallelism (integer string, 1-hardware_concurrency). |
| `CUSTOM_INTER_OP_THREADS_NUM` | `CUSTOM_INTER_OP_THREADS` | Number of threads for ONNX Runtime inter-operator parallelism (integer string, 1-hardware_concurrency). |

---

### Core Operations and  Examples

This section outlines the primary operations supported by the Configuration class, with usage examples for both C++ and Python.

#### Enabling and Disabling Features

Enable or disable specific runtime modules using the `ITEM` enumeration. This allows dynamic control over major DXRT features at runtime.

**C++**
```cpp
// Enable the profiler
config.SetEnable(dxrt::Configuration::ITEM::PROFILER, true);

// Check if the profiler is enabled
if (config.GetEnable(dxrt::Configuration::ITEM::PROFILER)) {
    std::cout << "Profiler is enabled." << std::endl;
}
```

**Python**
```python
# Enable showing model information
config.set_enable(Configuration.ITEM.SHOW_MODEL_INFO, True)

# Check if showing model info is enabled
is_enabled = config.get_enable(Configuration.ITEM.SHOW_MODEL_INFO)
print(f"SHOW_MODEL_INFO is enabled: {is_enabled}")
```

#### Working with Attributes

Configure detailed runtime behavior by setting or retrieving string-based values using the ATTRIBUTE enumeration. `Attributes` are typically tied to a specific `ITEM`.  

**C++**
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

**Python**
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

Query the current DXRT library and driver versions. These functions are essential for debugging, compatibility checks, and system diagnostics.  

**C++**
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

**Python**
```python
print(f"Library Version: {config.get_version()}")
print(f"Driver Version: {config.get_driver_version()}")
print(f"PCIe Driver Version: {config.get_pcie_driver_version()}")
```

---

### Loading Configuration from File

The Configuration class supports loading settings from external configuration files using the `LoadConfigFile()` method. This allows you to manage runtime settings through configuration files rather than hardcoding them in your application.

#### Configuration File Format

Configuration files use a simple key-value format with `KEY=VALUE` pairs. Here's an example configuration file (`common.cfg`):

```properties
# General debug and profiling settings
ENABLE_DEBUG=0
USE_PROFILER=1
ENABLE_SHOW_PROFILER_DATA=1
ENABLE_SAVE_PROFILER_DATA=1

# ONNX Runtime thread settings (Opt-in Example)
# Note: Default common.cfg sets these to 0 (disabled)
USE_CUSTOM_INTRA_OP_THREADS=1
USE_CUSTOM_INTER_OP_THREADS=1
CUSTOM_INTRA_OP_THREADS_COUNT=4
CUSTOM_INTER_OP_THREADS_COUNT=2
```

!!! warning "IMPORTANT"  
    The ONNX Runtime thread settings shown above are an **opt-in example**. The actual default `common.cfg` file sets `USE_CUSTOM_INTRA_OP_THREADS=0` and `USE_CUSTOM_INTER_OP_THREADS=0`, which means ONNX Runtime will use its automatic thread management by default.

#### Configuration Parameters

**General Settings:**

* **`ENABLE_DEBUG`**: Enable/disable debug mode (0=disabled, 1=enabled)
* **`USE_PROFILER`**: Enable/disable profiler functionality (0=disabled, 1=enabled)
* **`ENABLE_SHOW_PROFILER_DATA`**: Enable/disable showing profiler data in console (0=disabled, 1=enabled)
* **`ENABLE_SAVE_PROFILER_DATA`**: Enable/disable saving profiler data to file (0=disabled, 1=enabled)

**Thread Configuration Parameters:**

The following parameters control ONNX Runtime thread behavior:

* **`USE_CUSTOM_INTRA_OP_THREADS`**: Enable/disable custom intra-operator thread count (0=disabled, 1=enabled)
* **`USE_CUSTOM_INTER_OP_THREADS`**: Enable/disable custom inter-operator thread count (0=disabled, 1=enabled)  
* **`CUSTOM_INTRA_OP_THREADS_COUNT`**: Number of threads for intra-operator parallelism (integer string, range: 1 to `hardware_concurrency()`)
* **`CUSTOM_INTER_OP_THREADS_COUNT`**: Number of threads for inter-operator parallelism (integer string, range: 1 to `hardware_concurrency()`)

**Thread Count Validation:**

* Values are automatically clamped to the range [1, `std::thread::hardware_concurrency()`]
* Invalid values (non-numeric strings) default to 1
* Empty values default to 1
* Out-of-range values are clamped with debug logging

**ONNX Runtime Behavior:**

* **Default behavior** (when `USE_CUSTOM_INTRA_OP_THREADS=0`): ONNX Runtime uses automatic thread count (typically equals hardware concurrency)
* **Default behavior** (when `USE_CUSTOM_INTER_OP_THREADS=0`): Uses sequential execution mode with 1 thread
* **Custom behavior** (when enabled=1): Uses the specified `CUSTOM_*_THREADS_COUNT` values with validation and clamping

#### Loading Configuration Files

**C++**
```cpp
#include "dxrt/common.h"

// Load configuration from file
dxrt::Configuration& config = dxrt::Configuration::GetInstance();
config.LoadConfigFile("path/to/common.cfg");

// Configuration settings are now applied globally
```

**Python**
```python
from dx_engine.configuration import Configuration

# Load configuration from file  
config = Configuration()
config.load_config_file("path/to/common.cfg")

# Configuration settings are now applied globally
```
---

## Device Status Monitoring

The `DeviceStatus` class provides a real-time snapshot of the NPU device's state, including static properties (e.g., model name, memory) and dynamic metrics (e.g., temperature, clock speed). Each instance represents the status of a specific device at the time it was queried.  

**Workflow Overview:**  

  * Retrieve the total number of available devices.
  * Access a specific device's status using its device ID.
  * Query hardware information and real-time metrics via instance methods.

### Getting Started: Accessing Devices

The first step is always to find out how many devices are available and then create a status object for the one you want to inspect. To monitor a device's status, begin by checking how many NPU devices are available, then retrieve the status object for the desired device.  

#### Step 1: Get the Device Count

Use the static method to determine how many devices are currently recognized by the system.  

**C++**
```cpp
#include "dxrt/dxrt_api.h" // Main C++ header

int deviceCount = dxrt::DeviceStatus::GetDeviceCount();
std::cout << "Found " << deviceCount << " devices." << std::endl;
```

**Python**
```python
from dx_engine.dev_status import DeviceStatus # Main Python class

device_count = DeviceStatus.get_device_count()
print(f"Found {device_count} devices.")
```

#### Step 2: Retrieve the Device Status

Once the count is known, access the status object using a valid device ID (`0` to `device_count - 1`).  

**C++**  
Use a `try...catch` block to handle invalid IDs safely:

```cpp
try {
    // Get a status snapshot for device with ID 0
    dxrt::DeviceStatus status = dxrt::DeviceStatus::GetCurrentStatus(0);
    std::cout << "Successfully created status object for device " << status.GetId() << std::endl;
} catch (const dxrt::Exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

**Python**  
Use the factory method `get_current_status()` to get a `DeviceStatus` object:  

```python
    # Get the status object for the first device (ID 0)
    status_obj = DeviceStatus.get_current_status(0)
    print(f"Successfully created status object for device ID: {status_obj.get_id()}")
```

---

### Querying Device Properties and Metrics

Once you obtain a `DeviceStatus` object, you can retrieve both static hardware properties and real-time operational metrics of the NPU device.  

#### Formatted Summary Strings (C++ Only)

For quick diagnostic logging or CLI-style output, the C++ API provides helper methods that return structured, human-readable summaries:  

  * **`GetInfoString()`**: Returns static hardware info (model, memory, board, firmware).  
  * **`GetStatusString()`**: Returns dynamic real-time status (NPU voltage, clock, temp, DVFS state).  

```cpp
// Print static hardware information
std::cout << "--- Device Info ---\n" << status.GetInfoString() << std::endl;

// Print dynamic, real-time status
std::cout << "--- Real-time Status ---\n" << status.GetStatusString() << std::endl;
```

#### Accessing Specific Attributes (C++ and Python)

For programmatic use, both C++ and Python interfaces offer methods to retrieve specific values from the status object:  

| Metric | C++ Method | Python Method | Return Value |
| :--- | :--- | :--- | :--- |
| **Device ID** | `GetId()` | `get_id()` | `int` |
| **Temperature** | `GetTemperature(ch)` | `get_temperature(ch)` | `int` (Celsius) |
| **NPU Voltage** | `GetNpuVoltage(ch)` | `get_npu_voltage(ch)` | `uint32_t` / `int` (mV) |
| **NPU Clock** | `GetNpuClock(ch)` | `get_npu_clock(ch)` | `uint32_t` / `int` (MHz) |

!!! note "NOTE"
     The C++ API provides a richer set of methods for querying static hardware details like memory, board type, and device variants.

---

### Complete Usage Examples 

This section demonstrates how to iterate through all available NPU devices and retrieve their status information using both C++ and Python.  

**C++**  
The following example uses `GetDeviceCount()` and `GetStatusString()` to print a summary for each device:  

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

**Python**  
In Python, use `DeviceStatus.get_device_count()` and `DeviceStatus.get_current_status()` to inspect device metrics:  

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

---
