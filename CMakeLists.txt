cmake_minimum_required(VERSION 3.14)
project(dxrt DESCRIPTION "DEEPX Runtime SDK")

if (MSVC)
    if (DEFINED ENV{WindowsSdkDir})
        set(CMAKE_SYSTEM_PREFIX_PATH "$ENV{WindowsSdkDir}")
    else()
        set(CMAKE_SYSTEM_PREFIX_PATH "C:/Program Files (x86)/Windows Kits/10")
    endif()
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/release" CACHE PATH "Default install path" FORCE)
endif()
# set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_SOURCE_DIR}/lib/lib)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_BINARY_DIR}/lib)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "${CMAKE_SYSTEM_PROCESSOR}")
    set(CROSS_COMPILE FALSE)
elseif(("${CMAKE_HOST_SYSTEM_PROCESSOR}" STREQUAL "aarch64") AND ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "arm64"))
    set(CROSS_COMPILE FALSE)
else()
    set(CROSS_COMPILE TRUE)
endif()
if(CROSS_COMPILE)
    set(USE_PYTHON OFF)
    message(STATUS "  USE_PYTHON is disabled for cross compile case in current version.")
endif()

# cmake options
include(dxrt.cfg)

# functions
include(dxrt.function)

set(CMAKE_CXX_STANDARD_REQUIRED "ON")

if(USE_ORT)
    set(CMAKE_CXX_STANDARD "14")
else()
    set(CMAKE_CXX_STANDARD "11")
endif()

# global compile options
if (MSVC)
    add_definitions(-D_AMD64_ -D_WIN64 -DWIN64)
else()
    if (USE_SHARED_DXRT_LIB)
        add_compile_options(-W -Wall -pthread -fstrict-volatile-bitfields -fPIC)
    else()
        add_compile_options(-W -Wall -pthread -fstrict-volatile-bitfields)
    endif()
endif()

if (ENABLE_DEBUG_INFO)
    if (NOT MSVC) 
        add_compile_options(-g -rdynamic)
    endif()
endif()
if (NOT MSVC)
    add_link_options(-pthread)
endif()
file(STRINGS build.cfg build_cfg_contents REGEX "^[^/]*[^/].*")
set(build_cfg_contents "${build_cfg_contents}" CACHE STRING "global compile def.")
file(STRINGS release.ver release_ver)
foreach(cfg ${build_cfg_contents})
    add_compile_definitions(${cfg})
endforeach()

if(USE_ORT)
    add_onnxruntime()
    set(USE_ORT_DEFINE "#define USE_ORT")
else()
    set(USE_ORT_DEFINE "")
# add_compile_definitions(USE_ORT)
endif()

if(USE_SERVICE)
    set(USE_SERVICE_DEFINE "#define USE_SERVICE")
else()
    set(USE_SERVICE_DEFINE "")
endif()

#if(MSVC)
    #include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared")
#endif()

if(USE_PYTHON)
    set(PYBIND_INCLUDE_DIR extern/pybind11/include)
    set(PYBIND_TARGET_DIR extern/pybind11)
    get_pybind11()
    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    #find_package(PythonInterp ${PYBIND11_PYTHON_VERSION} REQUIRED)
    set(PYTHON_EXECUTABLE ${Python_EXECUTABLE})
    if (Python_FOUND)
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

        # Python 버전을 "X.Y" 형식으로 설정
        set(PYBIND11_PYTHON_VERSION "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
        message(STATUS "Found Python: ${Python_EXECUTABLE} (version ${PYBIND11_PYTHON_VERSION})")
    else ()
        message(FATAL_ERROR "Python not found.")
    endif ()
    include_directories(${PYBIND_INCLUDE_DIR})
    add_subdirectory(${PYBIND_TARGET_DIR} EXCLUDE_FROM_ALL)
    # find python library
    include_directories(${PYTHON_INCLUDE_DIRS})
    # include_directories(/usr/include/eigen3)
    add_compile_definitions(USE_PYTHON)
endif()

# sub-directories
add_subdirectory(lib)
add_subdirectory(cli)
if(EXISTS "${CMAKE_SOURCE_DIR}/test")
    add_subdirectory(test)
endif()

if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

# show messages
message(STATUS "  Compiler : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} (${CMAKE_CXX_COMPILER})")
message(STATUS "  Using compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "  Using c++ standard required: ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "  Using c++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Using static linker flags: ${CMAKE_STATIC_LINKER_FLAGS}")
message(STATUS "  Using shared linker flags: ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "  CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "  CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "  CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "  CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "  CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message(STATUS "  CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "  CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
message(STATUS "  CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "  USE_SHARED_DXRT_LIB: ${USE_SHARED_DXRT_LIB}")
message(STATUS "  CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "  DXRT version : ${release_ver}")

if(CROSS_COMPILE)
    message("Cross compiling")
else()
    message("Not cross compiling")
endif()

get_cmake_property(variables VARIABLES)
foreach(variable ${variables})
    if(variable MATCHES "^USE_" OR variable MATCHES "^ENABLE_")
        message(STATUS "    ${variable}: ${${variable}}")
    endif()
endforeach()