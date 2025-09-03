## PR 633
### 1. Changed
- Update the .dxnn file format to version 7 (from v6).
- Update C++ exception handling to translate exceptions into Python for improved error handling.
- Update the Python v6_converter with enhanced functionality.
### 2. Fixed
- Fix several multi-tasking bugs related to CPU offloading buffer management and PPU output buffer mis-pointing.
- Fix a bug in the process of setting the PPU model format and layout.
- Fix a critical bug affecting models with multi-output and multi-tail configurations.
- Fix tensor mapping errors that occurred in non-ORT inference mode.
- Fix a warning message in get_output_tensors_info and a vector access bug in _npuModel.
- Fix an issue that prevented error messages from being displayed.
- Fix flaws in output tensor mapping and memory address configuration.
### 3. Added
- Add a new internal C++ converter for v6 models.
- Add new Python APIs for handling device configuration and status retrieval.
## PR 632
### 1. Changed
- Update license information
### 2. Fixed
### 3. Added
## PR 623
### 1. Changed
- feat: enhance OS and architecture checks in installation scripts [CSP-717](https://deepx.atlassian.net/browse/CSP-717)
### 2. Fixed
- docs: Updated documentation to reflect changes in supported CPU architecture and OS requirements. [CSP-686](https://deepx.atlassian.net/browse/CSP-686)
### 3. Added
- feat: enhance build and uninstall scripts with common utilities and improved logging [CSP-700](https://deepx.atlassian.net/browse/CSP-700)
  - Integrated common utility functions into build.sh for better modularity.
  - Added uninstall.sh script to handle project uninstallation, including cleanup of symlinks and directories.
  - Improved logging in both scripts using color-coded messages for better user feedback.
  - Updated color_env.sh and common_util.sh to support new logging features and ensure consistent output formatting.
  - Refactored build.sh to streamline the build process and enhance error handling.

## PR 629 NOTHING NEW
## PR 628 NOTHING NEW
## PR 625 NOTHING NEW
## PR 624
### 1. Changed
### 2. Fixed
### 3. Added
- Added PCIe bus number display for dxtop
## PR 622 NOTHING NEW
## PR 619 NOTHING NEW
## PR 621
### 1. Changed
### 2. Fixed
### 3. Added
- Add profiling data memory usage tracking with high usage warnings.
## PR 620 NOTHING NEW
## PR 618 NOTHING NEW
## PR 616
### 1. Changed
- Update user guide document
### 2. Fixed
### 3. Added
## PR 613
### 1. Changed
### 2. Fixed
- Force-disabled with a warning instead of throwing a runtime exception in builds that don't support USE_ORT.
### 3. Added
## PR 612 NOTHING NEW
## PR 611
### 1. Changed
### 2. Fixed
### 3. Added
- Add time-base inference mode to run_model (-t, --time option)
## PR 603
### 1. Changed
- Profiler now groups events by base name (before ) instead of showing individual job/request entries
- Limited duration details to 30 values per group for cleaner output
### 2. Fixed
### 3. Added
## PR 604
### 1. Changed
### 2. Fixed
- fix run_model error when -f option and -l loop count exceeds 1024
### 3. Added
## PR 602
### 1. Changed
### 2. Fixed
- Fix bounding issue on service
### 3. Added
## PR 601
### 1. Changed
### 2. Fixed
### 3. Added
- Add error handling for invalid firmware files and update conditions.
## PR 600
### 1. Changed
### 2. Fixed
### 3. Added
- Add a function to check Python version compatibility in build.sh.
- Add new documentation files for Inference API, Multi-Input Inference, and Global Instance.
- Add examples for asynchronous model inference with profiling capabilities in both C++ and Python.
