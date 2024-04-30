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

## 3rd Party Libraries
1. googleprotobuf, googletest  
    Prebuilt files are provided in ```extern/include```, ```extern/lib``` directory. If you want to use your custom files, overwrite them.  
2. OpenCV  
    Prebuilt files are not provided. CMake ```find_package()``` will find OpenCV packages automatically from your system. If you want to set specific OpenCV package path, configure following at ```cmake/toolchain.<CMAKE_SYSTEM_PROCESSOR>.cmake``` - just set the path which includes ```OpenCVConfig.cmake``` file. 
```
set(OpenCV_DIR <opencv build dir.> )
```
Example : OpenCV 4.5.5 Cross-compile for arm64  
```
wget -O opencv.4.5.5.zip https://github.com/opencv/opencv/archive/4.5.5.zip 
wget -O opencv_contrib.4.5.5.zip https://github.com/opencv/opencv_contrib/archive/4.5.5.zip
unzip opencv.4.5.5.zip
unzip opencv_contrib.4.5.5.zip
cd opencv-4.5.5
mkdir build_arm64 ; cd build_arm64
cmake -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/aarch64-gnu.toolchain.cmake -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.5/modules -D CMAKE_INSTALL_PREFIX=../install_arm64 -DCMAKE_CXX_COMPILER=/usr/bin/aarch64-linux-gnu-g++ -DCMAKE_C_COMPILER=/usr/bin/aarch64-linux-gnu-gcc ../
make -j8
sudo make install
```
Example : OpenCV 4.5.5 Cross-compile for riscv64  
```
wget -O opencv.4.5.5.zip https://github.com/opencv/opencv/archive/4.5.5.zip 
wget -O opencv_contrib.4.5.5.zip https://github.com/opencv/opencv_contrib/archive/4.5.5.zip
unzip opencv.4.5.5.zip
unzip opencv_contrib.4.5.5.zip
cd opencv-4.5.5
mkdir build_riscv64 ; cd build_riscv64
cmake -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/riscv64-gnu.toolchain.cmake -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.5/modules -D CMAKE_INSTALL_PREFIX=../install_riscv64 -DCMAKE_CXX_COMPILER=riscv64-linux-gnu-g++ -DCMAKE_C_COMPILER=riscv64-linux-gnu-gcc ../
make -j8
sudo make install
```
You can install opencv using ```install.sh```, as followings. OpenCV 4.5.5 version will be installed in ```extern/${cpu_arch}```.     
```
./install.sh --arch riscv64 --opencv
./install.sh --arch arm64 --opencv
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
Demo application binary files can be found in `<build directory>`/release/bin/, and `bin`.  
```
<build directory>/release/bin/
├── cls_demo_bin
├── ddrnet
├── dxrt_test
├── face_recognition
├── helloworld
├── parse_model
├── pidnet
├── run_model
├── ssd
├── yolo
├── yolo_ddrnet
└── yolo_pidnet
```
