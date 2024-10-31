## Setup build environments
* OS : Ubuntu 18.04, Ubuntu 20.04, Ubuntu 22.04 
* CMake : 3.14.0 or higher required. Refer to followings.  
```
$ sudo apt-get install zlib1g-dev libcurl4-openssl-dev
$ wget https://cmake.org/files/v3.14/cmake-3.14.0.tar.gz --no-check-certificate
$ tar xvf cmake-3.14.0.tar.gz
$ cd cmake-3.14.0
$ ./bootstrap --system-curl
$ make -j$(nproc)
$ sudo make install
```
* Ninja-build  
```
$ sudo apt install ninja-build
```
* GCC : 8.0.0 or higher required.  

* aarch64 gcc  
```
$ sudo apt-get install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```
* riscv64 gcc  
```
$ sudo apt-get install gcc-riscv64-linux-gnu g++-riscv64-linux-gnu
```
* (optional) onnxruntime  
Required if you need CPU offloading for NN ops that NPU does not support.  
Refer to following installation example for onnxruntime linux x64 v1.12.0.  
```
$ wget https://github.com/microsoft/onnxruntime/releases/download/v1.12.0/onnxruntime-linux-x64-1.12.0.tgz 
$ sudo tar -xvzf onnxruntime-linux-x64-1.12.0.tgz -C /usr/local --strip-components=1
$ sudo ldconfig 
```
Refer to [onnxruntime releases list](https://github.com/microsoft/onnxruntime/tags)    
You can use ./install.sh for onnxruntime library installation      
  ```shell
  ./install.sh --onnxruntime  
  ```        
  To specify the compilation environment as aarch64, use the **--arch** option.     
  ```shell
  ./install.sh --arch aarch64 --onnxruntime
  ```           
    
## Get software repository
Three types are provided.  
* Fetch [git repository](https://github.com/DEEPX-AI/dx_rt)
```
$ git clone git@github.com:DEEPX-AI/dx_rt.git
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
├── service
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
* service : dxrt service unit file
* test : dxrt unit test files  
  
## Install Linux Device Driver
   Please refer to [deepx driver git](https://github.com/DEEPX-AI/dx_rt_npu_linux_driver)
  
## Install python package
```
$ cd python_package
$ pip install .
* You can check dx-engine in the list of Python packages using the command below.
$ pip list | grep dx
dx-engine          0.0.1
```
  
## Register DX-RT Service
How to register DX-RT Service
```
# modify service unit file (check ExecStart path)
$ vi ./service/dxrt.service

# copy service unit file to system service folder
$ sudo cp ./service/dxrt.service /etc/systemd/system

# start service as daemon
$ sudo systemctl start dxrt.service
```
Stop the service
```
$ sudo systemctl stop dxrt.service
```

Display status of the service
```
$ sudo systemctl status dxrt.service
```

Restart the service
```
$ sudo systemctl restart dxrt.service
```

Enable the service to start automatically at boot
```
$ sudo systemctl enable dxrt.service
```

Disable the service to start automatically at boot
```
$ sudo systemctl disable dxrt.service
```

Check Logs
```
$ sudo journalctl -u dxrt.service
```