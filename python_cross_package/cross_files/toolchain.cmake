# message("ARM64 Cross-Compile")
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

SET(CMAKE_C_COMPILER      /usr/bin/aarch64-linux-gnu-gcc )
SET(CMAKE_CXX_COMPILER    /usr/bin/aarch64-linux-gnu-g++ )
SET(CMAKE_LINKER          /usr/bin/aarch64-linux-gnu-ld  )
SET(CMAKE_NM              /usr/bin/aarch64-linux-gnu-nm )
SET(CMAKE_OBJCOPY         /usr/bin/aarch64-linux-gnu-objcopy )
SET(CMAKE_OBJDUMP         /usr/bin/aarch64-linux-gnu-objdump )
SET(CMAKE_RANLIB          /usr/bin/aarch64-linux-gnu-ranlib )

#SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_C_COMPILER})
#SET(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_CXX_COMPILER})

set(onnxruntime_LIB_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/lib)
set(onnxruntime_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/include)

SET(CMAKE_SYSROOT /home/output/sysroot)
SET(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(Python_EXECUTABLE ${CMAKE_SYSROOT}/usr/bin/python3.11)
SET(Python_INCLUDE_DIRS ${CMAKE_SYSROOT}/usr/include/python3.11)
SET(Python_LIBRARIES ${CMAKE_SYSROOT}/usr/lib/aarch64-linux-gnu/libpython3.11.so)
SET(pybind11_DIR "${CMAKE_SYSROOT}/home/dxdemo/.local/lib/python3.11/site-packages/pybind11/share/cmake/pybind11/")
