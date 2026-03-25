set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Load dxrt configuration to get USE_VNPU option
include(${CMAKE_CURRENT_LIST_DIR}/dxrt.cfg.cmake)

if (USE_VNPU)

    SET(TOOLCHAIN_PREFIX /opt/toolchain/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu)
    SET(CMAKE_C_COMPILER      ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-gcc)
    SET(CMAKE_CXX_COMPILER    ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-g++)
    SET(CMAKE_LINKER          ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-ld)
    SET(CMAKE_NM              ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-nm)
    SET(CMAKE_OBJCOPY         ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-objcopy)
    SET(CMAKE_OBJDUMP         ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-objdump)
    SET(CMAKE_RANLIB          ${TOOLCHAIN_PREFIX}/bin/aarch64-none-linux-gnu-ranlib)
    #message("ARM64 Cross-Compile for VNPU")

else()

    SET(CMAKE_C_COMPILER      /usr/bin/aarch64-linux-gnu-gcc )
    SET(CMAKE_CXX_COMPILER    /usr/bin/aarch64-linux-gnu-g++ )
    SET(CMAKE_LINKER          /usr/bin/aarch64-linux-gnu-ld  )
    SET(CMAKE_NM              /usr/bin/aarch64-linux-gnu-nm )
    SET(CMAKE_OBJCOPY         /usr/bin/aarch64-linux-gnu-objcopy )
    SET(CMAKE_OBJDUMP         /usr/bin/aarch64-linux-gnu-objdump )
    SET(CMAKE_RANLIB          /usr/bin/aarch64-linux-gnu-ranlib )
    #message("ARM64 Cross-Compile")

endif()

set(onnxruntime_LIB_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/lib)
set(onnxruntime_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/util/onnxruntime_aarch64/include)