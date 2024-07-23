set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

SET(CMAKE_C_COMPILER      riscv64-enx122-linux-gnu-gcc )
SET(CMAKE_CXX_COMPILER    riscv64-enx122-linux-gnu-g++ )
SET(CMAKE_LINKER          riscv64-enx122-linux-gnu-ld  )
SET(CMAKE_NM              riscv64-enx122-linux-gnu-nm )
SET(CMAKE_OBJCOPY         riscv64-enx122-linux-gnu-objcopy )
SET(CMAKE_OBJDUMP         riscv64-enx122-linux-gnu-objdump )
SET(CMAKE_RANLIB          riscv64-enx122-linux-gnu-ranlib )

set(onnxruntime_LIB_DIRS /usr/local/lib)
set(onnxruntime_INCLUDE_DIRS /usr/local/include)