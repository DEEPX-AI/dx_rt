This chapter covers Windows requirements, DEEPX SDK installation, source builds, runtime setup, and verification.

## Overview & Requirements

**DX-RT** is the runtime for DEEPX NPUs, including **DX-M1**, **DX-M1M**, and **DX-H1 Quattro**.

| Output | Description |
|--------|-------------|
| `dxrt.dll` / `dxrt.lib` | Runtime library / application link library |
| `onnxruntime.dll` / `onnxruntime_providers_shared.dll` | ONNX Runtime libraries for CPU operations |
| `dxrtd.exe` | Runtime daemon for resource scheduling and multi-process/device support |
| `dxcli.exe` | Device management and firmware interface |
| `dxrun.exe` | Model inference and performance verification |
| `dxparse.exe` | Model information viewer |
| `dxbenchmark.exe` | Batch benchmarking |
| `dxtop.exe` | Real-time NPU monitoring |

!!! note "Legacy Names"
    `dxrt-cli.exe`, `run_model.exe`, and `parse_model.exe` remain available as backward-compatible aliases.

### Hardware

| Item | Requirement |
|------|-------------|
| CPU | amd64 (x86_64) |
| RAM | 8 GB minimum; 16 GB recommended |
| Storage | At least 4 GB free |
| NPU connection | PCIe slot, M.2 slot, or USB4/Thunderbolt with PCIe tunneling |

!!! note "USB Connection"
    USB connections require PCIe tunneling. Regular USB 3.x ports are not supported.

### Software

| Component | Requirement |
|-----------|-------------|
| OS | Windows 10 22H2 (build 19045) or later (x64) |
| Source build tools | Visual Studio 2022 with **Desktop development with C++**, including CMake and vcpkg |
| Driver | v1.8.0 or later |
| PCIe Driver | v2.0.1 or later |
| Firmware | v2.7.6 or later |

!!! warning "Driver Required"
    Install the NPU device driver before using DX-RT.

The DEEPX SDK installer provides the driver and pre-built runtime; building from source is optional.

## Install the DEEPX SDK

1. Connect the NPU using one of the supported interfaces above.
2. Download the installer for your NPU from [DEEPX Software Download](https://developer.deepx.ai/sw-download/).
3. Run the installer. In the UAC prompt, verify the publisher is **DEEPX Co., Ltd.** and allow Administrator access.
4. Follow the prerequisite checks for Windows and .NET Desktop Runtime 10 x64.
5. Select an SDK package and accept its license agreement.
6. Select components: **Driver** and **Runtime** are required; choose **C/C++ SDK**, **Python SDK**, and **Examples** as needed.
7. Choose an installation path and select **Install**. Close programs using target files if prompted.
8. Review the completion results. Use **Installation Status Diagnostics** and **Rescan** for issues; see the installer's **User Guide** for screen-by-screen instructions.
9. Open a new terminal and run `dxcli -s`. Use **Quick Start and Next Steps** or the **DEEPX** Start menu for Launcher, firmware tools, and Python setup.

### Modify or Remove an Existing Installation

To change an existing installation, rerun the installer:

| Mode | Action |
|------|--------|
| Upgrade | Replace the installation with a newer version |
| Repair | Reinstall the same component configuration |
| Modify | Add or remove components |
| Uninstall | Remove the complete installation; close programs using installed files when prompted |

## Build from Source

### Visual Studio 2022

Install [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/) with **Desktop development with C++**. For CLI-only builds, install Build Tools 2022 from the repository root:

```batch
build.bat install-tools
```

This uses `winget`. Restart the terminal after installation.

!!! warning "Visual Studio 2022 Required"
    Use Visual Studio 2022. Visual Studio 2019 or earlier and 2026 or later cannot build DX-RT due to compiler compatibility issues; install 2022 separately if necessary.

    `build.bat` selects Visual Studio 2022 even when a newer Visual Studio with C++ tools is also installed. If you start it from the Developer Command Prompt of another Visual Studio version, that environment is used instead, so run `build.bat` from a regular terminal or the **Developer Command Prompt for VS 2022**. The `VS Path` line in the build banner shows the selected installation and MSVC version.

### Presets

| Preset | Generator | Purpose |
|--------|-----------|---------|
| `x64-Debug` | Ninja | Debug build |
| `x64-Release` | Ninja | Release build |
| `x64-VS` | Visual Studio 17 2022 | Generate `build_vs\dxrt.sln` |

### Method 1: Visual Studio IDE

1. In Visual Studio 2022, select **Open a local folder** and open the repository.
2. Wait for CMake configuration and automatic vcpkg dependency installation.
3. Select `x64-Debug` or `x64-Release`, then **Build > Build All**.
4. Select **Build > Install** to copy outputs to `out\install\x64-<type>`.

Alternatively, `build.bat vs` generates `build_vs\dxrt.sln` for the IDE.

### Method 2: build.bat

Open a regular Command Prompt in the repository root. A Developer Command Prompt is not required; `build.bat` initializes the Visual Studio environment.

| Command | Description |
|---------|-------------|
| `build.bat` | Full Release build (same as `build.bat all`); waits for a key before exiting |
| `build.bat debug` | Full Debug build (same as `build.bat debug all`) |
| `build.bat configure` | Configure only |
| `build.bat build` | Build only (skip configure), then run the Python package step |
| `build.bat install` | Configure, build, install to `out\install\x64-<type>`, then run the Python package step |
| `build.bat clean` | Delete the selected build directory; optionally delete `vcpkg_installed` when prompted |
| `build.bat rebuild` | Clean the selected build directory, configure, build, then run the Python package step |
| `build.bat all` | Clean the selected build/install directories, configure, build, install, then run the Python package step |
| `build.bat distclean` | Delete `out`, `build_vs`, and `vcpkg_installed` |
| `build.bat vs` | Generate the Visual Studio solution |
| `build.bat setup` | Add install `bin` to system PATH; replace/register and start `dxrtd` (Administrator required) |
| `build.bat unsetup` | Stop/remove `dxrtd` and remove the selected install `bin` from system PATH (Administrator required) |
| `build.bat install-tools` | Install Build Tools 2022 via `winget` |
| `build.bat help` | Show help |

!!! note "Build Configuration"
    Release is the default. Prefix actions with `debug` for Debug, for example `build.bat debug install`.

To configure and build without the full clean/install sequence:

```batch
build.bat configure
build.bat build
```

For a clean rebuild without installation, use `build.bat rebuild`. These build actions still run the Python package step.

!!! warning "Output Deletion and Service Interruption"
    **Full builds delete the selected build and install directories**, including their existing contents: `out\build\x64-<type>` and `out\install\x64-<type>`. This also applies to `build.bat` without arguments and `build.bat debug`.

    Full builds and `install` attempt to stop a running `dxrtd` service and restart it after successful completion. `distclean` also attempts to stop the service, but does not restart it.

!!! warning "Python Package Reinstallation"
    Every action that builds (`build`, `rebuild`, `install`, and full builds) attempts to reinstall `dx-engine` for each Python 3.10-3.14 installation found through the `py` launcher, not just the active environment. It upgrades `pip`, `wheel`, `scikit-build-core`, and `pybind11`, then runs `pip install --no-deps --force-reinstall .` in `python_package`. If no supported Python is found, this step is skipped with a warning.

### Output Layout & DLL Placement

Build outputs are in `out\build\x64-<type>`. After installation, `out\install\x64-<type>` contains the following (`<type>` is `Debug` or `Release`):

```text
bin\                     Tools and runtime DLLs
    examples\            C++ example executables
include\
    dxrt\                Public headers
lib\                     dxrt.lib and pdcurses.lib
    cmake\
        dxrt\            dxrtConfig.cmake and dxrtConfigVersion.cmake
```

Debug builds name the runtime `dxrtdbg.dll` and `dxrtdbg.lib`. The CMake package files support application builds using `find_package(dxrt)` with the Release installation only.

!!! warning "DLL Placement"
    Keep `dxrt.dll`, `onnxruntime.dll`, and `onnxruntime_providers_shared.dll` together. The application must be able to locate `dxrt.dll`; placing all three DLLs beside the executable is the simplest layout.

    In Windows builds with ONNX Runtime enabled, DX-RT uses a delay-load hook to load `onnxruntime.dll` explicitly from the directory containing `dxrt.dll` and searches that directory for its dependencies. Do not rely on an unrelated ONNX Runtime installation in `C:\Windows\System32`.

## Run a Source-Built dxrtd

The SDK installer registers and starts `dxrtd` and adds its tools to PATH. No manual setup is needed for the installed runtime.

For source builds, `dxrtd` manages NPU resources for applications using `dxrt.dll`. With the supplied service-enabled presets, run the daemon before inference, either as a Windows service or in a console.

### Option A: Windows Service

!!! warning "Existing Installer Service"
    The installer and source build use the same `dxrtd` service name. `setup` stops and removes an existing service before registering the source build. `unsetup` removes the currently registered service; it does not restore the installer service.

    After testing, run `unsetup`, then rerun the SDK installer and select **Repair** to restore the installer-managed service.

Build and install the source outputs first. From the repository root in an **Administrator Command Prompt**:

```batch
build.bat setup
sc.exe query dxrtd
```

`setup` requires `out\install\x64-Release\bin\dxrtd.exe`, adds that `bin` to the **system PATH**, and registers and starts the service. Open a new terminal for the PATH change. Use `build.bat debug setup` for Debug.

To stop/remove the service and remove the source PATH entry, run from the same repository root as Administrator:

```batch
build.bat unsetup
```

Use `build.bat debug unsetup` for Debug. Keep the source outputs until cleanup is complete.

### Option B: Console (Development/Testing)

If the installer service is running, stop it from an Administrator terminal before starting the source daemon:

```batch
sc.exe stop dxrtd
cd out\install\x64-Release\bin
dxrtd.exe
```

Use `x64-Debug` for Debug. Keep the console open while testing, then press `Ctrl+C` to stop the source daemon. If you stopped the installer service, restart it with `sc.exe start dxrtd`. Do not uninstall it for console testing.

!!! note "Service Control Manager"
    `dxrtd.exe --run` (or `-r`) is for Windows SCM, not interactive execution. Use Option A for a persistent background service.

## Verify the Installation

Open a new terminal after installation or PATH changes. For source builds, ensure the intended source `bin` is used; you can run its tools directly from that directory.

1. Check device status; expect device information without errors:

    ```batch
    dxcli -s
    ```

2. Check that the daemon process is running:

    ```batch
    tasklist | findstr dxrtd
    ```

3. Run 10 inference loops with a valid `.dxnn` model; expect successful inference and performance metrics:

    ```batch
    dxrun -m your_model.dxnn -l 10
    ```

Use `dxcli -i` for detailed device information or `dxtop` for monitoring (`q` to quit).

If the Python SDK is installed, use the same Python interpreter that received `dx-engine`, not an unrelated `python` on PATH. Activate the virtual environment where you installed the package, if applicable, then run:

```batch
python -c "import dx_engine"
```

After a source build, verify each Python version for which the build reported a successful installation. For example, if Python 3.12 was installed through the build's `py` launcher step:

```batch
py -3.12 -c "import dx_engine"
```

## Tools Reference & Troubleshooting

| Tool | Key Options |
|------|-------------|
| `dxcli.exe` | `-s` status, `-i` info, `-r` reset, `-u` firmware update |
| `dxrun.exe` | `-m` model, `-l` loop count, `-v` verbose |
| `dxparse.exe` | `-m` model, `-v` verbose, `-j` JSON extract |
| `dxbenchmark.exe` | `--dir` model directory, `-l` loop count |
| `dxtop.exe` | Real-time monitoring; `q` to quit |

Run a tool with `-h` for full usage. Options belong to the named executable: `dxcli.exe -i` requests device information, whereas `dxrtd.exe -i` installs the service.

| Daemon Command | Action |
|----------------|--------|
| `dxrtd.exe` | Run in a console |
| `dxrtd.exe --install` (or `-i`) | Install the Windows service |
| `dxrtd.exe --uninstall` (or `-u`) | Uninstall the Windows service |
| `dxrtd.exe --start` / `dxrtd.exe --stop` | Start / stop the registered service |
| `dxrtd.exe --run` (or `-r`) | Run under Windows SCM only |

### dx_app Integration

Use [dx_app](https://github.com/DEEPX-AI/dx_app) for reference code and demos, or develop your own application using the DX-RT libraries and headers. See [DX-APP Installation and Build](https://github.com/DEEPX-AI/dx_app/blob/main/docs/source/docs/02_DX-APP_Installation_and_Build.md).

### Troubleshooting

| Problem | Checks |
|---------|--------|
| Device not detected | Check PCIe/M.2 seating and Device Manager warnings. Use installer **Installation Status Diagnostics > Rescan**, or **Repair** the installation. |
| Daemon will not start | Run `dxrtd.exe` from the intended `bin` directory to inspect errors; check permissions and Windows Firewall/antivirus restrictions. |
| Build failure | Check Visual Studio 2022 and the C++ workload. `build.bat rebuild` recreates the selected build directory (including its CMake cache) and **also attempts Python package reinstallation** as described above. |
| Performance issues | Confirm the daemon is running and use `dxtop` to check thermal throttling; ensure adequate NPU cooling. |

### Related Links

- [DEEPX Software Download](https://developer.deepx.ai/sw-download/) - SDK installer for Windows
- [dx_app](https://github.com/DEEPX-AI/dx_app) - Demo applications using DX-RT
