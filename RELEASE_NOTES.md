# RELEASE_NOTES

## v3.2.0 / 2025-12-23

### 1. Changed
- Optimize PCIe DMA sequence for better performance.
- docs: update OS requirements in installation guide for debian

### 2. Fixed
- Optimize device memory footprint of PPU models

### 3. Added
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

## DX-RT v3.1.0 / 2025-11-28

### 1. Changed
- Update model file format version checks to include max version…
- Update minimum versions
  - Driver : 1.7.1 -> 1.8.0
  - PCIe Driver : 1.4.1 -> 1.5.1
  - Firmware : 2.1.0 -> 2.4.0
- Update Sanity Check to hide ONNX Runtime version when built with USE_ORT=OFF.
- Standardize all Python and C++ examples to use argparse and cxxopts for consistent command-line argument parsing
- Update all examples to support unified argument format
    - -m, --model for .dxnn model file path
    - -l, --loops for inference loop count
    - Additional options like verbose follow similar format conventions per example
- Update dxbenchmark default behavior to execute 30 loops when neither loop nor time options are specified
- Enhance dxbenchmark to automatically create result directory if it does not exist when result path is specified
- add a system requirement check in install.sh (RAM: 8GB, Arch: x86_64 or aarch64)
- remove the check for the libdxrt.so location in Sanity Check
- Improve parse_model CLI tool.
- remove dsp related code
- Update the .dxnn file format to version 7 (from v6).
- Update C++ exception handling to translate exceptions into Python for improved error handling.
- Update the Python v6_converter with enhanced functionality.
- Update license information
- feat: enhance OS and architecture checks in installation scripts
- Update user guide document
- Profiler now groups events by base name (before ) instead of showing individual job/request entries
- Limited duration details to 30 values per group for cleaner output

### 2. Fixed
- Use 'python3 -m pip' instead of 'pip' for better reliability
- fix some compile errors and warnings in windows environment
- Fix configuration option name in common.cfg.
- Update cross compile script for dxtop
- Fix several multi-tasking bugs related to CPU offloading buffer management and PPU output buffer mis-pointing.
- Fix a bug in the process of setting the PPU model format and layout.
- Fix a critical bug affecting models with multi-output and multi-tail configurations.
- Fix tensor mapping errors that occurred in non-ORT inference mode.
- Fix a warning message in get_output_tensors_info and a vector access bug in _npuModel.
- Fix an issue that prevented error messages from being displayed.
- Fix flaws in output tensor mapping and memory address configuration.
- docs: Updated documentation to reflect changes in supported CPU architecture and OS requirements.
- Force-disabled with a warning instead of throwing a runtime exception in builds that don't support USE_ORT.
- fix run_model error when -f option and -l loop count exceeds 1024
- Fix bounding issue on service

### 3. Added
- Loading PPCPU Firmware on service initialization
- (in NO_SERVICE mode, it is loaded with first inferenceengine initialization
- Include binary data as Array on source code, comes with internal-only generation script
- Add support for V8 DXNN file format
- Add PPU support for V8 models
- Support dynamic shape output of tail cpu task
- Implement asynchronous NPU Format Handler (NFH).
- Add new functions to profiler (Flush and GetPerformanceData)
- Add dxbenchmark, a command-line tool for comparing performance metrics across multiple models and generating detailed
- model voltage profiler (run_model_prof.py)
  - requires firmware > 2.2.0 and driver > 1.7.1
- Add a new internal C++ converter for v6 models.
- Add new Python APIs for handling device configuration and status retrieval.
- feat: enhance build and uninstall scripts with common utilities and improved logging
  - Integrated common utility functions into build.sh for better modularity.
  - Added uninstall.sh script to handle project uninstallation, including cleanup of symlinks and directories.
  - Improved logging in both scripts using color-coded messages for better user feedback.
  - Updated color_env.sh and common_util.sh to support new logging features and ensure consistent output formatting.
  - Refactored build.sh to streamline the build process and enhance error handling.
- Added PCIe bus number display for dxtop
- Add profiling data memory usage tracking with high usage warnings.
- Add time-base inference mode to run_model (-t, --time option)
- Add error handling for invalid firmware files and update conditions.
- Add a function to check Python version compatibility in build.sh.
- Add new documentation files for Inference API, Multi-Input Inference, and Global Instance.
- Add examples for asynchronous model inference with profiling capabilities in both C++ and Python.

---

## DX-RT v3.0.0 / 2025-07-31  

### 1. Changed
- Update minimum versions 
   - Driver : 1.5.0 -> 1.7.1
   - PCIe Driver : 1.4.0 -> 1.4.1
   - Firmware : 2.0.5 -> 2.1.0
- Update DeviceOutputWorker to use 4 threads for 4 DMA channels (3 channels to 4 channels)
- Update Python Package version (v1.1.1 -> v1.1.2)
- Modify run_async_model and run_async_model_output examples
- Modify build.sh (print python package install info)
- removed some unnecessary items from header files
- use Pyproject.toml instead setup.py (now setup.py is not recommended)
- Add options to SanityCheck.sh
   - Usage: sudo SanityCheck.sh [all(default) | dx_rt | dx_driver | help]
- Change build compiler has been updated to version 14 for both USE_ORT=ON and USE_ORT=OFF configurations.
- Modify run_model logging to include host info (Linux only).
- Enhance UI for better clarity, enabled dynamic data rendering, and added visual graphs for NPU Memory usage.
- Change default build option for DX-RT from USE_ORT=OFF to USE_ORT=ON. If the inference engine option is not specified separately, use_ort will be enabled by default, activating the CPU task for .dxnn models.
- Add automatic handling of input dummy padding and output dummy slicing when USE_ORT=OFF (build-time or via InferenceOption). Applications no longer need to add input dummy data or remove output dummy data for inference.
  
### 2. Fixed  
- fix kernel panic issue caused by wrong NPU channel number
- feat: Improve error message readability in install, build scripts
  - Apply color to error messages
  - Reorder message output to display errors before help messages
- fix some rapidjson issue from clients.
- remove bad using namespace std from model.h (some programs need change)
- Fix an issue where temporary files from the ONNX Runtime installation would accumulate.
- Fix a cross-compilation error related to the ncurses library for the dxtop utility.
- Update code for compatibility with v3 environment
- fix: fix dx-rt build error caused by pybind11 incompatibility with Python 3.6.9 on Ubuntu 18.04
  - Support automatic installation of minimum required Python version (>= 3.8.2)  
  - Install Python 3.8.2 if the system Python version is not supported
  - On Ubuntu 18.04, install via source build; on Ubuntu 20.04+, use apt install
  - Added support in install.sh to optionally accept --python_version and --venv_path for installation
  - Added support in build.sh to accept and use --python_exec
  - Added support in build.sh to optionally accept --venv_path and activate the specified virtual environment

### 3. Added  
- Add usb inference module (tcp/ip)
(MACRO : DXRT_USB_NETWORK_DRIVER)
- Add Sanity Check Features
   - Dependency version check.
   - Executable file check.
- Add APIs to the Configuration class for retrieving version information.
- PCIE details displayed on some device errors
- dxrt-cli --errorstat option added (this shows pcie detailed information)
- Add Python examples for configuration and device status.
- Add Python API for configuration and device status. (dx-engine-1.1.1)
- Add functionality to query the framework & driver versions in the Configuration class.
- Add weight checksum info for service
- Add ENABLE_SHOW_MODEL_INFO build option and configuration item
- Add dxtop tool, a terminal-based monitoring tool for Linux environments. It provides real-time insights into NPU core utilization and DRAM usage per NPU device.
- Add support for both .dxnn file formats: v6 (compiled with dx_com 1.40.2 or later) and v7 (compiled with dx_com 2.x.x).

---

## DX-RT v2.9.5 / 2025-06-09  

### 1. Changed
- Modified Python tensor info dictionary results. Removed 'size_in_bytes' and added 'elem_size' to the dictionaries returned by get_input_tensors_info() and get_output_tensors_info().
- Set the service to launch after a reboot when service=ON is built.
- Updated the run_model option and its description.
  - Changed the way device and NPU bounding options are configured.
  - Provided more detailed inference result information.
  - Added full support for Python run_model.
- Minimum Driver & Compiler versions
   - RT Driver version : v1.5.0
   - PCIe Driver version : v1.4.0
   - Firmware version : v2.0.5
   - .dxnn File Format Version : v6
   - Compiler : v1.15.2
- Removed the 'tools' directory and consolidated its functionalities within the example directory for streamlined project structure.

### 2. Fixed
- Fixed a bug where the run() API was returning incorrect output
- Fixed a bug where GetNpuInferenceTime related APIs returned incorrect values
- Fixed a bug where the task load could be displayed as a negative value
- Fixed incorrect 'dtype' in Python tensor info functions. Corrected the 'dtype' reported by get_input_tensors_info(), get_output_tensors_info(), and similar functions.

### 3. Added
- Included details about DXRT_DYNAMIC_CPU_THREAD usage in the model inference documentation (04_Model_Inference.md)
- Improved usability for python InferenceOption(). Users can now directly set the option variable without needing a separate method.
- Improved the Python API
  - InferenceOption is now supported identically to the C++ API.
  - Callback functions registered via register_callback now accept user_arg of custom types.
  - run() now supports both single-input and batch-input modes, depending on the input format.
- Add display_async_models examples

---

## DX-RT v2.8.4 / 2025-05-12

### 1. Changed
- Modify the build.sh script according to cmake options
  - CMake option USE_ORT=ON, running build.sh --clean installs ONNX Runtime.
  - CMake option USE_PYTHON=ON, running build.sh installs the Python package.
  - CMake option USE_SERVICE=ON, running build.sh starts or restarts the service.
- Improved callback handling by removing std::async, potentially leading to more predictable execution
- Enhanced concurrency by making key variables atomic, resolving potential race conditions.
- Addressed multithreading issues by implementing additional locks, improving stability under heavy load.
- Removed obsolete code, streamlining the codebase

### 2. Fixed
  - Fix crash on multi-device environment with more than 2 H1 cards(>=8 devices)
  - Resolved data corruption errors that could occur in different scenarios, ensuring data integrity.
  - Fix profiler bugs
  - Addressed issues identified by static analysis and other tools, enhancing code quality and reliability.

### 3. Added
- USE_ORT Option for Python BItmatch.py
- Add --use_ort flag to the run_model.py example for ONNX Runtime
 - Implemented profiler on/off functionality (by Configuration)
 -  Implemented a check to prevent tasks from being started multiple times, ensuring correct execution flow.
 - Implemented device blocking device on error
 - Implemented page alignment for buffers to address some I/O issues.

---

## DX-RT v2.8.3 / 2025-04-11

### 1. Changed
- Improve driver, firmware, and file format version check messages

### 2. Fixed
- None

### 3. Added
- Add --all option to build.sh

---

## DX-RT v2.8.2 / 2025-03-21

### 1. Changed
- Modified the run_async_model_output example to improve the passing condition.
- Modify Inference Engine to be used with 'with' statements, and update relevant examples.

### 2. Fixed
- failed to read output -70 bug

### 3. Added
- Round Robin,Shortest Job First scheduler added
- Implemented C++ Run (Batch) function within the InferenceEngine for batched inference execution.
- Added a new example, run_batch_model, demonstrating the usage of the batch inference function.
- Display the memory usage of the loaded model.
- Add Python inference option interface with the following configurations
   * NPU Device Selection / NPU Bound Option / ORT Usage Flag

---

## DX-RT v2.7.1 / 2025-03-12

### 1. Changed
- display dxnn versions in parse_model (.dxnn file format version & compiler version)

### 2. Fixed
- None

### 3. Added
- Add otp read / write api (internal only)

---

## DX-RT v2.7.1 / 2025-03-11
### 1. Changed
- Added instructions on how to retrieve device status information
- Driver and Firmware versions
  - RT Driver >= v1.3.3
  - Firmware >= v1.6.3
### 2. Fixed
- Include batch size in PPU output shape in Python API
### 3. Added
- Implemented retrieval of device status information by device ID
- Retrieved the count of installed devices
- Non contiguous input handling in Python API

---

## DX-RT v2.7.0 / 2025-02-25
### 1. Changed
- API renaming
- Optimize sync timing in asynchronous inference scenario
- DX-COM version >= 1.40.2
- onnxruntime version >= 1.20.1
### 2. Fixed
- Troubleshooting abnormal process terminations
- Multi process termination bug
- Stabilization on Windows operating systems
- Restrict multiple services from running
### 3. Added
- Configuration
- Dynamic CPU task multi threading
- Statistics profiler
- Clang compiler
- Average load on NPU devices and CPU tasks

---

## DX-RT v2.6.3 / 2025-01-06
### Changed
- seperate msg queue for Send To / Receive From
- merge windows code & modify bitmatch (C++)
### Fixed
- fix NPU memory leaks
### Added
- NONE

---

## DX-RT v2.6.2 / 2024-12-19
### Changed
- NONE
### Fixed
- Fix free output buffer locking issue for multi-threaded runAsync
### Added
- Modify configuration function for throttling using json

---

## DX-RT v2.6.1 / 2024-12-11
### Changed
- NONE
### Fixed
- Fix multi-device performance error
- Fix issue with running python run_model due to API not being updated.
### Added
- NONE

---

## DX-RT v2.6.0 / 2024-12-10
### Changed
- Modify inference load control
- onnxruntime minimum version : v1.18.0
- Update python version : v1.0.0
- Drvier and Firmware versions
  - RT Drvier  >= v1.3.0
  - PCIe Driver  >= v1.2.0
  - Firmware  >= v1.5.9
### Fixed
- Fixed a problem that did not work when using user memory in conjunction with the inference engine
- Fix profiler momory corruption issue
- Fix multi-device performance issue
### Added
- Add NPU memory caching

---

## DX-RT v2.1.0 / 2024-10-31
### Changed
- run_model async mode as default  
- Drvier and Firmware versions
  - RT Drvier  >= v1.1.0
  - PCIe Driver  >= v1.1.0
  - Firmware  >= v1.5.5
### Fixed
-
### Added
- Supports multi-process & multi-device
  - dxrtd daemon
- Supports Python Interface (Run & RunAsync)
  - Async mode / Batch mode
- Device status monitoring function via cli-command

---

## DX-RT v2.0.3 / 2024-09-04
### Changed
- align change: 64 to 16
### Fixed
- remove cross compile package for non-x64 environment
### Added
- add firmware upload mode on cli
- support INT64 for cpu onnx

---

## DX-RT v2.0.1 / 2024-08-06
### Changed
- None
### Fixed
- Fix argmax model w/ empty output
### Added
- None

---

## DX-RT v2.0.0 / 2024-08-02
### Changed
- dxnn version up(v6). so prior dxnn models will not work from this version.
### Fixed
- None
### Added
- stress test script
- batch run async in pybinding

---

## DX-RT v1.2.3 / 2024-07-23
### Changed
- Update process id & model_format for device message
- Remove device dependency for parse_model
### Fixed
- Fix memory leck problem
- Fix FindPythonInterp error after cmake 3.27
- Fix ppu output bug
### Added
- Implement multi-task and multi in/out for achieving CPU offloading level 1

---

## DX-RT v1.1.2 / 2024-07-03
### Changed
- update documents
### Fixed
- None
### Added
- None

---

## DX-RT v1.1.1 / 2024-06-03
### 1. Changed
- simplify dxrt-cli status message
- remove unnesessary outputs by dx_rt library
- change arch option: arm64->aarch64
### 2. Fixed
- fix cross-compile issue: cross compile occurs on aarch64 issue
- fix model memory check logic
### 3. Added
- None

---

## DX-RT v1.0.1 / 2024-05-23
### 1. Changed
- make option : -j8 -> -j$(nproc)
### 2. Fixed
- fix library install path issue: some files installed under /cmake/dxrt
### 3. Added
- None

---

## DX-RT v1.0.0 / 2024-04-29
### 1. Changed
- DXNN Version2 architecture sdk
- Remove driver folder
  Please refer to "dx_rt_npu_linux_driver".
### 2. Fixed
- None
### 3. Added
- Pybind11 support
  DXRT supports some Python APIs.

---

## DX-RT v0.5.4 / 2023-07-17
### 1. Changed
- Changed classification demo name
### 2. Fixed
- None
### 3. Added
- Support model configuration for real time face recognition demo
- Support to receive device information and dump device memory from commandline interface application

---

## DX-RT v0.5.3 / 2023-06-14
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Support DX-H1 ASIC
- Added firmware CLI tool for DX-M1, DX-H1
- Improve YOLOX postproc. performance

---

## DX-RT v0.5.2 / 2023-05-10
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Support DX-M1 ASIC
- Added PCIe driver build environment for DX-M1
- Added pose estimation application
- Added FPS estimation for run_model application

---

## DX-RT v0.5.1 / 2023-04-03
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Added post-processing callback API
- Added ethernet input scenario for yolo demo
- Added tensor transpose API
- Expand model image size parameter for yolo
- Added network packet classification application
- Added source code of L1 NPU driver

---

## DX-RT v0.5.0 / 2023-02-22
### 1. Changed
- Device variant/type setting is removed from build script (Device auto-detection is applied).
- Reduced interrupt latency for standalone device
### 2. Fixed
- None
### 3. Added
- Added yolov7 configurations in object detection app.
- Added PCIe driver (only for DX-M1 FPGA)

---

## DX-RT v0.4.0 / 2023-01-05
### 1. Changed
- Refactor device parameters in runtime lib.
### 2. Fixed
- None
### 3. Added
- Add ISP interface for object detection demo
- Add ONNX runtime interface for CPU task (verified only x86_64)

---

## DX-RT v0.3.1 / 2022-12-12
### 1. Changed
- Improve object detection pre/post parameter
- Unified post-processing for yolov5, yolox
### 2. Fixed
- None
### 3. Added
- Support documents generation

---

## DX-RT v0.3.0 / 2022-12-05
### 1. Changed
- Refactor face recognition application
- Change build architecture for auto-release
- Improve docs.
### 2. Fixed
- None
### 3. Added
- Add parse_model application

---

## DX-RT v0.2.0 / 2022-11-22
### 1. Changed
- Separate dev-build, release-build
### 2. Fixed
- None
### 3. Added
- Common framework for devices
- Support OpenCV for riscv64, arm64
- Support documentation as markdown format
- Added doxygen for API reference
- Support encrypted NPU parameters

---

## DX-RT v0.1.0 / 2022-06-30
- Initial release for DX-L1 (eyenix FPGA)
