This chapter describes the system requirements, source file structure, and the installation instructions for setting up **DX-RT** on a Windows-based host system.  

After you check the system requirements, follow these instructions.  

- System Requirement Check  
- DEEPX SDK Installation  
- Visual Studio 2022 Setup  
- DX-RT Framework Build  
- Runtime Service Setup  
- Installation Verification  

---

## Overview & System Requirements


**DX-RT** is a runtime software package for operating DEEPX NPU. This software is compatible with multiple DEEPX NPU products, including **DX-M1**, **DX-M1M**, and **DX-H1 Quattro**. The main build outputs from this repository are:

| Output | Description |
|--------|-------------|
| `dxrt.dll` | Core runtime library for NPU inference |
| `dxrt.lib` | Link library for application builds |
| `dxrtd.exe` | Runtime daemon service for managing apps using `dxrt.dll` |
| `dxcli.exe` | Device management and firmware interface tool |
| `dxrun.exe` | Model inference and performance verification tool |
| `dxparse.exe` | Model information viewer |
| `dxbenchmark.exe` | Benchmarking utility |
| `dxtop.exe` | Real-time NPU monitoring tool |

The built libraries are used by [dx_app](https://github.com/DEEPX-AI/dx_app), and users can also develop their own applications by referencing the dx_app code.

!!! note "NOTE. Legacy Names"  
    Legacy executable names `dxrt-cli.exe`, `run_model.exe`, and `parse_model.exe` are preserved as backward-compatible copies and continue to work.

!!! note "NOTE. Installer Package"  
    To install the pre-built DEEPX software package, including the driver, firmware binaries, Python wheel, and DX-RT runtime files, download and run the installer from the [DEEPX Software Download](https://developer.deepx.ai/sw-download/) page.  


### Hardware Requirements

| Item | Requirement | Notes |
|------|-------------|-------|
| **CPU** | amd64 (x86_64) | |
| **RAM** | 8GB or more | 16GB or more recommended |
| **Storage** | 4GB or more | Free disk space |
| **NPU Connection** | DEEPX NPU device | Choose one of the connection methods below |

**Supported NPU Connection Methods**

| Connection Method | Description |
|-------------------|-------------|
| PCIe Slot | Direct connection to PCIe slot |
| M.2 Slot | Connection via M.2 interface |
| USB 4.0 | PCIe tunneling support required (Thunderbolt™ or USB4) |

### Software Requirements

| Component | Requirement |
|-----------|-------------|
| **OS** | Windows 10 22H2 (build 19045) or later (x64) |
| **Visual Studio** | Visual Studio Community 2022 (Desktop development with C++ workload) |
| **CMake** | Included with Visual Studio 2022 |
| **VCPKG** | Included with Visual Studio 2022 |
| **Driver** | v1.8.0 or later |
| **PCIe Driver** | v1.6.1 or later |
| **Firmware** | v2.7.0 or later |

!!! warning "IMPORTANT"  
    **The NPU device driver must be installed before using DX-RT.** The DEEPX SDK installer installs the driver as part of the SDK installation process described below.  

---

## Pre-installation Setup

### DEEPX SDK Installation

Before building and running DX-RT, install the DEEPX SDK. The installer includes the NPU driver, firmware binaries, runtime, and SDK components.

**SDK Installer Download**  

Download the DEEPX SDK installer for your NPU from the [DEEPX Software Download](https://developer.deepx.ai/sw-download/) page. The installer includes the driver, firmware binaries, Python wheel, runtime, and SDK components.  

**SDK Installation Steps**  

**Step 1.** Connect the Device  

Connect the DEEPX NPU device to your system.  

- **PCIe or M.2 connection:** Install device in an available PCIe or M.2 slot  
- **USB connection:** Connect to a USB 4.0 port with PCIe tunneling support (regular USB 3.x ports are not supported)  

**Step 2.** Run the Installer

2-1. Run the downloaded installer, for example `DEEPX_SDK_Setup_vX.Y.Z.exe`. The installer is a single executable and does not require extraction.  
2-2. In the User Account Control dialog, verify the publisher is **DEEPX Co., Ltd.** and select **Yes**. Administrator privileges are required to install the driver and register services.  

**Step 3.** Wait for the Prerequisite Checks

Before the installer window opens, it verifies the following requirements automatically:  

- Windows 10 22H2 (build 19045) or later. If the requirement is not met, the installer provides Windows Update guidance.  
- .NET Desktop Runtime 10 (x64). If it is not installed, the installer provides a download link or `winget` command.  

**Step 4.** Select an SDK Package

4-1. Select the SDK package to install.  
4-2. Select a package card to view its supported hardware specifications.  
4-3. Select at least one SDK package to enable **Next**.  

**Step 5.** Accept the License Agreement

Read the end-user license agreement and select the agreement checkbox at the bottom. **Next** remains disabled until you accept the agreement.  

**Step 6.** Review the Pre-installation Checks

The installer checks available disk space, DEEPX NPU hardware connectivity, and Python installation. You can install the SDK even when the hardware is not connected.  

**Step 7.** Select Components

Select the components you need: **Driver**, **Runtime**, **C/C++ SDK**, **Python SDK**, **C# SDK**, and **Examples**. The driver and runtime are required.  

**Step 8.** Select an Installation Path

Choose the installation path. Use the storage gauge to review current usage, estimated installation size, and available space.  

**Step 9.** Install the SDK

Select **Install** to begin installation. The installer extracts and copies files, installs the driver, registers services, configures environment variables, and creates shortcuts. If another program is using a target file, close the program when prompted.  

**Step 10.** Review the Installation Result

The completion screen displays the installation result and validation status for each item: **Passed**, **Needs Attention**, or **Unable to Verify**. From this screen, you can open:  

- Installation Status Diagnostics  
- Quick Start and Next Steps  
- DEEPX Launcher  
- User Guide  

**Step 11.** Finish

Select **Finish** to close the installer.  

### Installation Status Diagnostics

Select **Installation Status Diagnostics** from the completion screen to check environment variables, services, hardware, installed components, and Python integration. After resolving an issue, select **Rescan** to verify it again. Select **Copy Results** to copy the full diagnostic results for support.  

### Quick Start and Next Steps

From the completion screen, you can install the Python SDK with `pip`, open the installation folder, start DEEPX Launcher or the firmware downloader, and view component-specific next steps and commands. Do not close the installer while the Python SDK is being installed.  

### Modify an Existing Installation

When you run the installer on a system with an existing installation, a mode selection screen displays the installed version and path. Select one of the following modes:  

| Mode | Action |
|------|--------|
| Upgrade | Overwrite the existing installation with a new version |
| Repair | Reinstall the same component configuration |
| Modify | Add or remove components |
| Uninstall | Remove the complete installation |

During uninstallation, close any programs using installed files when prompted. The installer also removes the DEEPX Start menu group and desktop shortcuts.  

### Optional Post-installation Verification

Run the following commands from Command Prompt to verify the installation:  

```batch
dxcli -s
```

If you installed the **Python SDK** component, verify Python integration:  

```batch
python -c "import dx_engine"
```

You can also start DEEPX Launcher, the firmware downloader, or DX Top Monitor from the **DEEPX** Start menu group or desktop shortcuts.  

### Visual Studio 2022 Setup

Visual Studio Community 2022 is required to build DX-RT.

**Installation Steps**  

Step 1. Download [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/)  
Step 2. Run the installer and select the following workload: **Desktop development with C++**  
Step 3. (Optional) Select any additional workloads or individual components as needed  
Step 4. Click **Install** to proceed with installation  

!!! warning "IMPORTANT. Visual Studio 2022 Required"  
    **You must use Visual Studio 2022.** Visual Studio 2019 or earlier, or 2026 or later versions cannot build DX-RT due to compiler compatibility issues. Even if you have other versions installed, install Visual Studio 2022 separately.  

---

## DX-RT Build Methods

DX-RT can be built using two methods. 

- Build using Visual Studio 2022 IDE  
- Build using Command Line Interface (CLI)  

### CMake Presets Configuration

DX-RT provides the following build presets via `CMakePresets.json`

| Preset | Description | Generator |
|--------|-------------|-----------|
| `x64-Debug` | x64 Debug build | Ninja |
| `x64-Release` | x64 Release build | Ninja |
| `x64-VS` | Generate Visual Studio solution/project files | Visual Studio 17 2022 |

---

### Method 1: Build Using Visual Studio 2022 IDE

You can build the CMake project directly using Visual Studio 2022's Open Folder feature.

**Step 1.** Open Project Folder  

1-1. Launch Visual Studio Community 2022  
1-2. Select **Open a local folder** from the start screen  
1-3. Select the DX-RT project folder (`dx_rt`)  

!!! warning "IMPORTANT"  
    **You must use Visual Studio 2022.** Compatibility with other versions (VS 2019, VS Code, etc.) is not guaranteed.  

**Step 2.** CMake Configuration  

When you open the project.  

- Dependencies specified in `vcpkg.json` are automatically downloaded and installed in the `vcpkg_installed` directory.  
- CMake automatically generates the build cache and configuration.  

**Step 3.** Select Build Configuration  

3-1. Click the build configuration dropdown in the top toolbar  
3-2. Select the desired preset  
- **x64-Debug**: For debugging builds  
- **x64-Release**: For release builds  

**Step 4.** Run Build  

4-1. Go to the **Build** menu  
4-2. Click **Build All** (or **Rebuild All**)  

When the build succeeds, outputs are generated in  

- Debug: `out/build/x64-Debug/`  
- Release: `out/build/x64-Release/`  

**Step 5.** Install  

To copy built outputs to the install directory.

5-1. **Build** menu → Select **Install**  

Install paths  

- Debug: `out/install/x64-Debug/`  
- Release: `out/install/x64-Release/`  

---

### Method 2: Build Using build.bat

Using the `build.bat` script, you can automatically handle Visual Studio environment setup, CMake configuration, build, and installation all at once.  

**build.bat Overview**

`build.bat` automatically handles the following:

- Auto-detection and setup of Visual Studio 2022 environment
- CMake configuration and build execution
- Installation and cleanup tasks

!!! note "NOTE. Installing Visual Studio Build Tools (For CLI-only builds without IDE)"  
    To build using only the command line without Visual Studio IDE, you can install just the Build Tools. 
    ```batch
    build.bat install-tools
    ```
    This command automatically installs Visual Studio Build Tools 2022 via winget.  

**Step 1.** Open Command Prompt  

Open a regular Command Prompt (`cmd.exe`). **Developer Command Prompt is not required** - `build.bat` automatically sets up the Visual Studio environment.

**Step 2.** Navigate to Project Directory

```batch
cd C:\path\to\dx_rt
```

**Step 3.** Run Build

Default build (Release configuration + build)
```batch
build.bat
```

Debug build
```batch
build.bat debug
```

Build and install
```batch
build.bat install
build.bat debug install
```

Full build (clean + configure + build + install)
```batch
build.bat all
build.bat debug all
```

**build.bat Command Options**  

| Command | Description |
|---------|-------------|
| `build.bat` | Release configure and build (default) |
| `build.bat debug` | Debug configure and build |
| `build.bat configure` | Run CMake configure only (skip build) |
| `build.bat build` | Run build only (skip configure) |
| `build.bat install` | Build and install (installs to `out\install\`) |
| `build.bat clean` | Clean build directories |
| `build.bat rebuild` | Clean and rebuild (configure + build) |
| `build.bat all` | Full build: clean + configure + build + install |
| `build.bat distclean` | Full reset (delete `out/`, `build_vs/`, `vcpkg_installed/`) |
| `build.bat vs` | Generate Visual Studio solution file (`build_vs\dx_rt.sln`) |
| `build.bat install-tools` | Install VS Build Tools 2022 (using winget) |
| `build.bat help` | Show help |

**Usage Examples**  

```batch
:: Release build (configure + build)
build.bat

:: Debug build (configure + build)
build.bat debug

:: Release build only (skip configure)
build.bat build

:: Debug build only (skip configure)
build.bat debug build

:: Release configure only
build.bat configure

:: Debug configure only
build.bat debug configure

:: Release clean and rebuild
build.bat rebuild

:: Debug clean and rebuild
build.bat debug rebuild

:: Release build + install
build.bat install

:: Debug build + install
build.bat debug install

:: Release full build (clean + configure + build + install)
build.bat all

:: Debug full build (clean + configure + build + install)
build.bat debug all

:: Generate Visual Studio project files
build.bat vs

:: Delete all build artifacts (keep source only)
build.bat distclean
```

!!! note "NOTE. Generate Visual Studio Solution"  
    Running `build.bat vs` generates the `build_vs\dx_rt.sln` file. You can open this file in Visual Studio to build in the IDE environment.
    ```batch
    start build_vs\dx_rt.sln
    ```

### Build Output & Directory Layout

Directory structure generated after build and installation. 

```
dx_rt/
├── out/
│   ├── build/
│   │   ├── x64-Debug/        # Debug build outputs
│   │   └── x64-Release/      # Release build outputs
│   └── install/
│       ├── x64-Debug/        # Debug install outputs
│       │   ├── bin/          # Executables and DLLs
│       │   ├── include/      # Header files
│       │   └── lib/          # Library files
│       └── x64-Release/      # Release install outputs
│           ├── bin/
│           ├── include/
│           └── lib/
└── build_vs/                 # Visual Studio solution (when using x64-VS preset)
    └── dx_rt.sln
```


!!! warning "WARNING. DLL Placement for Execution"  
    The executables (`.exe`) and the DLLs (`dxrt.dll, onnxruntime.dll`) **must reside in the same folder.**  
    - **Issue:** Windows may prioritize an older version of `onnxruntime.dll` found in `C:\Windows\System32`.  
    - **Solution:** Always keep the DLLs in the local bin folder to ensure the correct version is loaded first and avoid runtime crashes.  

---

## Runtime Setup (Service Configuration)

When you install the DEEPX SDK using the installer, it automatically registers and starts the DEEPX Runtime Daemon (`dxrtd.exe`) as a Windows service. It also adds command-line tools such as `dxcli.exe` and `dxrtd.exe` to the system `PATH`, so you can run them from any Command Prompt or PowerShell window.

The manual service configuration steps below apply only when you build DX-RT from source.

**Understanding the DX-RT Architecture**  

Before proceeding with the setup, it is important to understand how the application interacts with the NPU hardware. The `dxrtd.exe` acts as a middleman (daemon) that manages the communication between your software and the physical device.  

- **User Application:** Links with `dxrt.lib` and calls functions from `dxrt.dll`.  
- **dxrtd.exe (Daemon):** A background service that manages NPU resource scheduling, memory allocation, and concurrency.  
- **Multi-Process Support:** Because `dxrtd` manages the hardware access, multiple applications can perform inference on the NPU simultaneously without resource conflicts.  

!!! warning "IMPORTANT. Why is the Daemon Required?"  
     Without dxrtd.exe running in the background, dxrt.dll cannot establish a connection to the DX-RT driver. Therefore, **the service must be active** before you run any inference tasks or CLI tools.

### Option A: Register a Source Build as a Windows Service

Use this method to test a source build as a Windows service. It ensures the daemon starts automatically with Windows and runs efficiently in the background.

!!! warning "IMPORTANT. Existing Installer Service"  
    The source build and the DEEPX SDK installer use the same `dxrtd` Windows service name. Before registering a source build, stop and uninstall the existing installer service; otherwise, `dxrtd.exe --install` fails. After testing the source build, run the DEEPX SDK installer and select **Repair** to restore the installer-managed service.  

A-1. Open Command Prompt or PowerShell as Administrator  

A-2. If the DEEPX SDK installer service is registered, stop and uninstall it  
    ```batch
    dxrtd.exe --stop
    dxrtd.exe --uninstall
    ```

A-3. Navigate to the source build output directory  
    ```batch
    cd out\install\x64-Release\bin
    ```
    Replace `x64-Release` with `x64-Debug` when testing a Debug build.  

A-4. Install Windows service  
   ```batch
   dxrtd.exe --install
   ```

A-5. Start the service  
   ```batch
   dxrtd.exe --start
   ```

A-6. Verify service is running  
     : Check DeepX runtime service in Windows Services list (`services.msc`)  
     : Or verify `dxrtd.exe` process in Task Manager  

To remove the service
```batch
dxrtd.exe --stop
dxrtd.exe --uninstall
```

### Option B: Run a Source Build Manually (Debug Mode)

For development and testing purposes, you can run manually. In this case, you need to keep the terminal window open. 

If the DEEPX SDK installer service is running, stop it before testing the source build:  

```batch
dxrtd.exe --stop
```

```batch
cd out\install\x64-Release\bin
dxrtd.exe
```
Replace `x64-Release` with `x64-Debug` when testing a Debug build.  

If you stopped the installer-managed service, restart it after the source-build test finishes:  

```batch
dxrtd.exe --start
```

Do not run `dxrtd.exe --uninstall` for manual testing because the installer service remains registered.  

!!! warning "IMPORTANT"  
    When running manually, closing the terminal window will terminate the service. For production environments, **Method A (Windows Service registration)** is recommended.  

!!! note "NOTE"  
    `dxrtd.exe --run` (or `-r`) is for Windows SCM (Service Control Manager) and should not be used for regular interactive execution.  

---

## Installation Verification (Sanity Check)

After completing the installation and service setup, follow these steps to verify that the DEEPX NPU and runtime environment are functioning correctly.

### Device Status & Daemon Verification

**Step 1.** Check Device Status  

```batch
dxcli.exe -s
```

If device information is displayed without errors, the driver is installed correctly.

**Step 2.** Verify Runtime Daemon  

Verify that `dxrtd.exe` is running.  

**Using Task Manager**  

2-1. Open Task Manager with `Ctrl + Shift + Esc`  
2-2. Go to the **Details** tab  
2-3. Verify that `dxrtd.exe` is in the process list  

Using command line
```batch
tasklist | findstr dxrtd.exe
```

**Step 3.** Check Detailed Device Information  

Check detailed information including hardware info, temperature, and utilization. 

```batch
dxcli.exe -i
```

### Performance & Model Inference Test

**Step 4.** Real-time NPU Monitoring (Optional)  

Monitor NPU core utilization, memory usage, and device temperature in real-time.  

```batch
dxtop.exe
```

Press `q` to exit the monitoring tool. 

**Step 5.** Model Inference Test

If you have a `.dxnn` model file.

Parse model
```batch
dxparse.exe -m your_model.dxnn
```

Run inference
```batch
dxrun.exe -m your_model.dxnn -l 10
```

This command runs 10 inference loops and displays performance metrics.

---

## Tool Descriptions & Integration

### Core Libraries & DLL Placement Warning

| File | Description |
|------|-------------|
| `dxrt.dll` | Core runtime library for NPU inference |
| `dxrt.lib` | Library for application linking |
| `onnxruntime.dll` | ONNX Runtime library for CPU operations |

!!! warning "WARNING. DLL Placement and Version Conflict"  
    To ensure correct operation, `dxrt.dll` and `onnxruntime.dll` **must be located in the same directory as your executable** (`.exe`).  
    - **The Reason:** Windows may prioritize an older version of `onnxruntime.dll` located in `C:\Windows\System32`.  
    - **The Consequence:** DX-RT requires a specific higher version; using the system's default version will lead to runtime crashes or entry-point errors.  
    - **Status:** A permanent fix for this search-order dependency is scheduled for the next release.  

### Command Line Tool Reference

System Services

| File | Description |
|------|-------------|
| `dxrtd.exe` | DEEPX Runtime Daemon - manages multi-process/device support |

Command Line Tools

| Tool | Description | Key Options |
|------|-------------|-------------|
| `dxcli.exe` | Device management and firmware interface | `-s` status, `-i` info, `-r` reset, `-u` firmware update |
| `dxrun.exe` | Model inference and performance verification tool | `-m` model, `-l` loop count, `-v` verbose |
| `dxparse.exe` | Model information viewer | `-m` model, `-v` verbose, `-j` JSON extract |
| `dxbenchmark.exe` | Batch performance benchmarking | `--dir` model directory, `-l` loop count |
| `dxtop.exe` | Real-time NPU monitoring | - |

For detailed usage of each tool, check the `-h` option
```batch
dxrun.exe -h
dxbenchmark.exe -h
```

### Integration with dx_app

The built **DX-RT** library can be used with [dx_app](https://github.com/DEEPX-AI/dx_app).  

- Role: `dx_app` provides practical reference code and demo applications using the dxrt framework.  
- For integration instructions with dx_app, refer to the [DX-APP Installation and Build](https://github.com/DEEPX-AI/dx_app/blob/main/docs/source/docs/02_DX-APP_Installation_and_Build.md) documentation.  

---

## Troubleshooting

### Device Not Detected

- Verify the device is properly seated in the PCIe/M.2 slot
- Check for warning icons in Device Manager
- Open **Installation Status Diagnostics** from the installer completion screen and select **Rescan**
- Run the installer again and select **Repair** to reinstall the installed components

### dxrtd.exe Won't Start

- Manually run `dxrtd.exe` from the `bin/` directory
- Check if Windows Firewall or antivirus software is blocking it
- Run as Administrator if permission issues occur

### Build Failure

- Verify Visual Studio 2022 is correctly installed
- Verify "Desktop development with C++" workload is installed
- Delete CMake cache and retry: delete the `out/build` folder

### Performance Issues

- Verify `dxrtd.exe` is running
- Change Windows power management setting to **High Performance**
- Ensure proper cooling for the NPU device
- Monitor thermal throttling with `dxtop.exe`

---

## Related Links

- [DEEPX Software Download](https://developer.deepx.ai/sw-download/) - DEEPX SDK installer package for Windows
- [dx_app](https://github.com/DEEPX-AI/dx_app) - Demo applications using DX-RT

---
