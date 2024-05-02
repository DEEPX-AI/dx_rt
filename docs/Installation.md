## Setup build environments
* OS : Ubuntu 18.04, Ubuntu 20.04  
* CMake : 3.14.0 or higher required. Refer to followings.  
```
sudo apt-get install zlib1g-dev libcurl4-openssl-dev
wget https://cmake.org/files/v3.14/cmake-3.14.0.tar.gz --no-check-certificate
tar xvf cmake-3.14.0.tar.gz
cd cmake-3.14.0
./bootstrap --system-curl
make -j8
sudo make install
```
* Ninja-build  
```
sudo apt install ninja-build
```
* GCC : 8.0.0 or higher required.  

arm64 gcc (for DX_M1)  
```
sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```
riscv64 gcc (for DX_L2)  
```
sudo apt-get install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
```
riscv64-eyenix gcc (for DX_L1)  
```
tar xvf riscv64_lp64d_enx122_linux_cross_2023-01-20.tgz
cd cross_toolchain
./cross_install
source /etc/profile
```
* (optional) onnxruntime  
Required if you need CPU offloading for NN ops that NPU does not support.  
Refer to following installation example for onnxruntime v1.16.3.  
```
git clone --recursive https://github.com/Microsoft/onnxruntime.git
cd onnxruntime
git checkout -b _v1.16.3 v1.16.3
./build.sh --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync
sudo make install
```
Refer to [onnxruntime Build Guide](https://onnxruntime.ai/docs/build/inferencing.html)
## Get software repository
Three types are provided.  
* Fetch [git repository](https://github.com/DEEPX-AI/dx_rt)
```
git clone git@github.com:DEEPX-AI/dx_rt.git
```
## File structure
```
├── cli
├── build.cfg
├── build.sh
├── cmake
├── docs
├── extern
├── lib
├── python_package
├── sample
└── test
```

* cli : applications
* build.cfg : C/C++ preprocessor macros to use in application  
* build.sh : build shell script  
* cmake : required cmake scripts  
* docs : markdown files, and doxygen generator for API reference  
* extern : 3rd party library files  
* lib : prebuilt dxrt libarary files  
* sample : sample images for demo applications  
* test : dxrt unit test files  

## Install Linux Device Driver
   Please refer to [deepx driver git](https://github.com/DEEPX-AI/dx_rt_npu_linux_driver)
