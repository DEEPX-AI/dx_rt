## How to Create a CMake Project Using DXRT  
This guide provides step-by-step instructions on creating a new CMake project using the DXRT library.  
1. Build DXRT library  
Refer to Installation, Build chapter for instructions on fetching and building the DXRT source code.  
2. Create a new CMake project  
Create a new CMake project by making a project directory and creating a `CMakeLists.txt` file.  
```
mkdir MyProject
cd MyProject
touch CMakeLists.txt
```
3. Helloworld with DXRT API  
Let's write a simple hello world app. that calls the DXRT API.  
main.cpp:  
```
#include "dxrt/dxrt_api.h"

using namespace std;

int main(int argc, char *argv[])
{
    auto devices = dxrt::CheckDevices();
    cout << "hello, world" << endl;
    return 0;
}
```
4. Modify CMakeLists.txt
Open and modify `CMakeLists.txt` file as follows.
```
cmake_minimum_required(VERSION 3.14)
project(app_template)
set(CMAKE_CXX_STANDARD_REQUIRED "ON")
set(CMAKE_CXX_STANDARD "17")

# Specify the installation path where the DXRT library is installed.
set(DXRT_LIB_PATH "/usr/local/lib") # Adjust this path to the actual installation path of DXRT library.

# Find and retrieve the DXRT library.
find_library(DXRT_LIBRARY REQUIRED NAMES dxrt_${CMAKE_SYSTEM_PROCESSOR} PATHS ${DXRT_LIB_PATH})

# add google protobuf library.
add_library(protobuf SHARED IMPORTED)
set_target_properties(protobuf PROPERTIES
  IMPORTED_LOCATION "${DXRT_LIB_PATH}/libprotobuf.so.23"  
)

# Add project source files and executable.
add_executable(HelloWorld main.cpp)

# Link the DXRT library to the executable.
target_link_libraries(HelloWorld PRIVATE ${DXRT_LIBRARY} protobuf)
```
Set `DXRT_LIB_PATH` with the actual path where the DXRT library is installed.  
5. Build the Project
```
mkdir build
cd build
cmake ..
make
```
6. Run the Executable
After a successful build, run the generated executable.  

Now, you have created a new CMake project using the DXRT library!  