set(target _pydxrt)

cmake_minimum_required(VERSION 3.14)

if(NOT DEFINED PYBIND11_PYTHON_VERSION)
    message(FATAL_ERROR "PYBIND11_PYTHON_VERSION is not defined. To build _pydxrt, pass python version")
endif()

if(MSVC)
    set(PYTHON_MODULE_EXTENSION ".pyd")
else()
    set(python_v "${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")  
    if(${python_v} LESS 38)
        set(m_flag "m")
    else()
        set(m_flag "")
    endif()

    set(PYTHON_MODULE_EXTENSION ".cpython-${python_v}${m_flag}-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu.so")
endif()

set(PYDXRT_DIR ${CMAKE_CURRENT_LIST_DIR})

pybind11_add_module( ${target} py_inference_engine.cpp py_configuration.cpp py_device_status.cpp)
target_include_directories(${target} PRIVATE 
  ${CMAKE_SOURCE_DIR}/lib/include
  ${CMAKE_SOURCE_DIR}/extern/include
)
if(MSVC)
    target_link_libraries(${target} PRIVATE dxrt ${link_libs})
else()
    target_link_libraries(${target} PRIVATE dxrt pthread ${link_libs})
endif()
# add_dxrt(_pydxrt)

# target_include_directories(_pydxrt
#     PRIVATE
#     ${dxrt_lib_header_dir}
#     # $<BUILD_INTERFACE:${headers}>
# )

# # Link the C++ library target with the Python module target
# if(USE_ORT)
#     target_link_libraries(_pydxrt PRIVATE 
#         dxrt_lib 
#         "${CMAKE_SOURCE_DIR}/extern/lib/${CMAKE_SYSTEM_PROCESSOR}/libprotobuf.so.23"
#         ${onnxruntime_LIBS}
#         )
# else()
#     target_link_libraries(_pydxrt PRIVATE 
#         dxrt_lib 
#         "${CMAKE_SOURCE_DIR}/extern/lib/${CMAKE_SYSTEM_PROCESSOR}/libprotobuf.so.23"
#         )
# endif()

# set(DX_PYDXRT_TARGET_DIR ${PROJECT_SOURCE_DIR}/app_python)
set(DX_PYDXRT_TARGET_DIR ${PROJECT_SOURCE_DIR}/python_package/src/dx_engine/capi)

message(STATUS "Copying _pydxrt library into " ${DX_PYDXRT_TARGET_DIR})
add_custom_target(pydxrt_target ALL
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:_pydxrt> ${DX_PYDXRT_TARGET_DIR}
)

install(TARGETS _pydxrt
    LIBRARY DESTINATION ${DX_PYDXRT_TARGET_DIR}
    CONFIGURATIONS Release
)
