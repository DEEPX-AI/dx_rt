cmake_minimum_required(VERSION 3.15)

project(pydxrt)
set(target _pydxrt)

file(REMOVE "_pydxrt.so")

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
    # Python 버전을 "X.Y" 형식으로 설정
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

find_package(pybind11 REQUIRED)



pybind11_add_module( ${target} 
  ${BINDING_CPP_DIR}/py_inference_engine.cpp ${BINDING_CPP_DIR}/py_configuration.cpp ${BINDING_CPP_DIR}/py_device_status.cpp)
target_include_directories(${target} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../extern/include
)
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



