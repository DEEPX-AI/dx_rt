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

set(onnxruntime_LIB_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/lib)
set(onnxruntime_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/include)