## PR 593 NOTHING NEW
## PR 592
### 1. Changed
- Update minimum versions 
   - Driver : 1.5.0 -> 1.7.1
   - PCIe Driver : 1.4.0 -> 1.4.1
   - Firmware : 2.0.5 -> 2.1.0
### 2. Fixed
### 3. Added
## PR 591 NOTHING NEW
## PR 589
### 1. Changed
### 2. Fixed
- fix kernel panic issue caused by wrong NPU channel number
### 3. Added
## PR 588 NOTHING NEW
## PR 587 NOTHING NEW
## PR 586
### 1. Changed
- Update DeviceOutputWorker to use 4 threads for 4 DMA channels (3 channels to 4 channels)
### 2. Fixed
### 3. Added
## PR 582
### 1. Changed
### 2. Fixed
- feat: Improve error message readability in install, build scripts (TFT-101)[https://deepx.atlassian.net/browse/TFT-101]
  - Apply color to error messages
  - Reorder message output to display errors before help messages
### 3. Added
## PR 584 NOTHING NEW
## PR 583 NOTHING NEW
## PR 581 NOTHING NEW
## PR 580 NOTHING NEW
## PR 577 NOTHING NEW
## PR 578 NOTHING NEW
## PR 576 NOTHING NEW
## PR 574
### 1. Changed
- Update Python Package version (v1.1.1 -> v1.1.2)
- Modify run_async_model and run_async_model_output examples
- Modify build.sh (print python package install info)
### 2. Fixed
### 3. Added
## PR 573 NOTHING NEW
## PR 571
### 1. Changed
- removed some unnecessary items from header files
- use Pyproject.toml instead setup.py (now setup.py is not recommended)
### 2. Fixed
- fix some rapidjson issue from clients.
- remove bad using namespace std from model.h (some programs need change)
### 3. Added
## PR 570 NOTHING NEW
## PR 569
### 1. Changed
### 2. Fixed
### 3. Added
- Add usb inference module (tcp/ip)
(MACRO : DXRT_USB_NETWORK_DRIVER)
## PR 567 NOTHING NEW
## PR 538 NOTHING NEW
## PR 563 NOTHING NEW
## PR 551
### 1. Changed
- Add options to SanityCheck.sh
   - Usage: sudo SanityCheck.sh [all(default) | dx_rt | dx_driver | help]
### 2. Fixed
### 3. Added
## PR 547
### 1. Changed
-  Change build compiler has been updated to version 14 for both USE_ORT=ON and USE_ORT=OFF configurations.
### 2. Fixed
- Fix an issue where temporary files from the ONNX Runtime installation would accumulate.
- Fix a cross-compilation error related to the ncurses library for the dxtop utility.
### 3. Added
- Add Sanity Check Features
   - Dependency version check.
   - Executable file check.
- Add APIs to the Configuration class for retrieving version information.

## PR 539 NOTHING NEW
## PR 535 NOTHING NEW
## PR 537
### 1. Changed
### 2. Fixed
### 3. Added
- PCIE details displayed on some device errors
- dxrt-cli --errorstat option added (this shows pcie detailed information)
## PR 536
### 1. Changed
- Modify run_model logging to include host info (Linux only).
### 2. Fixed
### 3. Added
- Add Python examples for configuration and device status.
- Add Python API for configuration and device status. (dx-engine-1.1.1)
- Add functionality to query the framework & driver versions in the Configuration class.
## PR 534
### 1. Changed
### 2. Fixed
### 3. Added
- Add weight checksum info for service
## PR 533
### 1. Changed
### 2. Fixed
### 3. Added
- Add ENABLE_SHOW_MODEL_INFO build option and configuration item
## PR 532
### 1. Changed
### 2. Fixed
- Update code for compatibility with v3 environment 
### 3. Added
## PR 531
### 1. Changed
- Enhance UI for better clarity, enabled dynamic data rendering, and added visual graphs for NPU Memory usage.
### 2. Fixed
### 3. Added
## PR 521 NOTHING NEW
## PR 529 NOTHING NEW
## PR 524
### 1. Changed
### 2. Fixed
- fix: fix dx-rt build error caused by pybind11 incompatibility with Python 3.6.9 on Ubuntu 18.04 [TFT-82](https://deepx.atlassian.net/browse/TFT-82)
  - Support automatic installation of minimum required Python version (>= 3.8.2)  
  - Install Python 3.8.2 if the system Python version is not supported
  - On Ubuntu 18.04, install via source build; on Ubuntu 20.04+, use apt install
  - Added support in install.sh to optionally accept --python_version and --venv_path for installation
  - Added support in build.sh to accept and use --python_exec
  - Added support in build.sh to optionally accept --venv_path and activate the specified virtual environment

### 3. Added

## PR 528 NOTHING NEW
## PR 520 NOTHING NEW
## PR 519 NOTHING NEW
## PR 513
### 1. Changed
- The default build option for DX-RT has been changed from USE_ORT=OFF to USE_ORT=ON. If the inference engine option is not specified separately, use_ort will be enabled by default, activating the CPU task for .dxnn models.
### 2. Fixed
### 3. Added
## PR 517 NOTHING NEW
## PR 518 NOTHING NEW
## PR 512
### 1. Changed
### 2. Fixed
### 3. Added
- Add dxtop tool, a terminal-based monitoring tool for Linux environments. It provides real-time insights into NPU core utilization and DRAM usage per NPU device.
## PR 516 NOTHING NEW
## PR 515 NOTHING NEW
## PR 514 NOTHING NEW
## PR 511 NOTHING NEW
## PR 510 NOTHING NEW
