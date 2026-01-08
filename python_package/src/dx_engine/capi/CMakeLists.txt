cmake_minimum_required(VERSION 3.15)

project(pydxrt)
set(target _pydxrt)

file(REMOVE "_pydxrt.so")
if (MSVC)
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8) # detect x64 using pointer size
    message(FATAL_ERROR "Please use Visual Studio x64 Native Tools Command Prompt")
endif()
endif()

find_package(Python COMPONENTS Interpreter Development REQUIRED)
set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})

    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import sys; print(sys.version_info.major)"
        OUTPUT_VARIABLE PYTHON_VERSION_MAJOR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import sys; print(sys.version_info.minor)"
        OUTPUT_VARIABLE PYTHON_VERSION_MINOR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS ${LS_VALUE})
    set(PYBIND11_PYTHON_VERSION "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
    message(STATUS "Found Python: ${Python_EXECUTABLE} (version ${PYBIND11_PYTHON_VERSION})")


if(MSVC)
    set(PYTHON_MODULE_EXTENSION ".pyd")
else()
    set(python_v "${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}")
    if(${python_v} LESS 38)
        set(m_flag "m")
    else()
        set(m_flag "")
    endif()

    set(PYTHON_MODULE_EXTENSION ".so")
endif()

set(PYDXRT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(BINDING_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})



include_directories(
  ${CMAKE_SOURCE_DIR}/../../../../lib/include
  ${CMAKE_SOURCE_DIR}/../../../../extern/include
)

find_package(pybind11 REQUIRED)


pybind11_add_module( ${target}
  ${BINDING_CPP_DIR}/py_inference_engine.cpp
  ${BINDING_CPP_DIR}/py_configuration.cpp
  ${BINDING_CPP_DIR}/py_device_status.cpp
  ${BINDING_CPP_DIR}/py_runtime_event_dispatcher.cpp)

target_include_directories(${target} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../extern/include
)
if (Python_EXECUTABLE MATCHES "pypy")
    # Define C++ macro to indicate PyPy.
    target_compile_definitions(${target} PRIVATE PYBIND11_IS_PYPY)
endif()

if(MSVC)
    target_link_libraries(${target} PRIVATE ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/lib/dxrt.lib ${link_libs})
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

# Install required DLLs to capi directory
if(MSVC)
    install(FILES 
        ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/bin/dxrt.dll
        ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/bin/onnxruntime.dll
        DESTINATION ${DX_PYDXRT_TARGET_DIR}
        CONFIGURATIONS Release
        OPTIONAL
    )
endif()



