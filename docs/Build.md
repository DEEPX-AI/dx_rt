## Compile Definitions
You can set global compile definitions in build.cfg to use in your application(It doesn't affect to DXRT libraries).  
Following means, `#define EXAMPLE_FLAG 1` in compile time.  
```
EXAMPLE_FLAG=1
```

## Options
You can configure DXRT options in cmake/dxrt.cfg.cmake  
`USE_ORT` : Use ONNX runtime for NN ops that NPU does not support.  
`USE_DXRT_TEST` : (applicable for only DEEPX-internal version) Build DXRT Unit test. Prebult googletest libraries are linked if this option is `ON`.  
`USE_PYTHON` : Enable python RT.  

```
option(USE_ORT "Use ONNX Runtime" OFF)
option(USE_DXRT_TEST "Use DXRT Unit Test" ON)
option(USE_SHARED_DXRT_LIB "Build for DXRT Shared Library" ON)
option(USE_PYTHON "Use Python" OFF)
```

## Cross-compile Setup
DXRT supports 3 kinds of CPU Architecture - arm64, riscv64, x86_64.  
Set compiler toolchain path in ```cmake/toolchain.<CMAKE_SYSTEM_PROCESSOR>.cmake```  
Example : arm64 cross-compile  
```
SET(CMAKE_C_COMPILER      /usr/bin/aarch64-linux-gnu-gcc )
SET(CMAKE_CXX_COMPILER    /usr/bin/aarch64-linux-gnu-g++ )
SET(CMAKE_LINKER          /usr/bin/aarch64-linux-gnu-ld  )
SET(CMAKE_NM              /usr/bin/aarch64-linux-gnu-nm )
SET(CMAKE_OBJCOPY         /usr/bin/aarch64-linux-gnu-objcopy )
SET(CMAKE_OBJDUMP         /usr/bin/aarch64-linux-gnu-objdump )
SET(CMAKE_RANLIB          /usr/bin/aarch64-linux-gnu-ranlib )
```
Example : riscv64 cross-compile  
```
set(CMAKE_C_COMPILER /usr/bin/riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/riscv64-linux-gnu-g++)
```
## Build guide

### non-cross-compile case
To build DXRT for the host system, execute the build script with the appropriate installation path.  
It is recommended to set the installation path to a location commonly referenced in the OS, such as `/usr/local`.  
```
./build.sh --install /usr/local
```
### cross-compile case
To cross-compile DXRT for a target system, target CPU architecture should be configured.  
(If not specified, target CPU will be set to the host system's CPU)  
```
./build.sh --arch <target_cpu>
```

Please refer to following options.  
```
./build.sh [ options ]
    --help     show this help
    --clean    clean build
    --verbose  show build commands
    --type     cmake build type : [ Release, Debug, RelWithDebInfo ]
    --arch     target CPU architecture : [ x86_64, arm64, riscv64 ]
    --install  install path
    --uninstall  uninstall dx-rt files
```
Examples:
```
./build.sh --arch arm64
./build.sh --arch riscv64
./build.sh --arch x86_64
```
Build directories and related output files will be generated according to the target CPU (e.g., `build_arm64`, `build_riscv64`, ...).  
Demo application binary files can be found in `<build directory>`/bin/, and `bin`.  
```
<build directory>/bin/
├── dxrt-cli
├── run_model
├── parse_model
```
