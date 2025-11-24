cmake_minimum_required(VERSION 3.15)

project(pydxrt)
set(target _pydxrt)

file(REMOVE "_pydxrt.so")

#find_package(python3.11 REQUIRED COMPONENTS Interpreter Development)

    message(STATUS ${LS_VALUE})
    set(pybind11_PYTHON_VERSION "3.11")
    message(STATUS "Found Python: ${Python_EXECUTABLE} (version ${PYBIND11_PYTHON_VERSION})")


if(MSVC)
    set(PYTHON_MODULE_EXTENSION ".pyd")
else()
    set(python_v "311")  
    if(${python_v} LESS 38)
        set(m_flag "m")
    else()
        set(m_flag "")
    endif()

    set(PYTHON_MODULE_EXTENSION ".so")
endif()

set(PYDXRT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(BINDING_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(pybind11 REQUIRED)

# include_directories(/home/output/sysroot/usr/include/python3.11)

file(GLOB BINDING_CPP_FILES "${BINDING_CPP_DIR}/*.cpp")

pybind11_add_module( ${target} 
  ${BINDING_CPP_FILES})
target_include_directories(${target} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../extern/include
  /home/output/dx_rt/lib/include
  /home/output/dx_rt/extern/include
)

set_target_properties(_pydxrt PROPERTIES
  OUTPUT_NAME "_pydxrt"
  SUFFIX ".so"
)

target_link_directories(${target} PRIVATE /home/output/dx_rt/build_aarch64/lib)

if(MSVC)
    target_link_libraries(${target} PRIVATE dxrt ${link_libs})
else()
    target_link_libraries(${target} PRIVATE dxrt pthread ${link_libs})
endif()


set(DX_PYDXRT_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Copying _pydxrt library into " ${DX_PYDXRT_TARGET_DIR})
add_custom_target(pydxrt_target ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_pydxrt> ${DX_PYDXRT_TARGET_DIR}
)

install(TARGETS _pydxrt
    LIBRARY DESTINATION ${DX_PYDXRT_TARGET_DIR}
    CONFIGURATIONS Release
)



