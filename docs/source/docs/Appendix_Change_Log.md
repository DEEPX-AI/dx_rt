## v3.4.0 (July 2026)

#### Changed
- Debian package bundles `dx_engine` Python wheels for Python 3.8 – 3.14.
- CLI binary names updated for consistency: `dxrt-cli` → `dxcli`, `parse_model` → `dxparse`, `run_model` → `dxrun`
- Old names (`dxrt-cli`, `parse_model`, `run_model`) remain functional as backward-compatible aliases (symlinks on Linux/macOS, hard links on Windows)
- Python bindings now use dxrt_cxx_api.h (header-only) instead of direct C++ linkage
- libdxrt.so exports C symbols only (dxrt_*); internal C++ symbols hidden via version script
- Update minimum versions
   - Driver : 2.4.0 -> 2.5.0
   - PCIe Driver : 2.2.0 -> 2.4.0
   - Firmware : 2.5.2 -> 2.7.0
- Code style cleanup across Python package (`inference_option.py`, `runtime_event_dispatcher.py`, `utils.py`, etc.)
- Upgrade extern/include/dxrt/extern/cxxopts.hpp from v3.1.1 to v3.3.1
- Increase firmware update sequence wait time from 2s to 4s.
- Replace top-level matplotlib imports with lazy loading via `ensure_dependencies()` in `plot.py` to provide install guidance instead of ImportError

#### Fixed
- Improved shared-memory performance.
- Fix SEGV in NFHLayer caused by dereferencing uninitialized `Request::_task` in profiler instrumentation
- Return `INT16_MIN` instead of `0` from `DeviceStatus::GetTemperature()` when channel index is out of range
- Improve dxrtd error messages when manually executed
- Fix missing separator between `devices` and `buffer_count` fields in `InferenceOption.__repr__` output
- Update ppcpu logic for hardness
- Fixed a critical crash (nullptr) in pyRunBenchmark by ensuring the memory lifetime of input buffers.
- Improve temperature range validation in dxtop.
- Fix multi-input dictionary handling issue.

#### Added
- Script to build `dx_engine` wheels per Python version.
- Documentation describing the Debian prebuilt directory structure.
- Script to generate the `prebuilt/` directory.
- Stable C ABI (dxrt_c_api.h, 103 functions) enabling prebuilt SDK distribution without recompilation
- Header-only C++ wrapper (dxrt_cxx_api.h) for single-include modern C++ usage
- SDK-compat bridge headers (legacy/) for backward compatibility with existing dxrt_api.h code
- H1M firmware compatibility check: distinguish H1M (LPDDR4) from H1 (LPDDR5/LPDDR5X) in firmware update and device detection; support mixed 4-pack / 6-pack H1M configurations
- Deprecated Python API migration guide added to Python API Reference documentation
- Add shared memory-based inter-process communication using memfd
- Add dynamic IPC infrastructure with packet-based protocol layer
- Add cross-platform support for Linux and Windows IPC transport
- Add new device monitoring APIs (memory usage, per-core temperature, and utilization)
- Add a tool (plot_html.py) to generate HTML visualization reports for profiling data (profiler.json)
- Profiler
  - Add GetJobMetrics() API for comprehensive per-job profiling
  - Enhance performance statistics with Coefficient of Variation (CoV) metric
- Add input dtype validation to prevent undefined behavior caused by mismatching NumPy array types.
- Add `libdxrt-bin` Debian packaging for pre-built binaries with amd64/arm64 auto-detection.
 

## v3.3.2 (May 2026)

#### Changed
- Removed redundant build artifacts and temporary directories from the Debian package.

#### Fixed
- Improve Python extension module linking for _pydxrt build.

#### Added
- Added conditional pip upgrade (v21.3+) to ensure build stability on legacy OS environments.

## v3.3.1 (April 2026)

#### Changed
- Change the version of pre-built onnxruntime(1.23.2 -> 1.22.0) and openvino(25.4 -> 25.1)
- Enhance uninstall script to remove debian packages, legacy binaries, and test files from previous versions

#### Fixed
- Fix typos in the document
- Fix error in uninstall logic

#### Added
- Add libdxrt 3.3.1 debian package with updated build and install pipeline

## v3.3.0 (April 2026)

#### Changed
- Update minimum versions
   - Driver : 1.8.0 -> 2.4.0
   - PCIe Driver : 1.5.1 -> 2.2.0
   - Firmware : 2.4.0 -> 2.5.2
- Update the Python module version to match the C++ Runtime version.

#### Fixed
- Fix PPU data transfer error during multi-process execution in H1 and Multiple M1 M.2 environments.
- Fix input data lifecycle issues in the Python Runtime Module.
- Fix intermittent interrupt exceptions in IPC Message Queue.

#### Added
- Add dxtop for No Service Mode
- Add an example that loads an entire model file into a memory buffer and performs inference directly using this memory buffer.
- Add python InferenceEngine from numpy array
- Add acceleration features for CPU operations (Requires separate option configuration and build)


## v3.2.0 (December 2025)

#### Changed
- Optimize PCIe DMA sequence for better performance.
- Update OS requirements in installation guide for debian

#### Fixed
- Optimize device memory footprint of PPU models

#### Added
- Add RuntimeEventDispatcher class for C++
   - RuntimeEventDispatcher is a singleton class that provides centralized event dispatching and handling for runtime events from the DX-RT system, such as device errors, warnings, and notifications.
- Add Python wrapper for RuntimeEventDispatcher class -> dx_engine 1.1.4
- Add `--profiler` option to enable profiling mode in run_model.py
- Add `--buffer-count` option to configure inference buffer count in run_model.py
- Add build.sh options
```
  --use_service_on  Enable the use of the service in the build.
  --use_service_off Disable the use of the service in the build.
  --use_ort_on      Enable the use of the ORT component in the build.
  --use_ort_off     Disable the use of the ORT component in the build.
```
- Add `__version__` import to the main `dx_engine` module namespace
- Implement per-instance configuration of Input and Output buffer counts for the Inference Engine.
- Enable direct loading of the .dxnn model format from a memory buffer within the Inference Engine.
- Add RuntimeEventDispatcher for centralized event handling and logging.

## v3.1.0 (November 2025)

#### Changed
- Update minimum versions
  ```
  - Driver: 1.7.1 → 1.8.0
  - PCIe Driver: 1.4.1 → 1.5.1
  - Firmware: 2.1.0 → 2.4.0
  ```
- Standardize command-line argument parsing across all examples
  ```
  - Use argparse (Python) and cxxopts (C++)
  - Unified argument format: -m/--model, -l/--loops, etc.
  ```
- Update SanityCheck.sh
  ```
  - Remove redundant libdxrt.so location check (validated via run_model execution)
  - Hide ONNX Runtime version when built with USE_ORT=OFF
  ```
- Enhance build and installation scripts
  ```
  - Use 'python3 -m pip' instead of 'pip' for better reliability
  - Add Python version compatibility check in build.sh
  - Improve logging with color-coded messages
  - Add uninstall.sh script for project cleanup
  ```
- Improve profiler functionality
  ```
  - Add Flush and GetPerformanceData functions
  - Group events by base name instead of individual job/request entries
  - Limit duration details to 30 values per group for cleaner output
  - Add memory usage tracking with high usage warnings
  ```
- Improve parse_model CLI tool with enhanced functionality
- Support dynamic shape output for tail CPU tasks
- Update C++ exception handling to translate exceptions into Python

#### Fixed
- Fix multiple bugs
  ```
  - Multi-tasking bugs related to CPU offloading and PPU output buffer
  - PPU model format and layout issues
  - Multi-output and multi-tail configuration bugs
  - Tensor mapping errors in non-ORT inference mode
  - Output tensor mapping and memory address configuration
  - run_model error when -f option and -l loop count exceeds 1024
  - Bounding issues on service
  ```

#### Added
- Add support for V8 DXNN file format with PPU support
- Add model voltage profiler tool (run_model_prof.py)
  ```
  - Requires firmware ≥ 2.2.0 and driver ≥ 1.7.1
  ```
- Add dxbenchmark CLI tool for performance comparison across multiple models
  ```
  - Default execution of 30 loops when neither loop nor time options are specified
  - Automatically creates result directory when result path is specified
  ```
- Add system requirements check in install.sh
  ```
  - Verify RAM ≥ 8GB
  - Verify architecture (x86_64 or aarch64)
  - Exit with error if requirements not met
  ```
- Add model file format version range validation
  ```
  - Error messages now indicate valid version range (min to max)
  ```
- Add time-based inference mode to run_model (-t, --time option)
- Add error handling for invalid firmware files and update conditions
- Add dxtop enhancements
  ```
  - Display PCIe bus number
  - Update cross-compilation script
  ```
- Add new Python APIs for device configuration and status retrieval
- Add new documentation files for Inference API, Multi-Input Inference, and Global Instance
- Add examples for asynchronous model inference with profiling capabilities

---

## v3.0.0 (September 2025)

- Update the .dxnn file format to version 7 (from v6).
- Update C++ exception handling to translate exceptions into Python for improved error handling.
- Update the Python v6_converter with enhanced functionality.
- Add support for models with multiple heads, tails, inputs, and outputs, including new C++ and Python APIs.
- Add a new internal C++ converter for v6 models.
- Add new Python APIs for handling device configuration and status retrieval.
- Fix several multi-tasking bugs related to CPU offloading buffer management and PPU output buffer mis-pointing.
- Fix a bug in the process of setting the PPU model format and layout.
- Fix a critical bug affecting models with multi-output and multi-tail configurations.
- Fix tensor mapping errors that occurred in non-ORT inference mode.
- Fix a warning message in get_output_tensors_info and a vector access bug in _npuModel.
- Fix an issue that prevented error messages from being displayed.
- Fix flaws in output tensor mapping and memory address configuration.
- Enhance OS and architecture checks in installation scripts 
- Update documentation to reflect changes in supported CPU architecture and OS requirements.
- Enhance build and uninstall scripts with common utilities and improved logging
- Add PCIe bus number display for dxtop
- Add profiling data memory usage tracking with high usage warnings.
- Update user guide document
- Force-disabled with a warning instead of throwing a runtime exception in builds that don't support USE_ORT.
- Add time-based inference mode to run_model (-t, --time option)
- Profiler now groups events by base name (before) instead of showing individual job/request entries
- Limited duration details to 30 values per group for cleaner output
- Fix run_model error when -f option and -l loop count exceeds 1024
- Fix bounding option issue on service
- Add error handling for invalid firmware files and update conditions.
- Add a function to check Python version compatibility in build.sh.
- Add new documentation files for Inference API, Multi-Input Inference, and Global Instance.
- Add examples for asynchronous model inference with profiling capabilities in both C++ and Python.
- Update minimum versions 
  ```
  - Driver : 1.5.0 -> 1.7.1
  - PCIe Driver : 1.4.0 -> 1.4.1
  - Firmware : 2.0.5 -> 2.1.0
  ```
- Fix kernel panic issue caused by wrong NPU channel number
- Update DeviceOutputWorker to use 4 threads for 4 DMA channels (3 channels to 4 channels)
- Improve error message readability in install, build scripts
  ```
  - Apply color to error messages
  - Reorder message output to display errors before help messages
  ```
- Update Python Package version (v1.1.1 -> v1.1.2)
- Modify run_async_model and run_async_model_output examples
- Modify build.sh (print python package install info)
- removed some unnecessary items from header files
- use Pyproject.toml instead of setup.py (now setup.py is not recommended)
- Fix some rapidjson issue from clients.
- Remove bad using namespace std from model.h (some programs need change)
- Add usb inference module (tcp/ip)
(MACRO : DXRT_USB_NETWORK_DRIVER)
- Add options to SanityCheck.sh
   ```
   - Usage: sudo SanityCheck.sh [all(default) | dx_rt | dx_driver | help]
   ```
- The build compiler has been updated to version 14 for both USE_ORT=ON and USE_ORT=OFF configurations.
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
- Fix: fix dx-rt build error caused by pybind11 incompatibility with Python 3.6.9 on Ubuntu 18.04
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
 
---

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

---

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
