## v3.0.0 (August 2025)
- Update minimum versions 
  ```
  - Driver : 1.5.0 -> 1.7.1
  - PCIe Driver : 1.4.0 -> 1.4.1
  - Firmware : 2.0.5 -> 2.1.0
  ```
- fix kernel panic issue caused by wrong NPU channel number
- Update DeviceOutputWorker to use 4 threads for 4 DMA channels (3 channels to 4 channels)
- feat: Improve error message readability in install, build scripts (TFT-101)[https://deepx.atlassian.net/browse/TFT-101]
  ```
  - Apply color to error messages
  - Reorder message output to display errors before help messages
  ```
- Update Python Package version (v1.1.1 -> v1.1.2)
- Modify run_async_model and run_async_model_output examples
- Modify build.sh (print python package install info)
- removed some unnecessary items from header files
- use Pyproject.toml instead setup.py (now setup.py is not recommended)
- fix some rapidjson issue from clients.
- remove bad using namespace std from model.h (some programs need change)
- Add usb inference module (tcp/ip)
(MACRO : DXRT_USB_NETWORK_DRIVER)
- Add options to SanityCheck.sh
   ```
   - Usage: sudo SanityCheck.sh [all(default) | dx_rt | dx_driver | help]
   ```
-  Change build compiler has been updated to version 14 for both USE_ORT=ON and USE_ORT=OFF configurations.
- Fix an issue where temporary files from the ONNX Runtime installation would accumulate.
- Fix a cross-compilation error related to the ncurses library for the dxtop utility.
- Add Sanity Check Features
   ```
   - Dependency version check.
   - Executable file check.
   ```
- Add APIs to the Configuration class for retrieving version information.
- PCIE details displayed on some device errors
- dxrt-cli --errorstat option added (this shows pcie detailed information)
- Modify run_model logging to include host info (Linux only).
- Add Python examples for configuration and device status.
- Add Python API for configuration and device status. (dx-engine-1.1.1)
- Add functionality to query the framework & driver versions in the Configuration class.
- Add weight checksum info for service
- Add ENABLE_SHOW_MODEL_INFO build option and configuration item
- Update code for compatibility with v3 environment 
- Enhance UI for better clarity, enabled dynamic data rendering, and added visual graphs for NPU Memory usage.
- Fix: fix dx-rt build error caused by pybind11 incompatibility with Python 3.6.9 on Ubuntu 18.04 [TFT-82](https://deepx.atlassian.net/browse/TFT-82)
  ```
  - Support automatic installation of minimum required Python version (>= 3.8.2)  
  - Install Python 3.8.2 if the system Python version is not supported
  - On Ubuntu 18.04, install via source build; on Ubuntu 20.04+, use apt install
  - Added support in install.sh to optionally accept --python_version and --venv_path for installation
  - Added support in build.sh to accept and use --python_exec
  - Added support in build.sh to optionally accept --venv_path and activate the specified virtual environment
  ```
- The default build option for DX-RT has been changed from USE_ORT=OFF to USE_ORT=ON. If the inference engine option is not specified separately, use_ort will be enabled by default, activating the CPU task for .dxnn models.
- Add dxtop tool, a terminal-based monitoring tool for Linux environments. It provides real-time insights into NPU core utilization and DRAM usage per NPU device.
 

## v2.9.5 (May 2025)
- Added full support for Python run_model.  
- Updated the run_model option and its description  
- Improve the Python API  
    ```
    - InferenceOption is now supported identically to the C++ API.  
       - set_devices(...) → devices = [0]  
       - set_bound_option(...) → bound_option = InferenceOption.BOUND_OPTION.NPU_ALL
       - set_use_ort(...) → use_ort = True
    - Callback functions registered via register_callback now accept user_arg of custom types. (removed .value)
       - user_arg.value → user_arg
    - run() now supports both single-input and batch-input modes, depending on the input format.
    ```
- Modify the build.sh script according to cmake options.  
    ```
    - CMake option USE_ORT=ON, running build.sh --clean installs ONNX Runtime.  
    - CMake option USE_PYTHON=ON, running build.sh installs the Python package.  
    - CMake option USE_SERVICE=ON, running build.sh starts or restarts the service.  
    ```
- Add dxrt-cli -v to display minimum driver & compiler versions  
- Addressed multithreading issues by implementing additional locks, improving stability under heavy load.  
- Fix crash on multi-device environments with more than 2 H1 cards. (>=8 devices)  
- Resolved data corruption errors that could occur in different scenarios, ensuring data integrity.  
- Fix profiler bugs.  
- Addressed issues identified by static analysis and other tools, enhancing code quality and reliability.  
- Add --use_ort flag to the run_model.py example for ONNX Runtime.  
- Add run batch function. (Python & C++)  
    ```
    - batch inference with multiple inputs and multiple outputs. 
    ``` 
- Minimum model file versions  
    ```
    - .dxnn file format version >= v6  
    - compiler version >= v1.15.2  
    ```
- Minimum Driver and Firmware versions  
    ```
    - RT Driver Version >= v1.5.0  
    - PCIe Driver Version >= v1.4.0  
    - Firmware Version >= v2.0.5  
    ```

## v2.8.2 (April 2025)

- Modify Inference Engine to be used with 'with' statements, and update relevant examples.  
- Add Python inference option interface with the following configurations  
- NPU Device Selection / NPU Bound Option / ORT Usage Flag  
- Display dxnn versions in parse_model (.dxnn file format version & compiler version)  
- Added instructions on how to retrieve device status information  
- Driver and Firmware versions  
    ```
    - RT Driver >= v1.3.3  
    - Firmware >= v1.6.3
    ```  

---
