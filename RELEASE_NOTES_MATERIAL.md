## PR 719 NOTHING NEW
## PR 718 NOTHING NEW
## PR 717 NOTHING NEW
## PR 716 NOTHING NEW
## PR 712
### 1. Changed
### 2. Fixed
### 3. Added
- Loading PPCPU Firmware on service initialization
- (in NO_SERVICE mode, it is loaded with first inferenceengine initialization
- Include binary data as Array on source code, comes with internal-only generation script
## PR 715 NOTHING NEW
## PR 714 NOTHING NEW
## PR 713
### 1. Changed
### 2. Fixed
### 3. Added
- Add support for V8 DXNN file format
- Add PPU support for V8 models
## PR 710 NOTHING NEW
## PR 705 NOTHING NEW
## PR 707 NOTHING NEW
## PR 706 NOTHING NEW
## PR 703 NOTHING NEW
## PR 699 NOTHING NEW
## PR 702 NOTHING NEW
## PR 698 NOTHING NEW
## PR 697 NOTHING NEW
## PR 696 NOTHING NEW
## PR 694
### 1. Changed
### 2. Fixed
- Use 'python3 -m pip' instead of 'pip' for better reliability
### 3. Added
## PR 693 NOTHING NEW
## PR 692
### 1. Changed
- Update Sanity Check to hide ONNX Runtime version when built with USE_ORT=OFF.
### 2. Fixed
### 3. Added
## PR 688
### 1. Changed
### 2. Fixed
### 3. Added
- Add DX-Fit tuning toolkit, documentation, and curated examples.
## PR 691 NOTHING NEW
## PR 689
### 1. Changed
- Standardize all Python and C++ examples to use argparse and cxxopts for consistent command-line argument parsing
- Update all examples to support unified argument format
    - -m, --model for .dxnn model file path
    - -l, --loops for inference loop count
    - Additional options like verbose follow similar format conventions per example
- Update dxbenchmark default behavior to execute 30 loops when neither loop nor time options are specified
- Enhance dxbenchmark to automatically create result directory if it does not exist when result path is specified
### 2. Fixed
### 3. Added
## PR 690
### 1. Changed
- add a system requirement check in install.sh (RAM: 8GB, Arch: x86_64 or aarch64)
- remove the check for the libdxrt.so location in Sanity Check
### 2. Fixed
### 3. Added
## PR 687 NOTHING NEW
## PR 684 NOTHING NEW
## PR 683 NOTHING NEW
## PR 682 NOTHING NEW
## PR 675
### 1. Changed
### 2. Fixed
### 3. Added
-  Support dynamic shape output of tail cpu task
## PR 681 NOTHING NEW
## PR 679 NOTHING NEW
## PR 678 NOTHING NEW
## PR 677 NOTHING NEW
## PR 674 NOTHING NEW
## PR 673 NOTHING NEW
## PR 669 NOTHING NEW
## PR 668 NOTHING NEW
## PR 670 NOTHING NEW
## PR 667 NOTHING NEW
## PR 666 NOTHING NEW
## PR 660 NOTHING NEW
## PR 664
### 1. Changed
### 2. Fixed
- fix some compile errors and warnings in windows environment
### 3. Added
## PR 662
### 1. Changed
- Improve parse_model CLI tool.
### 2. Fixed
- Fix configuration option name in common.cfg.
### 3. Added
- Implement asynchronous NPU Format Handler (NFH).
## PR 658 NOTHING NEW
## PR 657 NOTHING NEW
## PR 654 NOTHING NEW
## PR 652 NOTHING NEW
## PR 650 NOTHING NEW
## PR 651 NOTHING NEW
## PR 649
### 1. Changed
### 2. Fixed
- Update cross compile script for dxtop
### 3. Added
## PR 648 NOTHING NEW
## PR 640 NOTHING NEW
## PR 639 NOTHING NEW
## PR 637
### 1. Changed
### 2. Fixed
### 3. Added
- Add new functions to profiler (Flush and GetPerformanceData)
- Add dxbenchmark, a command-line tool for comparing performance metrics across multiple models and generating detailed 
## PR 642 NOTHING NEW
## PR 638 NOTHING NEW
## PR 634
### 1. Changed
- remove dsp related code
### 2. Fixed
### 3. Added
## PR 615 NOTHING NEW
## PR 627
### 1. Changed
### 2. Fixed
### 3. Added
- model voltage profiler (run_model_prof.py)
  - requires firmware > 2.2.0 and driver > 1.7.1
## PR 631 NOTHING NEW
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
