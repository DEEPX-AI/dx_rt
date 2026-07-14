This chapter introduces global utility classes provided by the **DX-RT** SDK for managing configuration settings and querying device status. These classes are implemented as singletons to ensure consistent state across C++ and Python, enabling centralized control over runtime behavior and hardware monitoring.

---

## Configuration Management

The `Configuration` class serves as the centralized interface for managing global runtime settings in the **DX-RT** library. Designed as a thread-safe singleton, it ensures consistent configuration across both C++ and Python environments. In Python, this class wraps the underlying C++ singleton, maintaining a shared state between the two languages.  

**Key Features:**

  * **Singleton Design**: Guarantees a single, globally accessible configuration instance.  
  * **Runtime Configurability**: Supports dynamic enabling/disabling of features and real-time attribute updates.  
  * **Version Access**: Provides functions to retrieve library, driver, and device version information.  
  * **Cross-Language Support**: Fully accessible from both C++ and Python with identical behavior.  

---

### Obtaining the Configuration Instance

The method of accessing the global Configuration object differs slightly between C++ and Python, but both ensure interaction with the same underlying singleton.  

**C++**
In C++, the global configuration instance is accessed via the static method `GetInstance()`. Always use `GetInstance()` to obtain the shared singleton — constructing a local `Configuration` object would create a separate instance that does not reflect the global runtime state.  

```cpp
#include "dxrt/common.h"

// Recommended: Get the shared, global instance
dxrt::Configuration& config = dxrt::Configuration::GetInstance();
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

### Core Operations and Examples

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

The `DeviceStatus` class provides a real-time snapshot of the NPU device's state, including static properties (e.g., memory) and dynamic metrics (e.g., temperature, clock speed). Each instance represents the status of a specific device at the time it was queried.  

**Workflow Overview:**  

  * Retrieve the total number of available devices.
  * Access a specific device's status using its device ID.
  * Query hardware information and real-time metrics via instance methods.

### Getting Started: Accessing Devices

The first step is to find out how many devices are available, then create a status object for the one you want to inspect.  

#### Step 1: Get the Device Count

Use the static method to determine how many devices are currently recognized by the system.  

**C++**
```cpp
#include <dxrt/dxrt_cxx_api.h>

int deviceCount = dxrt::DeviceStatus::GetDeviceCount();
std::cout << "Found " << deviceCount << " devices." << std::endl;
```

**Python**
```python
from dx_engine.device_status import DeviceStatus # Main Python class

device_count = DeviceStatus.get_device_count()
print(f"Found {device_count} devices.")
```

#### Step 2: Retrieve the Device Status

Once the count is known, access the status object using a valid device ID (`0` to `device_count - 1`).  

**C++**  
Use `IsValid()` to verify that the device data was successfully retrieved:

```cpp
// Get a status snapshot for device with ID 0
dxrt::DeviceStatus status = dxrt::DeviceStatus::GetCurrentStatus(0);
if (status.IsValid())
{
    std::cout << "Successfully created status object for device " << status.GetId() << std::endl;
}
else
{
    std::cerr << "Warning: Device 0 data is not available." << std::endl;
}
```

**Python**  
Use the factory method `get_current_status()` to get a `DeviceStatus` object:  

```python
# Get the status object for the first device (ID 0)
status_obj = DeviceStatus.get_current_status(0)
if status_obj.is_valid():
    print(f"Successfully created status object for device ID: {status_obj.get_id()}")
else:
    print("Warning: Device 0 data is not available.")
```

---

### Querying Device Properties and Metrics

Once you obtain a `DeviceStatus` object, you can retrieve both static hardware properties and real-time operational metrics of the NPU device.  

#### Accessing Specific Attributes (C++ and Python)

For programmatic use, both C++ and Python interfaces offer methods to retrieve specific values from the status object:  

**Hardware Status (C++ and Python)**

| Metric | C++ Method | Python Method | Return Value |
| :--- | :--- | :--- | :--- |
| **Device ID** | `GetId()` | `get_id()` | `int` |
| **Temperature** | `GetTemperature(ch)` | `get_temperature(ch)` | `int` (Celsius) |
| **NPU Voltage** | `GetNpuVoltage(ch)` | `get_npu_voltage(ch)` | `uint32_t` / `int` (mV) |
| **NPU Clock** | `GetNpuClock(ch)` | `get_npu_clock(ch)` | `uint32_t` / `int` (MHz) |
| **Core Utilization** | `GetCoreUtilization(coreId)` | `get_core_utilization(core_id)` | `double` / `float` (0.0~100.0 %) |
| **Memory Used** | `GetMemoryUsed()` | `get_memory_used()` | `uint64_t` / `int` (bytes) |
| **Memory Free** | `GetMemoryFree()` | `get_memory_free()` | `uint64_t` / `int` (bytes) |
| **Data Validity** | `IsValid()` | `is_valid()` | `bool` |

!!! note "NOTE"

    The monitoring thread updates shared memory approximately every 1 second. Polling `GetCurrentStatus()` more frequently than this interval will return the same data.

**Formatted Summary Strings (C++ Only)**

| Method | Description | Return Value |
| :--- | :--- | :--- |
| `GetInfoString()` | Returns static hardware info (model, memory, board, firmware) | `std::string` |
| `GetStatusString()` | Returns dynamic real-time status (voltage, clock, temp, DVFS state) | `std::string` |

For a complete list of C++ methods (memory info, board type, device variants, etc.), see the [C++ API Reference](10_01_C++_API_Reference.md#class-dxrtdevicestatus).

---

### Complete Usage Example

This section demonstrates how to iterate through all available NPU devices and retrieve their full status information — including hardware status, core utilization, and memory usage — using both C++ and Python. These examples are based on the `device_monitoring` sample provided with the SDK.

For method details, see the [C++ API Reference](10_01_C++_API_Reference.md#class-dxrtdevicestatus) and [Python API Reference](10_02_Python_API_Reference.md#class-devicestatus).

**C++**

```cpp
#include <iostream>
#include <dxrt/dxrt_cxx_api.h>

void printDeviceInfo(const dxrt::DeviceStatus &ds)
{
    std::cout << "=== Device Info (id=" << ds.GetId() << ") ===" << std::endl;
    std::cout << ds.GetInfoString() << std::endl;
}

void printDeviceDynamicStatus(const dxrt::DeviceStatus &ds)
{
    std::cout << "[Device " << ds.GetId() << "]"
              << " temp=["
              << ds.GetTemperature(0) << ", "
              << ds.GetTemperature(1) << ", "
              << ds.GetTemperature(2) << "]C"
              << ", voltage=["
              << ds.GetNpuVoltage(0) << ", "
              << ds.GetNpuVoltage(1) << ", "
              << ds.GetNpuVoltage(2) << "]mV"
              << ", clock=["
              << ds.GetNpuClock(0) << ", "
              << ds.GetNpuClock(1) << ", "
              << ds.GetNpuClock(2) << "]MHz"
              << " | util=["
              << ds.GetCoreUtilization(0) << "%, "
              << ds.GetCoreUtilization(1) << "%, "
              << ds.GetCoreUtilization(2) << "%]"
              << ", mem_used=" << ds.GetMemoryUsed()
              << ", mem_free=" << ds.GetMemoryFree()
              << std::endl;
}

int main()
{
    int device_count = dxrt::DeviceStatus::GetDeviceCount();
    if (device_count == 0)
    {
        std::cout << "No DEEPX devices found." << std::endl;
        return 1;
    }

    std::cout << "Querying status for " << device_count << " device(s)...\n" << std::endl;

    for (int id = 0; id < device_count; id++)
    {
        auto ds = dxrt::DeviceStatus::GetCurrentStatus(id);
        if (!ds.IsValid())
        {
            std::cerr << "Warning: Device " << id << " data is not available." << std::endl;
            continue;
        }
        printDeviceInfo(ds);
        printDeviceDynamicStatus(ds);
        std::cout << std::endl;
    }

    return 0;
}
```

**Python**

```python
from dx_engine.device_status import DeviceStatus

def print_device_status(ds):
    """Print all fields from a DeviceStatus object."""
    print(f"=== Device Status (id={ds.get_id()}) ===")

    print(f"[STATUS] temp=["
          f"{ds.get_temperature(0)}, "
          f"{ds.get_temperature(1)}, "
          f"{ds.get_temperature(2)}]C, "
          f"voltage=["
          f"{ds.get_npu_voltage(0)}, "
          f"{ds.get_npu_voltage(1)}, "
          f"{ds.get_npu_voltage(2)}]mV, "
          f"clock=["
          f"{ds.get_npu_clock(0)}, "
          f"{ds.get_npu_clock(1)}, "
          f"{ds.get_npu_clock(2)}]MHz")

    print(f"[USAGE] util=["
          f"{ds.get_core_utilization(0):.1f}%, "
          f"{ds.get_core_utilization(1):.1f}%, "
          f"{ds.get_core_utilization(2):.1f}%], "
          f"mem_used={ds.get_memory_used()}, "
          f"mem_free={ds.get_memory_free()}")

def main():
    device_count = DeviceStatus.get_device_count()
    if device_count == 0:
        print("No devices found.")
        return

    print(f"Querying status for {device_count} device(s)...\n")

    for device_id in range(device_count):
        ds = DeviceStatus.get_current_status(device_id)
        if not ds.is_valid():
            print(f"Warning: Device {device_id} data is not available.")
            continue
        print_device_status(ds)
        print()

if __name__ == "__main__":
    main()
```

---

### Periodic Monitoring Example

The following examples demonstrate how to continuously monitor a device in a background thread with a configurable refresh interval. This is useful for frontend or business applications that need periodic device status updates while performing other tasks.

!!! note "NOTE"

    The runtime's monitoring thread updates shared memory approximately every **1 second**. Setting the refresh interval below 1 second will not yield more frequent data updates. A minimum interval of **1 second** is recommended.

**C++**

```cpp
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <dxrt/dxrt_cxx_api.h>

std::atomic<bool> g_running{true};

void monitorDevice(int deviceId, int intervalSec)
{
    while (g_running.load())
    {
        auto ds = dxrt::DeviceStatus::GetCurrentStatus(deviceId);
        if (ds.IsValid())
        {
            std::cout << "[Device " << ds.GetId() << "]"
                      << " temp=" << ds.GetTemperature(0) << "C"
                      << ", util=" << ds.GetCoreUtilization(0) << "%"
                      << ", mem_used=" << ds.GetMemoryUsed()
                      << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(intervalSec));
    }
}

int main()
{
    int device_id = 0;
    int interval_sec = 2;  // Refresh interval (minimum: 1 second)

    std::thread monitor_thread(monitorDevice, device_id, interval_sec);

    // Let the monitoring run for 30 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));

    g_running.store(false);
    monitor_thread.join();

    return 0;
}
```

**Python**

```python
import threading
import time
from dx_engine.device_status import DeviceStatus

def monitor_device(device_id: int, interval_sec: int, stop_event: threading.Event):
    while not stop_event.is_set():
        try:
            ds = DeviceStatus.get_current_status(device_id)
            if ds.is_valid():
                print(f"[Device {ds.get_id()}]"
                      f" temp={ds.get_temperature(0)}C"
                      f", util={ds.get_core_utilization(0):.1f}%"
                      f", mem_used={ds.get_memory_used()}")
        except Exception as e:
            print(f"Warning: {e}")
        stop_event.wait(interval_sec)

if __name__ == "__main__":
    device_id = 0
    interval_sec = 2  # Refresh interval (minimum: 1 second)

    stop_event = threading.Event()
    monitor_thread = threading.Thread(
        target=monitor_device,
        args=(device_id, interval_sec, stop_event)
    )
    monitor_thread.start()

    # Let the monitoring run for 30 seconds
    time.sleep(30)

    stop_event.set()
    monitor_thread.join()
```

---
