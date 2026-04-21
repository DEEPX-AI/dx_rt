This chapter describes the system requirements, source file structure, and the installation instructions for setting up **DX-RT** on a Windows-based host system.  

After you check the system requirements, follow these instructions.  

- System Requirement Check  
- DX-RT Windows Driver Installation  
- Visual Studio 2022 Setup  
- DX-RT Framework Build  
- Runtime Service Setup  
- Installation Verification  

---

## Overview & System Requirements


**DX-RT** is a runtime software package for operating DEEPX NPU. This software is compatible with multiple DEEPX NPU products, including **DX-M1**, **DX-M1M**, and **DX-H1 Quattro**. The main build outputs from this repository are 

| Output | Description |
|--------|-------------|
| `dxrt.dll` | Core runtime library for NPU inference |
| `dxrt.lib` | Link library for application builds |
| `dxrtd.exe` | Runtime daemon service for managing apps using `dxrt.dll` |
| `dxrt-cli.exe` | Device management and firmware interface tool |
| `run_model.exe` | Model inference and performance verification tool |
| `parse_model.exe` | Model information viewer |
| `dxbenchmark.exe` | Benchmarking utility |
| `dxtop.exe` | Real-time NPU monitoring tool |

The built libraries are used by [dx_app](https://github.com/DEEPX-AI/dx_app), and users can also develop their own applications by referencing the dx_app code.

!!! note "NOTE. Pre-built Binaries"  
    To use pre-built binaries without building, refer to the [dx_rt_windows](https://github.com/DEEPX-AI/dx_rt_windows) repository.  


### Hardware Requirements

| Item | Requirement | Notes |
|------|-------------|-------|
| **CPU** | amd64 (x86_64) | |
| **RAM** | 8GB or more | 16GB or more recommended |
| **Storage** | 4GB or more | Free disk space |
| **NPU Connection** | DEEPX NPU device (DX-M1) | Choose one of the connection methods below |

**Supported NPU Connection Methods**

| Connection Method | Description |
|-------------------|-------------|
| PCIe Slot | Direct connection to PCIe slot |
| M.2 Slot | Connection via M.2 interface |
| USB 4.0 | PCIe tunneling support required (Thunderbolt™ or USB4) |

### Software Requirements

| Component | Requirement |
|-----------|-------------|
| **OS** | Windows 10 or later (x64) |
| **Visual Studio** | Visual Studio Community 2022 (Desktop development with C++ workload) |
| **CMake** | Included with Visual Studio 2022 |
| **VCPKG** | Included with Visual Studio 2022 |
| **DX_FW** | v2.5.0 or later |

!!! warning "IMPORTANT"  
    **The NPU device driver must be installed first.** For driver installation instructions, see Section. **DX-RT Windows Driver Installation** below.  

---

## Pre-installation Setup

### DX-RT Windows Driver Installation

Before building and running DX-RT, you need to install the DX-RT Windows driver.

**Driver Download**  

Drivers are provided in the [dx_rt_windows](https://github.com/DEEPX-AI/dx_rt_windows) repository.  

```
dx_rt_windows/
└── m1/v.3.2.0/
    ├── dxm1drv/           # PCIe driver package (dxm1drv.zip)
    ├── dx_rt/             # Runtime (bin/, include/, lib/)
    └── dx_app/            # Demo applications and examples
```

- `dxm1drv/`: DX-RT Windows driver (dxm1drv.zip archive)  
- `dx_rt/`: DLLs and executables built from DX-RT code  
- `dx_app/`: Executables built by linking dx_app repository with dxrt.lib  

**Driver Installation Steps**  

**Step 1.** Connect the Device  

Connect the DEEPX NPU device to your system.  

- **PCIe or M.2 connection:** Install device in an available PCIe or M.2 slot  
- **USB connection:** Connect to a USB 4.0 port with PCIe tunneling support (regular USB 3.x ports are not supported)  

**Step 2.** Install the Driver

2-1. Navigate to the `m1/v.3.2.0/dxm1drv/` directory  
2-2. Extract `dxm1drv.zip`  
2-3. Open the extracted `dxm1drv/` folder  
2-4. Right-click on the `dxm1drv.inf` file  
2-5. Select **Install** from the context menu  
2-6. If a security warning about the driver publisher appears, click **Install** or **Yes**  
2-7. Wait for installation to complete  

**Advanced.** Installation via Command Line  

Open Command Prompt or PowerShell as Administrator and run.  

```
pnputil /add-driver "path\to\extracted\dxm1drv\dxm1drv.inf" /install
```

**Step 3.** Verify Installation

3-1. Open Device Manager (right-click Start menu → Device Manager)  
3-2. Look for **M1 PCI CONTROLLER** device  
3-3. Verify that the device appears in the list without warning icons  

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
    └── dxrt.sln
```


!!! warning "WARNING. DLL Placement for Execution"  
    The executables (`.exe`) and the DLLs (`dxrt.dll, onnxruntime.dll`) **must reside in the same folder.**  
    - **Issue:** Windows may prioritize an older version of `onnxruntime.dll` found in `C:\Windows\System32`.  
    - **Solution:** Always keep the DLLs in the local bin folder to ensure the correct version is loaded first and avoid runtime crashes.  

---

##  Runtime Setup (Service Configuration)

After the build is complete, you must configure the runtime environment. The most critical component in this stage is the DEEPX Runtime Daemon (`dxrtd.exe`).

**Understanding the DX-RT Architecture**  

Before proceeding with the setup, it is important to understand how the application interacts with the NPU hardware. The `dxrtd.exe` acts as a middleman (daemon) that manages the communication between your software and the physical device.  

- **User Application:** Links with `dxrt.lib` and calls functions from `dxrt.dll`.  
- **dxrtd.exe (Daemon):** A background service that manages NPU resource scheduling, memory allocation, and concurrency.  
- **Multi-Process Support:** Because `dxrtd` manages the hardware access, multiple applications can perform inference on the NPU simultaneously without resource conflicts.  

!!! warning "IMPORTANT. Why is the Daemon Required?"  
     Without dxrtd.exe running in the background, dxrt.dll cannot establish a connection to the DX-RT driver. Therefore, **the service must be active** before you run any inference tasks or CLI tools.

### Option A: Register as Windows Service (Recommended)

This method ensures the daemon starts automatically with Windows and runs efficiently in the background.

A-1. Open Command Prompt or PowerShell as Administrator  

A-2. Navigate to the built `bin/` directory  
   ```batch
   cd out\install\x64-Release\bin
   ```

A-3. Install Windows service  
   ```batch
   dxrtd.exe --install
   ```

A-4. Start the service  
   ```batch
   dxrtd.exe --start
   ```

A-5. Verify service is running  
     : Check DeepX runtime service in Windows Services list (`services.msc`)  
     : Or verify `dxrtd.exe` process in Task Manager  

To remove the service
```batch
dxrtd.exe --stop
dxrtd.exe --uninstall
```

### Option B: Manual Execution (Debug Mode)

For development and testing purposes, you can run manually. In this case, you need to keep the terminal window open. 

```batch
cd out\install\x64-Release\bin
dxrtd.exe
```

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
cd out\install\x64-Release\bin
dxrt-cli.exe -s
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
dxrt-cli.exe -i
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
parse_model.exe -m your_model.dxnn
```

Run inference
```batch
run_model.exe -m your_model.dxnn -l 10
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
| `dxrt-cli.exe` | Device management and firmware interface | `-s` status, `-i` info, `-r` reset, `-u` firmware update |
| `run_model.exe` | Model inference and performance verification tool | `-m` model, `-l` loop count, `-v` verbose |
| `parse_model.exe` | Model information viewer | `-m` model, `-v` verbose, `-j` JSON extract |
| `dxbenchmark.exe` | Batch performance benchmarking | `--dir` model directory, `-l` loop count |
| `dxtop.exe` | Real-time NPU monitoring | - |

For detailed usage of each tool, check the `-h` option
```batch
run_model.exe -h
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
- Try reinstalling the driver using the right-click INF file installation method

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

- [dx_rt_windows](https://github.com/DEEPX-AI/dx_rt_windows) - Pre-built Windows binaries and drivers
- [dx_app](https://github.com/DEEPX-AI/dx_app) - Demo applications using DX-RT

---
