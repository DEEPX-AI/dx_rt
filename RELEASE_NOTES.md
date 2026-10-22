# RELEASE_NOTES

## DX-RT v3.5.0 / 2026-10-23

### 1. Changed
- **`dxcli` Version Output**: Changed to `version (build: hash)`.
- **DDR Type in `dxtop`**: Shown in the device variant string.
- **`NOTICE.md`**: Added detailed ncurses and PDCurses license information.
- **Latency Definition**: Latency (`GetLatency*`) now equals E2E Response Time; use Core Latency for the value excluding buffer pool and queue wait times.
- **`dxrun` Latency Description**: Changed to "Core latency per request excluding buffer pool and queue wait times".
- **Minimum Versions**: Raised the minimum required driver and firmware versions.
   - Linux Driver : 2.5.0 -> 2.7.0
   - Linux PCIe Driver : 2.4.0 -> 2.7.0
   - Windows PCIe Driver : 1.5.1 -> 2.0.1
   - Firmware : 2.7.0 -> 2.7.6
- **Windows Driver Request Errors**: Pending requests that later fail are now reported as errors.
- **Windows `dxrtd` Named Pipe**: Rejects remote clients.
- **C# Support via C API**: Removed the C++/CLI `csharp_package` and its build step (no .NET SDK/msbuild needed); C# now uses the C API via P/Invoke.

### 2. Fixed
- `dxrun` now exits with an error when no inference succeeds.
- `--max-throughput` now fails when no round succeeds, instead of recommending an untested buffer count.
- `dxrun` now rejects non-positive `--loops` and negative `--time` values.
- Fixed a segfault when PPU outputs are empty.
- Fixed busy-spin during teardown after device `Identify` failures.
- Fixed deadlock/heap corruption on `ServiceDevice` shutdown and swapped arguments in sub-command logging.
- Fixed divide-by-zero in the usage timer when the tick interval is 0.
- Cleared stale IPC client response state on `sendMessageSync` timeout.
- Fixed `dxrtd` stability issues (NPU memory leak on abnormal exit, Windows service crashes/stalls, device re-enable) and `find_package(dxrt)` failure on Windows.
- Prevented latency statistics overflow in long-running sessions (updates stop at `INT_MAX` samples).

### 3. Added
- **V9 `.dxnn` Format**: Added V9 model parser, including PPU data transfer.
- **Python/Native Version Check**: `import dx_engine` raises `RuntimeError` if the `libdxrt.so` major.minor version differs (bypass: `DX_ENGINE_SKIP_VERSION_CHECK=1`).
- **`dxrun --max-throughput` Mode**: Auto-tunes the I/O buffer count for peak throughput.
- **`showModelInfo` Option**: `InferenceOption::showModelInfo` (C++) / `dxrt_options_t::show_model_info` (C) hides the model info banner per engine; ABI size unchanged.
- **Input Quantization (SIMD)**: float32 → int8 input quantization with NEON/SSE4.1 kernels, reducing DMA write time.
- **Fan Control**: Forced on / forced off / auto modes.
- **FCT/OTP Authentication (H1M)**: FCT test results with OTP authentication (H1M only).
- **`SLT`/`VNPU` Board Types**: Shown in device information and CLI output.
- **Windows Release Metadata**: Version metadata in Windows binaries, with Authenticode signing guide.
- **E2E Response Time API**: `GetE2EResponseTime()` and related APIs, including data transfer and overhead.
- **E2E Response Time Output**: Shown in `dxrun --verbose` (with Queue Wait Time) and the profiler summary (with Latency Average).
- **Core Latency API**: `GetCoreLatency*` (C++), `dxrt_engine_get_core_latency*` (C), `get_core_latency*` (Python).
- **Device Removed Error**: C API `DXRT_ERR_DEVICE_REMOVED` (-10), C++ `ERROR_CODE::DEVICE_REMOVED`, and `DeviceRemovedException`.
- **C# Usage Guide**: Using the C API from C# (`examples/csharp/README.md`).

---

## DX-RT v3.4.2 / 2026-08-14

### 1. Changed
- **MSVC Build Log Cleanup**: Improved the MSVC build log by resolving Windows build warnings in the C++ CLI code
- **Vendored Header Staging on Windows**: Made external (vendored) header staging on Windows idempotent regardless of Developer Mode state, preventing "existing path cannot be removed" errors and stale empty-directory failures on reconfigure.
- **Ninja Generator for Windows Wheel Builds**: Forced the Ninja generator for Python wheel builds on Windows to avoid Visual Studio generator compiler auto-detection failures (No CMAKE_C_COMPILER could be found) inside an activated vcvars shell.

### 2. Fixed
- Staged the generated `gen.h` into the public include path (with a copy fallback when symlinks are unavailable on Windows), fixing downstream "cannot open include file" errors.
- Documentation improvements: unified OS requirements (Ubuntu 20.04+, Debian 12+), corrected typos, added missing CLI examples, fixed grammar errors across installation and tutorial guides

### 3. Added
- **Windows Release Version Metadata**: Added `release.ver` based version information to the Windows DXRT executable and DLLs
- **Vendored Header Copy Fallback**: Added a recursive copy fallback for vendored headers (cxxopts, rapidjson) on Windows when directory symlinks are unavailable (Developer Mode/admin disabled).

---

## DX-RT v3.4.1 / 2026-08-03

This is a documentation-only hotfix. The `libdxrt` and `libdxrt-bin` Debian packages and `dx_engine` Python wheels are unchanged and remain at v3.4.0.

### 1. Changed
- None

### 2. Fixed
- Corrected supported OS list in installation guide: removed Ubuntu 18.04 (EOL), added Ubuntu 26.04
- Clarified ONNX Runtime version recommendation wording in installation guide (`02_Installation_on_Linux.md`)
- Removed stray emoji characters from C++ and Python tutorial pages

### 3. Added
- Added bundled wheel installation guide (Option B) for virtual environments in installation guide
- Added info note documenting `/usr/share/libdxrt-bin/python/` bundled wheel directory usage

---

## DX-RT v3.4.0 / 2026-07-22

### 1. Changed
- Debian package bundled `dx_engine` Python wheels for Python 3.8 – 3.14.
- CLI binary names updated for consistency: `dxrt-cli` → `dxcli`, `parse_model` → `dxparse`, `run_model` → `dxrun`
- Old names (`dxrt-cli`, `parse_model`, `run_model`) remain functional as backward-compatible aliases (symlinks on Linux/macOS, hard links on Windows)
- Python bindings now use dxrt_cxx_api.h (header-only) instead of direct C++ linkage
- libdxrt.so exports C symbols only (dxrt_*); internal C++ symbols hidden via version script
- Updated minimum versions
   - Driver : 2.4.0 -> 2.5.0
   - PCIe Driver : 2.2.0 -> 2.4.0
   - Firmware : 2.5.2 -> 2.7.0
- Code style cleanup across Python package (`inference_option.py`, `runtime_event_dispatcher.py`, `utils.py`, etc.)
- Upgraded extern/include/dxrt/extern/cxxopts.hpp from v3.1.1 to v3.3.1
- Increased firmware update sequence wait time from 2s to 4s.
- Replaced top-level matplotlib imports with lazy loading via `ensure_dependencies()` in `plot.py` to provide install guidance instead of ImportError

### 2. Fixed
- Improved shared-memory performance.
- Fixed SEGV in NFHLayer caused by dereferencing uninitialized `Request::_task` in profiler instrumentation
- Returned `INT16_MIN` instead of `0` from `DeviceStatus::GetTemperature()` when channel index is out of range
- Improved dxrtd error messages when manually executed
- Fixed missing separator between `devices` and `buffer_count` fields in `InferenceOption.__repr__` output
- Updated ppcpu logic for hardness
- Fixed a critical crash (nullptr) in pyRunBenchmark by ensuring the memory lifetime of input buffers.
- Improved temperature range validation in dxtop.
- Fixed multi-input dictionary handling issue.

### 3. Added
- Script to build `dx_engine` wheels per Python version.
- Documentation describing the Debian prebuilt directory structure.
- Script to generate the `prebuilt/` directory.
- Stable C ABI (dxrt_c_api.h, 103 functions) enabling prebuilt SDK distribution without recompilation
- Header-only C++ wrapper (dxrt_cxx_api.h) for single-include modern C++ usage
- SDK-compat bridge headers (legacy/) for backward compatibility with existing dxrt_api.h code
- H1M firmware compatibility check: distinguish H1M (LPDDR4) from H1 (LPDDR5/LPDDR5X) in firmware update and device detection; support mixed 4-pack / 6-pack H1M configurations
- Deprecated Python API migration guide added to Python API Reference documentation
- Added shared memory-based inter-process communication using memfd
- Added dynamic IPC infrastructure with packet-based protocol layer
- Added cross-platform support for Linux and Windows IPC transport
- Added new device monitoring APIs (memory usage, per-core temperature, and utilization)
- Added a tool (plot_html.py) to generate HTML visualization reports for profiling data (profiler.json)
- Profiler
  - Added GetJobMetrics() API for comprehensive per-job profiling
  - Enhanced performance statistics with Coefficient of Variation (CoV) metric
- Added input dtype validation to prevent undefined behavior caused by mismatching NumPy array types.
- Added `libdxrt-bin` Debian packaging for pre-built binaries with amd64/arm64 auto-detection.

---

## DX-RT v3.3.2 / 2026-05-11

### 1. Changed
- Removed redundant build artifacts and temporary directories from the Debian package.

### 2. Fixed
- Improved Python extension module linking for _pydxrt build.

### 3. Added
- Added conditional pip upgrade (v21.3+) to ensure build stability on legacy OS environments.

---

## DX-RT v3.3.1 / 2026-05-06

### 1. Changed
- Changed the version of pre-built onnxruntime(1.23.2 -> 1.22.0) and openvino(25.4 -> 25.1)
- Enhanced uninstall script to remove debian packages, legacy binaries, and test files from previous versions

### 2. Fixed
- Fixed typos in the document
- Fixed error in uninstall logic

### 3. Added
- Added libdxrt 3.3.1 debian package with updated build and install pipeline

---

## DX-RT v3.3.0 / 2026-04-10

### 1. Changed
- Updated minimum versions
   - Driver : 1.8.0 -> 2.4.0
   - PCIe Driver : 1.5.1 -> 2.2.0
   - Firmware : 2.4.0 -> 2.5.2
- Updated the Python module version to match the C++ Runtime version.

### 2. Fixed
- Fixed PPU data transfer error during multi-process execution in H1 and Multiple M1 M.2 environments.
- Fixed input data lifecycle issues in the Python Runtime Module.
- Fixed intermittent interrupt exceptions in IPC Message Queue.

### 3. Added
- Added dxtop for No Service Mode
- Added an example that loads an entire model file into a memory buffer and performs inference directly using this memory buffer.
- Added python InferenceEngine from numpy array
- Added acceleration features for CPU operations (Requires separate option configuration and build)

---

## DX-RT v3.2.0 / 2026-01-16

### 1. Changed
- Optimized PCIe DMA sequence for better performance.
- Updated OS requirements in installation guide for debian

### 2. Fixed
- Optimized device memory footprint of PPU models

### 3. Added
- Added RuntimeEventDispatcher class for C++
   - RuntimeEventDispatcher is a singleton class that provides centralized event dispatching and handling for runtime events from the DX-RT system, such as device errors, warnings, and notifications.
- Added Python wrapper for RuntimeEventDispatcher class -> dx_engine 1.1.4
- Added `--profiler` option to enable profiling mode in run_model.py
- Added `--buffer-count` option to configure inference buffer count in run_model.py
- Added build.sh options
```
  --use_service_on  Enable the use of the service in the build.
  --use_service_off Disable the use of the service in the build.
  --use_ort_on      Enable the use of the ORT component in the build.
  --use_ort_off     Disable the use of the ORT component in the build.
```
- Added `__version__` import to the main `dx_engine` module namespace
- Implemented per-instance configuration of Input and Output buffer counts for the Inference Engine.
- Enabled direct loading of the .dxnn model format from a memory buffer within the Inference Engine.
- Added RuntimeEventDispatcher for centralized event handling and logging.

---

## DX-RT v3.1.0 / 2025-11-28

### 1. Changed
- Updated model file format version checks to include max version…
- Updated minimum versions
  - Driver : 1.7.1 -> 1.8.0
  - PCIe Driver : 1.4.1 -> 1.5.1
  - Firmware : 2.1.0 -> 2.4.0
- Updated Sanity Check to hide ONNX Runtime version when built with USE_ORT=OFF.
- Standardized all Python and C++ examples to use argparse and cxxopts for consistent command-line argument parsing
- Updated all examples to support unified argument format
    - -m, --model for .dxnn model file path
    - -l, --loops for inference loop count
    - Additional options like verbose follow similar format conventions per example
- Updated dxbenchmark default behavior to execute 30 loops when neither loop nor time options are specified
- Enhanced dxbenchmark to automatically create result directory if it does not exist when result path is specified
- Added a system requirement check in install.sh (RAM: 8GB, Arch: x86_64 or aarch64)
- Removed the check for the libdxrt.so location in Sanity Check
- Improved parse_model CLI tool.
- Removed dsp related code
- Updated the .dxnn file format to version 7 (from v6).
- Updated C++ exception handling to translate exceptions into Python for improved error handling.
- Updated the Python v6_converter with enhanced functionality.
- Updated license information
- Enhanced OS and architecture checks in installation scripts
- Updated user guide document
- Profiler now groups events by base name (before ) instead of showing individual job/request entries
- Limited duration details to 30 values per group for cleaner output

### 2. Fixed
- Used 'python3 -m pip' instead of 'pip' for better reliability
- Fixed some compile errors and warnings in windows environment
- Fixed configuration option name in common.cfg.
- Updated cross compile script for dxtop
- Fixed several multi-tasking bugs related to CPU offloading buffer management and PPU output buffer mis-pointing.
- Fixed a bug in the process of setting the PPU model format and layout.
- Fixed a critical bug affecting models with multi-output and multi-tail configurations.
- Fixed tensor mapping errors that occurred in non-ORT inference mode.
- Fixed a warning message in get_output_tensors_info and a vector access bug in _npuModel.
- Fixed an issue that prevented error messages from being displayed.
- Fixed flaws in output tensor mapping and memory address configuration.
- Updated documentation to reflect changes in supported CPU architecture and OS requirements.
- Force-disabled with a warning instead of throwing a runtime exception in builds that don't support USE_ORT.
- Fixed run_model error when -f option and -l loop count exceeds 1024
- Fixed bounding issue on service

### 3. Added
- Loaded PPCPU Firmware on service initialization
- (in NO_SERVICE mode, it is loaded with first inferenceengine initialization
- Included binary data as Array on source code, comes with internal-only generation script
- Added support for V8 DXNN file format
- Added PPU support for V8 models
- Supported dynamic shape output of tail cpu task
- Implemented asynchronous NPU Format Handler (NFH).
- Added new functions to profiler (Flush and GetPerformanceData)
- Added dxbenchmark, a command-line tool for comparing performance metrics across multiple models and generating detailed
- model voltage profiler (run_model_prof.py)
  - requires firmware > 2.2.0 and driver > 1.7.1
- Added a new internal C++ converter for v6 models.
- Added new Python APIs for handling device configuration and status retrieval.
- Enhanced build and uninstall scripts with common utilities and improved logging
  - Integrated common utility functions into build.sh for better modularity.
  - Added uninstall.sh script to handle project uninstallation, including cleanup of symlinks and directories.
  - Improved logging in both scripts using color-coded messages for better user feedback.
  - Updated color_env.sh and common_util.sh to support new logging features and ensure consistent output formatting.
  - Refactored build.sh to streamline the build process and enhance error handling.
- Added PCIe bus number display for dxtop
- Added profiling data memory usage tracking with high usage warnings.
- Added time-base inference mode to run_model (-t, --time option)
- Added error handling for invalid firmware files and update conditions.
- Added a function to check Python version compatibility in build.sh.
- Added new documentation files for Inference API, Multi-Input Inference, and Global Instance.
- Added examples for asynchronous model inference with profiling capabilities in both C++ and Python.

---

## DX-RT v3.0.0 / 2025-09-08  

### 1. Changed
- Updated minimum versions 
   - Driver : 1.5.0 -> 1.7.1
   - PCIe Driver : 1.4.0 -> 1.4.1
   - Firmware : 2.0.5 -> 2.1.0
- Updated DeviceOutputWorker to use 4 threads for 4 DMA channels (3 channels to 4 channels)
- Updated Python Package version (v1.1.1 -> v1.1.2)
- Modified run_async_model and run_async_model_output examples
- Modified build.sh (print python package install info)
- Removed some unnecessary items from header files
- Used Pyproject.toml instead setup.py (now setup.py is not recommended)
- Added options to SanityCheck.sh
   - Usage: sudo SanityCheck.sh [all(default) | dx_rt | dx_driver | help]
- Updated the build compiler to version 14 for both USE_ORT=ON and USE_ORT=OFF configurations.
- Modified run_model logging to include host info (Linux only).
- Enhanced UI for better clarity, enabled dynamic data rendering, and added visual graphs for NPU Memory usage.
- Changed default build option for DX-RT from USE_ORT=OFF to USE_ORT=ON. If the inference engine option is not specified separately, use_ort will be enabled by default, activating the CPU task for .dxnn models.
- Added automatic handling of input dummy padding and output dummy slicing when USE_ORT=OFF (build-time or via InferenceOption). Applications no longer need to add input dummy data or remove output dummy data for inference.
  
### 2. Fixed  
- Fixed kernel panic issue caused by wrong NPU channel number
- Improved error message readability in install, build scripts
  - Applied color to error messages
  - Reordered message output to display errors before help messages
- Fixed some rapidjson issue from clients.
- Removed bad using namespace std from model.h (some programs need change)
- Fixed an issue where temporary files from the ONNX Runtime installation would accumulate.
- Fixed a cross-compilation error related to the ncurses library for the dxtop utility.
- Updated code for compatibility with v3 environment
- Fixed dx-rt build error caused by pybind11 incompatibility with Python 3.6.9 on Ubuntu 18.04
  - Supported automatic installation of minimum required Python version (>= 3.8.2)  
  - Installed Python 3.8.2 if the system Python version is not supported
  - On Ubuntu 18.04, installed via source build; on Ubuntu 20.04+, used apt install
  - Added support in install.sh to optionally accept --python_version and --venv_path for installation
  - Added support in build.sh to accept and use --python_exec
  - Added support in build.sh to optionally accept --venv_path and activate the specified virtual environment

### 3. Added  
- Added usb inference module (tcp/ip)
(MACRO : DXRT_USB_NETWORK_DRIVER)
- Added Sanity Check Features
   - Dependency version check.
   - Executable file check.
- Added APIs to the Configuration class for retrieving version information.
- PCIE details displayed on some device errors
- dxrt-cli --errorstat option added (this shows pcie detailed information)
- Added Python examples for configuration and device status.
- Added Python API for configuration and device status. (dx-engine-1.1.1)
- Added functionality to query the framework & driver versions in the Configuration class.
- Added weight checksum info for service
- Added ENABLE_SHOW_MODEL_INFO build option and configuration item
- Added dxtop tool, a terminal-based monitoring tool for Linux environments. It provides real-time insights into NPU core utilization and DRAM usage per NPU device.
- Added support for both .dxnn file formats: v6 (compiled with dx_com 1.40.2 or later) and v7 (compiled with dx_com 2.x.x).

---

## DX-RT v2.9.5 / 2025-07-23  

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
- Added display_async_models examples

---

## DX-RT v2.8.4 / 2025-05-12

### 1. Changed
- Modified the build.sh script according to cmake options
  - CMake option USE_ORT=ON, running build.sh --clean installs ONNX Runtime.
  - CMake option USE_PYTHON=ON, running build.sh installs the Python package.
  - CMake option USE_SERVICE=ON, running build.sh starts or restarts the service.
- Improved callback handling by removing std::async, potentially leading to more predictable execution
- Enhanced concurrency by making key variables atomic, resolving potential race conditions.
- Addressed multithreading issues by implementing additional locks, improving stability under heavy load.
- Removed obsolete code, streamlining the codebase

### 2. Fixed
  - Fixed crash on multi-device environment with more than 2 H1 cards(>=8 devices)
  - Resolved data corruption errors that could occur in different scenarios, ensuring data integrity.
  - Fixed profiler bugs
  - Addressed issues identified by static analysis and other tools, enhancing code quality and reliability.

### 3. Added
- Added a USE_ORT option for Python Bitmatch.py
- Added --use_ort flag to the run_model.py example for ONNX Runtime
 - Implemented profiler on/off functionality (by Configuration)
 -  Implemented a check to prevent tasks from being started multiple times, ensuring correct execution flow.
 - Implemented device blocking device on error
 - Implemented page alignment for buffers to address some I/O issues.

---

## DX-RT v2.8.3 / 2025-04-11

### 1. Changed
- Improved driver, firmware, and file format version check messages

### 2. Fixed
- None

### 3. Added
- Added --all option to build.sh

---

## DX-RT v2.8.2 / 2025-03-21

### 1. Changed
- Modified the run_async_model_output example to improve the passing condition.
- Modified Inference Engine to be used with 'with' statements, and updated relevant examples.

### 2. Fixed
- Failed to read output -70 bug

### 3. Added
- Round Robin,Shortest Job First scheduler added
- Implemented C++ Run (Batch) function within the InferenceEngine for batched inference execution.
- Added a new example, run_batch_model, demonstrating the usage of the batch inference function.
- Displayed the memory usage of the loaded model.
- Added Python inference option interface with the following configurations
   * NPU Device Selection / NPU Bound Option / ORT Usage Flag

---

## DX-RT v2.7.1 / 2025-03-12
### 1. Changed
- Displayed dxnn versions in parse_model (.dxnn file format version & compiler version)
### 2. Fixed
- None
### 3. Added
- Added otp read / write api (internal only)

---

## DX-RT v2.7.1 / 2025-03-11
### 1. Changed
- Added instructions on how to retrieve device status information
- Driver and Firmware versions
  - RT Driver >= v1.3.3
  - Firmware >= v1.6.3
### 2. Fixed
- Included batch size in PPU output shape in Python API
### 3. Added
- Implemented retrieval of device status information by device ID
- Retrieved the count of installed devices
- Added non-contiguous input handling in Python API

---

## DX-RT v2.7.0 / 2025-02-25
### 1. Changed
- Renamed APIs
- Optimized sync timing in asynchronous inference scenario
- Updated minimum DX-COM version to >= 1.40.2
- Updated minimum onnxruntime version to >= 1.20.1
### 2. Fixed
- Troubleshooting abnormal process terminations
- Multi process termination bug
- Stabilization on Windows operating systems
- Restricted multiple services from running
### 3. Added
- Added Configuration
- Added Dynamic CPU task multi threading
- Added Statistics profiler
- Added Clang compiler
- Added Average load on NPU devices and CPU tasks

---

## DX-RT v2.6.3 / 2025-01-06
### 1. Changed
- Separated msg queue for Send To / Receive From
- Merged windows code & modified bitmatch (C++)
### 2. Fixed
- Fixed NPU memory leaks
### 3. Added
- None

---

## DX-RT v2.6.2 / 2024-12-19
### 1. Changed
- None
### 2. Fixed
- Fixed free output buffer locking issue for multi-threaded runAsync
### 3. Added
- Modified configuration function for throttling using json

---

## DX-RT v2.6.1 / 2024-12-11
### 1. Changed
- None
### 2. Fixed
- Fixed multi-device performance error
- Fixed issue with running python run_model due to API not being updated.
### 3. Added
- None

---

## DX-RT v2.6.0 / 2024-12-10
### 1. Changed
- Modified inference load control
- onnxruntime minimum version : v1.18.0
- Updated python version : v1.0.0
- Driver and Firmware versions
  - RT Driver  >= v1.3.0
  - PCIe Driver  >= v1.2.0
  - Firmware  >= v1.5.9
### 2. Fixed
- Fixed a problem that did not work when using user memory in conjunction with the inference engine
- Fixed profiler memory corruption issue
- Fixed multi-device performance issue
### 3. Added
- Added NPU memory caching

---

## DX-RT v2.1.0 / 2024-10-31
### 1. Changed
- run_model async mode as default  
- Driver and Firmware versions
  - RT Driver  >= v1.1.0
  - PCIe Driver  >= v1.1.0
  - Firmware  >= v1.5.5
### 2. Fixed
-
### 3. Added
- Supported multi-process & multi-device
  - dxrtd daemon
- Supported Python Interface (Run & RunAsync)
  - Async mode / Batch mode
- Device status monitoring function via cli-command

---

## DX-RT v2.0.3 / 2024-09-04
### 1. Changed
- Changed the alignment from 64 to 16
### 2. Fixed
- Removed cross compile package for non-x64 environment
### 3. Added
- Added firmware upload mode on cli
- Supported INT64 for cpu onnx

---

## DX-RT v2.0.1 / 2024-08-06
### 1. Changed
- None
### 2. Fixed
- Fixed argmax model w/ empty output
### 3. Added
- None

---

## DX-RT v2.0.0 / 2024-08-02
### 1. Changed
- Updated the .dxnn file format to version 6.
### 2. Fixed
- None
### 3. Added
- Added stress test script
- Added batch run async in pybinding

---

## DX-RT v1.2.3 / 2024-07-23
### 1. Changed
- Updated process id & model_format for device message
- Removed device dependency for parse_model
### 2. Fixed
- Fixed memory leak problem
- Fixed FindPythonInterp error after cmake 3.27
- Fixed ppu output bug
### 3. Added
- Implemented multi-task and multi in/out for achieving CPU offloading level 1

---

## DX-RT v1.1.2 / 2024-07-03
### 1. Changed
- Updated documents
### 2. Fixed
- None
### 3. Added
- None

---

## DX-RT v1.1.1 / 2024-06-03
### 1. Changed
- Simplified dxrt-cli status message
- Removed unnecessary outputs by dx_rt library
- Changed arch option: arm64->aarch64
### 2. Fixed
- Fixed cross-compile issue: cross compile occurs on aarch64 issue
- Fixed model memory check logic
### 3. Added
- None

---

## DX-RT v1.0.1 / 2024-05-23
### 1. Changed
- Changed the make option from -j8 to -j$(nproc)
### 2. Fixed
- Fixed library install path issue: some files installed under /cmake/dxrt
### 3. Added
- None

---

## DX-RT v1.0.0 / 2024-04-29
### 1. Changed
- DXNN Version2 architecture sdk
- Removed driver folder
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
- Supported model configuration for real time face recognition demo
- Supported receiving device information and dumping device memory from commandline interface application

---

## DX-RT v0.5.3 / 2023-06-14
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Supported DX-H1 ASIC
- Added firmware CLI tool for DX-M1, DX-H1
- Improved YOLOX postproc. performance

---

## DX-RT v0.5.2 / 2023-05-10
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Supported DX-M1 ASIC
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
- Expanded model image size parameter for yolo
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
- Refactored device parameters in runtime lib.
### 2. Fixed
- None
### 3. Added
- Added ISP interface for object detection demo
- Added ONNX runtime interface for CPU task (verified only x86_64)

---

## DX-RT v0.3.1 / 2022-12-12
### 1. Changed
- Improved object detection pre/post parameter
- Unified post-processing for yolov5, yolox
### 2. Fixed
- None
### 3. Added
- Supported documents generation

---

## DX-RT v0.3.0 / 2022-12-05
### 1. Changed
- Refactored face recognition application
- Changed build architecture for auto-release
- Improved docs.
### 2. Fixed
- None
### 3. Added
- Added parse_model application

---

## DX-RT v0.2.0 / 2022-11-22
### 1. Changed
- Separated dev-build, release-build
### 2. Fixed
- None
### 3. Added
- Added Common framework for devices
- Supported OpenCV for riscv64, arm64
- Supported documentation as markdown format
- Added doxygen for API reference
- Supported encrypted NPU parameters

---

## DX-RT v0.1.0 / 2022-06-30
- Initial release for DX-L1 (eyenix FPGA)
