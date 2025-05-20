This chapter describes the system requirements, source file structure, and the installation instructions for setting up **DX-RT** on a Linux-based host system.  

After you check the system requirements, follow these instructions.  

- System Requirement Check  
- Build Environment Setup  
- Source File Structure Check  
- Framework Build  
- Linux Device Driver Installation  
- Python Package Installation  
- Service Registration  

## System Requirements

This section describes the hardware and software requirements for running **DX-RT** on Linux.

**Hardware and Software Requirements**  

- **CPU:** aarch64, x86_64
- **RAM:** 8GB RAM (16GB RAM or higher is recommended)
- **Storage:** 4GB or higher available disk space
- **OS:** Ubuntu 20.04 / 22.04 / 24.04 (x64 / aarch64)
- **Hardware:** The system **must** support connection to an **M1 M.2** module with the **M.2 interface** on the host PC. 

![](./../resources/02_DX-M1_M.2_LPDDR5x2.png){ width=600px }

---

## Build Environment Setup

**DEEPX** provides an installation shell script to set up the **DX-RT** build environment. You can install the entire toolchain installation or perform a partial installation as necessary.

**DX-RT** supports the Target OS of **Ubuntu 18.04,** **Ubuntu 20.04,** **Ubuntu 22.04,** and **Ubuntu 24.04**. 

**Installation of DX-RT**    
To install the full **DX-RT** toolchain, use the following commands.  

```
$ cd dx_rt
$ ./install.sh --all
```

Here are the available `install.sh` options. 
```
  ./install.sh [ options ]
    --help            Shows help message
    --arch [x86_64, aarch64]
                      Sets target CPU architecture
    --dep             Installs build dependencies : cmake, gcc, ninja, etc..
    --onnxruntime     (Optional) Installs onnxruntime library
    --all             Installs architecture + dependency + onnxruntime library
```

**Installation with ONNX Runtime**  
Use the ONNX Runtime option if you need to offload certain neural network (NN) operations to the CPU that are **not** supported by the NPU.  

We recommend using ONNX Runtime linux x64 version more than v1.20.1.  
```
https://github.com/microsoft/onnxruntime/releases/download/v1.20.1/onnxruntime-linux-x64-1.20.1.tgz
$ sudo tar -xvzf onnxruntime-linux-x64-1.20.1.tgz -C /usr/local --strip-components=1
$ sudo ldconfig
```

To install the ONNX Runtime library, run the following command.  
```
./install.sh --onnxruntime
```

**Installation for a Specific CPU Architecture**  
The **DX-RT** targets the **x86_64** architecture. If you’re compiling for another architecture (e.g., **aarch64**), specify it using the `--arch` option. 
```
./install.sh --arch aarch64 --onnxruntime
```

---

## Source File Structure

The **DX-RT** source directory is organized as follows. 
You can install the full toolchain using the `install.sh`, and the build and library using `build.sh`.  

```
├── assets
├── bin
├── cli
├── build.sh
├── build_x86_64
├── build_aarch64
├── cmake
├── docs
├── examples
├── extern
├── install.sh
├── lib
├── python_package
├── sample
├── service
└── tool
```

- `assets`: Images for documentation
- `bin`: Compiled binary executables
- `cli`: Command-line application source code
- `build.sh`: Shell script for building the framework
- `build_arch`: Build outputs for aarch64 architecture
- `cmake`: CMake scripts for build configuration
- `docs`: Markdown documents 
- `examples`: Inference example files
- `extern`: Third-party libraries
- `install.sh`: Shell script for toolchain installation
- `lib`: DX-RT library sources
- `python_package`: Python modules for DX-RT
- `sample`: Sample input files for demo apps
- `service`: Service unit files for runtime management
- `tool`: Profiler result visualization tools

---

## Framework Build on Linux

After compiling the **DX-RT** environment setup, you can build the framework using the provided `build.sh` shell script.

### Framework Source Build  
DEEPX supports the default target CPU architecture as **x86_64, aarch64**.  

The build script also supports options for build cleaning, specifying build type, and installing libraries to the system paths.  

**Build Instructions**  
To build the **DX-RT** framework, run the following command.
```
$ cd dx_rt
$ ./build.sh
```

Here are the `build.sh` options and their descriptions.  
```
./build.sh [ options ]
    --help       Shows help message
    --clean      Cleans previous build artifacts
    --verbose    Shows full build commands during execution
    --type [Release, Debug, RelWithDebInfo]
                 Specifies the cmake build type
    --arch [x86_64, aarch64]
                 Sets target CPU architecture
    --install <path>
                 Sets the installation path for built libraries
    --uninstall  Removes installed DX-RT files
    --clang      Compiles using clang
```

**Example. Build with `clean` Option**  
To clean existing build files before rebuilding.  
```
$ ./build.sh --clean
```

**Example. Build with `library` Option**  
To install build library files to `/usr/local`. 
```
# default path is /usr/local
$ ./build.sh --install /usr/local
```

### Options for Build Target  
**DX-RT** supports configuration  build targets, allowing you to enable or disable option features such as  ONNX Runtime, Python API, multi-process service support, and shared library builds.  

You can configure these options by editing the following file: `cmake/dxrt.cfg.cmake`  

Here are the available options for building targets.  
```
option(USE_ORT "Use ONNX Runtime" OFF)
option(USE_PYTHON "Use Python" OFF)
option(USE_SERVICE "Use Service" OFF)
option(USE_SHARED_DXRT_LIB "Build for DX-RT Shared Library" ON)
```

- `USE_ORT`: Enables ONNX Runtime for NN (neural network) operations that NPU does **not** support
- `USE_PYTHON`: Enables Python API support
- `USE_SERVICE`: Enables service for multi-process support 
- `USE_SHARED_DXRT_LIB`: Builds **DX-RT** as shared library (default: ON)

### Build Guide for Cross-compile  

**Setup Files for Cross-compile** 
DEEPX supports cross-compilation for the following default target CPU Architecture: **x86_64, aarch64**.  
DEEPX supports  the default target CPU architecture as **x86_64**.  

Toolchain Configuration  
To cross-compile for a specific target, configure the toolchain file.  
`cmake/toolchain.<CMAKE_SYSTEM_PROCESSOR>.cmake`

Example  
To cross-compile files for **aarch64**.
```
SET(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc )
SET(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++ )
SET(CMAKE_LINKER /usr/bin/aarch64-linux-gnu-ld )
SET(CMAKE_NM /usr/bin/aarch64-linux-gnu-nm )
SET(CMAKE_OBJCOPY /usr/bin/aarch64-linux-gnu-objcopy )
SET(CMAKE_OBJDUMP /usr/bin/aarch64-linux-gnu-objdump )
SET(CMAKE_RANLIB /usr/bin/aarch64-linux-gnu-ranlib )
```

**Non Cross-compile Case (Build on Host)**  
To build and install **DX-RT** on the host system, run the following command.  
```
./build.sh --install /usr/local
```

Recommended install path: `/usr/local` (commonly included in OS search paths)  

**Cross-compile Case (Build for Target Architecture)**  
Cross-compile for a specific architecture, run the following command.  
```
./build.sh --arch <target_cpu>
```

Here are the `build.sh` options and their descriptions.  
```
./build.sh [ options ]
   --help 	Shows help message
   --clean	Cleans previous build artifacts
   --verbose	Shows full build commands during execution
   --type 	Specifies the cmake build type : [ Release, Debug, RelWithDebInfo ]
   --arch 	Sets target CPU architecture : [ x86_64, aarch64, riscv64 ]
   --install	Installs build libraries 
   --uninstall 	Removes installed DX-RT files
```

Here are the examples of cross-compile cases.  
```
./build.sh --arch aarch64
./build.sh --arch x86_64
```

Output Directory  
After a successful build, output binaries is located under `<build directory> /bin/`
```
<build directory>/bin/
 ├── dxrtd
 ├── dxrt-cli
 ├── parse_model
 ├── run_model
 └── examples
```

---

## Linux Device Driver Installation
After building the **DX-RT** framework, you can install the Linux device driver for **M1 AI Accelerator** (NPU).  

### Prerequisites  
Before installing the Linux device driver, you should check that the accelerator device is properly recognized by the system.  

To check PCIe device recognition, run the following command.  
```
$ lspci -vn | grep 1ff4
0b:00.0 1200: 1ff4:0000
```

**Note.** **If there is no output,** the PCIe link is **not** properly connected. Please check the physical connection and system BIOS settings.  


**Optional.** Display the DEEPX name in `lspci`.  
If you want to display the DEEPX name in `lspci`, you can modify the PCI DB. (Only for Ubuntu)  

To display the DeepX device name, run the following command.
```
$ sudo update-pciids
$ lspci
...
0b:00.0 Processing accelerators: DEEPX Co., Ltd. DX_M1
```

### Linux Device Driver Structure  
The **DX-RT** Linux device driver source is structured to support flexible builds across devices, architectures, and modules. The directory layout is as follows.
```
- .gitmodules

- [modules]
    |
    - device.mk		
    - kbuild
    - Makefile
    - build.sh
    - [rt]
        - Kbuild
    - [pci_deepx] : submodule
        - Kbuild

- [utils] : submodule
```

- `device.mk`: Device configuration file
- `kbuild`: Top-level build rules
- `Makefile`: Build entry point
- `build.sh`: Build automation script
- `rt`: Runtime driver source (`dxrt_driver.ko`)
- `pci_deepx`: PCIe DMA driver (`submodule, dx_dma.ko`)
- `utils`: Supporting utilities (`submodule`)

Here are the descriptions of the key components.  

**`device.mk`**  
Defines supported device configuration.  

To build for a specific device, run the following command.  
```
$ make DEVICE=[device]
```

For example, in the case of a device like **M1**, you should select a submodule, such as PCIe, that has a dependency on **M1**.  
```
$ make DEVICE=m1 PCIE=[deepx]
```

**`kbuild`**  
Linux kernel build configuration file for each module directory. It instructs the kernel build system on how to compile driver modules.  

**`build.sh`**  
Shell script to streamline the build process. It runs the Makefile with common options. 

Here are the options for `build.sh`.
```
Usage:
Usage:
   build.sh <options>

options:
   -d, --device   [device]      select target device: m1
   -m, --module   [module]      select PCIe module: deepx
   -k, --kernel   [kernel dir]  'KERNEL_DIR=[kernel dir]', The directory where the 
                                kernel source is located 
                                default: /lib/modules/6.5.0-18-generic/build)
   -a, --arch     [arch]        set 'ARCH=[arch]' Target CPU architecture for 
                                cross-compilation, default: x86_64
   -t, --compiler [cross tool]  'CROSS_COMPILE=[cross tool]' cross compiler binary, 
                                e.g aarch64-linux-gnu-
   -i, --install  [install dir] 'INSTALL_MOD_PATH=[install dir]', module install 
                                directory install to: 
                                [install dir]/lib/modules/[KERNELRELEASE]/extra/
   -c, --command  [command]     clean | install | uninstall
                                - uninstall: Remove the module files installed 
                                on the host PC.
   -j, --jops     [jobs]        set build jobs
   -f, --debug    [debug]       set debug feature [debugfs | log | all]
   -v, --verbose                build verbose (V=1)
   -h, --help                   show this help
```

The build process generates the following kernel modules.  

  - `modules/rt` -> `dxrt_driver.ko`  
    : a core runtime driver for **M1 NPU** devices. This is responsible for system-level communication, memory control, and device command execution. 

  - `modules/pci_deepx` -> `dx_dma.ko`  
    : PCIe DMA (Direct Memory Access) kernel module for high-speed data transfer between host and the **M1** device. This enables efficient data movement with minimal CPU overhead, ideal for real-time and data intensive AI workloads. 

### Linux Device Driver Build  
After completing the environment setup of the DXNN Linux Device Driver, you can build the kernel modules using either the make(Makefile) or `build.sh` script. Both methods are supported by DEEPX.  
 
**Option 1. Build Using `Makefile`**  

**`build`**  
```
e.g $ cd modules
e.g $ make DEVICE=m1 PCIE=deepx
```

**`clean`**  
```
e.g $ cd modules
e.g $ make DEVICE=m1 PCIE=deepx clean
```

**`install`**  
Installs the driver to: `/lib/modules/$(KERNELRELEASE)/extra/`
```
e.g $ cd modules
e.g $ make DEVICE=m1 PCIE=deepx install
```

**Option 2. Build Using `build.sh`**  

Use this method if your system supports self-compiling kernel modules (`.ko` files).

**`build`** 
```
e.g $ ./build.sh -d m1 -m deepx
(Default device: m1, PCI3 module: deepx)
```

**`clean`**  
```
e.g $ ./build.sh -c clean
```

**`install`**  
Installs the driver to: `/lib/modules/$(KERNELRELEASE)/extra/`
```
e.g $ sudo ./build.sh -c install
```

### Auto-Loading Modules at Boot Time
DEEPX allows kernel modules to be automatically loaded at system boot, either through manual setup or using the `build.sh` script.  

**Manual Installation Method**  

**Step 1.** Install Kernel Modules  
Installs modules to: `/lib/modules/$(KERNELRELEASE)/extra/`
```
make DEVICE=m1 PCIE=deepx install
```

**Step 2.** Update Module Dependencies  
Updates: `/lib/modules/$(KERNELRELEASE)/modules.dep`
```
$ sudo depmod -A
```

**Step 3.** Add Module Confiduration  
Copy the preconfigured module config file.
```
$ sudo cp modules/dx_dma.conf /etc/modprobe.d/
```

This ensures the modules (`dx_dma`) are auto-loaded on boot.  

**Step 4.** Test with modprobe  
To verify the correct installation.  
``` 
$ sudo modprobe dx_dma
$ lsmod
  dxrt_driver            40960  0
  dx_dma                176128  1 dxrt_driver
```

**Automated Installation Using `build.sh`**  
The `build.sh` script automates installation and setup, including dependency updates and modprobe configuration.  

Run the following command
```
$ sudo ./build.sh -d m1 -m deepx -c install
- DEVICE        : m1
- PCIE          : deepx
- MODULE CONF   : /.../rt_npu_linux_driver/modules/dx_dma.conf
- ARCH (HOST)   : x86_64
- KERNEL        : /lib/modules/5.15.0-102-generic/build
- INSTALL       : /lib/modules/5.15.0-102-generic/extra/

 *** Build : install ***
$ make DEVICE=m1 PCIE=deepx install

make -C /lib/modules/5.15.0-102-generic/build M=/home/jhk/deepx/dxrt/module/rt_npu_linux_driver/modules  modules_install
 ....
 - SUCCESS

 *** Update : /lib/modules/5.15.0-102-generic/modules.dep ***
 $ depmod -A
 $ cp /home/jhk/deepx/dxrt/module/rt_npu_linux_driver/modules/dx_dma.conf /etc/modprobe.d/
```

**Uninstalling Modules**  
To completely remove the installed modules and configs.
```
$ ./build.sh -d m1 -m deepx -c uninstall
- DEVICE        : m1
- PCIE          : deepx
- MODULE CONF   : /.../rt_npu_linux_driver/modules/dx_dma.conf
- ARCH (HOST)   : x86_64
- KERNEL        : /lib/modules/5.15.0-102-generic/build
- INSTALL       : /lib/modules/5.15.0-102-generic/extra/

 *** Remove : /lib/modules/5.15.0-102-generic/extra ***
 $ rm -rf /lib/modules/5.15.0-102-generic/extra/pci_deepx
 $ rm -rf /lib/modules/5.15.0-102-generic/extra/rt

 *** Remove : /etc/modprobe.d ***
 $ rm /etc/modprobe.d/dx_dma.conf

 *** Update : /lib/modules/5.15.0-102-generic/modules.dep ***
 $ depmod
```

---

## Python Package Installation  
DEEPX provides a Python package for **DX-RT**, available under the module name dx-engine. It supports Python 3.8 or later and allows you to interface with **DX-RT** in Python-based applications. 

**Installation Steps**  
**1.** Navigate to the python_package directory. 
```
$ cd python_package
```

**2.** Install the package
```
$ pip install .
```

**3.** Verify the installation
```
$ pip list | grep dx
dx-engine          1.0.0
```

For details on using DX-RT with Python, refer to **Section 6.2 Python in 6. Programming Guide**.

---

## Service Registration
**DX-RT** supports multi-process operation through the background service (`dxrtd daemon`). To enable the multi-process feature, you **must** build the Runtime with Service support and the service must be registered in the system below.  

**Note.**  
  - **DX-RT** **must** be built with `USE_SERVICE=ON`. (default setting)  
  - **DX-RT** **must** be registered and managed as a system service using `systemd`.  

**Registering and Running the DX-RT Service**  
**1.** Modify the service unit file.  
  Ensure the ExecStart path is correctly configured.  
```
$ vi ./service/dxrt.service
```

**2.** Copy the service file to the system folder.  
```
$ sudo cp ./service/dxrt.service /etc/systemd/system
```

**3.** Start the service.  
```
$ sudo systemctl start dxrt.service
```

**Service Management Commands**
```
$ sudo systemctl stop dxrt.service	 	    # Stop the service
$ sudo systemctl status dxrt.service		# Check service status
$ sudo systemctl restart dxrt.service		# Restart the service
$ sudo systemctl enable dxrt.service		# Enable on boot
$ sudo systemctl disable dxrt.service		# Disable on boot
$ sudo journalctl -u dxrt.service 		    # View service logs
```

---

## Sanity Check
The Sanity Check is a script used to quickly verify that a driver has been installed correctly and that the device is recognized properly.

```
$ sudo ./SanityCheck.sh
============================================================================
==== Sanity Check Date : DATE ====
Log file location : .../dx_rt/dx_report/sanity/result/sanity_check_result_[date]_[hh/mm/ss].log

==== PCI Link-up Check ====
[OK] Vendor ID 1ff4 is present in the PCI devices.(num=2)
==== Device File Check ====
[OK] /dev/dxrt0 exists.
[OK] /dev/dxrt0 is a character device.
[OK] /dev/dxrt0 has correct permissions (0666).
[OK] /dev/dxrt1 exists.
[OK] /dev/dxrt1 is a character device.
[OK] /dev/dxrt1 has correct permissions (0666).
==== Kernel Module Check ====
[OK] dxrt_driver module is loaded.
[OK] dx_dma module is loaded.
[OK] PCIe 02:00.0 driver probe is success.
[OK] PCIe 07:00.0 driver probe is success.

============================================================================
** Sanity check PASSED!
============================================================================
```

---
