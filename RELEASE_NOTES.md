# RELEASE_NOTES

## v2.9.0 / 2025-05-19
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
### 2. Fixed
- Fixed incorrect 'dtype' in Python tensor info functions. Corrected the 'dtype' reported by get_input_tensors_info(), get_output_tensors_info(), and similar functions.
- None
### 3. Added
- Improved usability for python InferenceOption(). Users can now directly set the option variable without needing a separate method.
- Improved the Python API
  - InferenceOption is now supported identically to the C++ API.
  - Callback functions registered via register_callback now accept user_arg of custom types.
  - run() now supports both single-input and batch-input modes, depending on the input format.
- Add display_async_models examples
- None
## v2.8.4 / 2025-05-12
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

## v2.8.3 / 2025-04-11
### 1. Changed
- Improve driver, firmware, and file format version check messages
### 2. Fixed
- None
### 3. Added
- Add --all option to build.sh

## v2.8.2 / 2025-03-21
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

## v2.7.1 / 2025-03-12
### 1. Changed
- display dxnn versions in parse_model (.dxnn file format version & compiler version)
### 2. Fixed
- None
### 3. Added
- Add otp read / write api (internal only)
## v2.7.1 / 2025-03-11
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
## v2.7.0 / 2025-02-25
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

## v2.6.3 / 2025-01-06
### Changed
- seperate msg queue for Send To / Receive From
- merge windows code & modify bitmatch (C++)
### Fixed
- fix NPU memory leaks
### Added
- NONE

## v2.6.2 / 2024-12-19
### Changed
- NONE
### Fixed
- Fix free output buffer locking issue for multi-threaded runAsync
### Added
- Modify configuration function for throttling using json

## v2.6.1 / 2024-12-11
### Changed
- NONE
### Fixed
- Fix multi-device performance error
- Fix issue with running python run_model due to API not being updated.
### Added
- NONE

## v2.6.0 / 2024-12-10
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

## v2.1.0 / 2024-10-31
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

## v2.0.3 / 2024-09-04
### Changed
- align change: 64 to 16
### Fixed
- remove cross compile package for non-x64 environment
### Added
- add firmware upload mode on cli
- support INT64 for cpu onnx

## v2.0.1 / 2024-08-06
### Changed
- None
### Fixed
- Fix argmax model w/ empty output
### Added
- None

## v2.0.0 / 2024-08-02
### Changed
- dxnn version up(v6). so prior dxnn models will not work from this version.
### Fixed
- None
### Added
- stress test script
- batch run async in pybinding

## v1.2.3 / 2024-07-23
### Changed
- Update process id & model_format for device message
- Remove device dependency for parse_model
### Fixed
- Fix memory leck problem
- Fix FindPythonInterp error after cmake 3.27
- Fix ppu output bug
### Added
- Implement multi-task and multi in/out for achieving CPU offloading level 1

## v1.1.2 / 2024-07-03
### Changed
- update documents
### Fixed
- None
### Added
- None

## v1.1.1 / 2024-06-03
### 1. Changed
- simplify dxrt-cli status message
- remove unnesessary outputs by dx_rt library
- change arch option: arm64->aarch64
### 2. Fixed
- fix cross-compile issue: cross compile occurs on aarch64 issue
- fix model memory check logic
### 3. Added
- None

## v1.0.1 / 2024-05-23
### 1. Changed
- make option : -j8 -> -j$(nproc)
### 2. Fixed
- fix library install path issue: some files installed under /cmake/dxrt
### 3. Added
- None


## v1.0.0 / 2024-04-29
### 1. Changed
- DXNN Version2 architecture sdk
- Remove driver folder
  Please refer to "dx_rt_npu_linux_driver".
### 2. Fixed
- None
### 3. Added
- Pybind11 support
  DXRT supports some Python APIs.

## v0.5.4 / 2023-07-17
### 1. Changed
- Changed classification demo name
### 2. Fixed
- None
### 3. Added
- Support model configuration for real time face recognition demo
- Support to receive device information and dump device memory from commandline interface application

## v0.5.3 / 2023-06-14
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Support DX-H1 ASIC
- Added firmware CLI tool for DX-M1, DX-H1
- Improve YOLOX postproc. performance

## v0.5.2 / 2023-05-10
### 1. Changed
- None
### 2. Fixed
- None
### 3. Added
- Support DX-M1 ASIC
- Added PCIe driver build environment for DX-M1
- Added pose estimation application
- Added FPS estimation for run_model application

## v0.5.1 / 2023-04-03
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

## v0.5.0 / 2023-02-22
### 1. Changed
- Device variant/type setting is removed from build script (Device auto-detection is applied).
- Reduced interrupt latency for standalone device
### 2. Fixed
- None
### 3. Added
- Added yolov7 configurations in object detection app.
- Added PCIe driver (only for DX-M1 FPGA)

## v0.4.0 / 2023-01-05
### 1. Changed
- Refactor device parameters in runtime lib.
### 2. Fixed
- None
### 3. Added
- Add ISP interface for object detection demo
- Add ONNX runtime interface for CPU task (verified only x86_64)

## v0.3.1 / 2022-12-12
### 1. Changed
- Improve object detection pre/post parameter
- Unified post-processing for yolov5, yolox
### 2. Fixed
- None
### 3. Added
- Support documents generation

## v0.3.0 / 2022-12-05
### 1. Changed
- Refactor face recognition application
- Change build architecture for auto-release
- Improve docs.
### 2. Fixed
- None
### 3. Added
- Add parse_model application

## v0.2.0 / 2022-11-22
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

## v0.1.0 / 2022-06-30
- Initial release for DX-L1 (eyenix FPGA)
