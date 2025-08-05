cmake_minimum_required(VERSION 3.14)

if(WIN32)
    if(NOT DEFINED ONNXRUNTIME_ROOTDIR OR NOT EXISTS ${ONNXRUNTIME_ROOTDIR})
        set(ONNXRUNTIME_ROOTDIR "C:/DeepX/onnxruntime"
            CACHE STRING "")
    endif()

    if(NOT DEFINED onnxruntime_LIB_DIRS OR NOT EXISTS ${onnxruntime_LIB_DIRS})
        set(onnxruntime_LIB_DIRS "${ONNXRUNTIME_ROOTDIR}/lib"
            CACHE STRING "")
    endif()

    if(NOT DEFINED onnxruntime_INCLUDE_DIRS OR NOT EXISTS ${onnxruntime_INCLUDE_DIRS})
        set(onnxruntime_INCLUDE_DIRS "${ONNXRUNTIME_ROOTDIR}/include"
            CACHE STRING "")
    endif()
endif()

project(dxrt DESCRIPTION "DEEPX Runtime SDK")

set(CMAKE_CXX_STANDARD_REQUIRED "ON")
set(CMAKE_CXX_STANDARD "11")
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
if (${USE_CLANG})
    include(dxrt.clang)
endif()

# functions
include(dxrt.function)

set(CMAKE_CXX_STANDARD_REQUIRED "ON")

if(USE_ORT)
    set(CMAKE_CXX_STANDARD "14")
else()
    #set(CMAKE_CXX_STANDARD "11")
    set(CMAKE_CXX_STANDARD "14")
endif()

# global compile options
if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DWIN32 /D_WINDOWS /W3 /GR /EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /W3 /GR /EHsc")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /DEBUG:FULL")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG:FULL")
    
    add_compile_options(-D_CRT_SECURE_NO_WARNINGS)

    add_definitions(-D_AMD64_ -D_WIN64 -DWIN64)

    # Set the runtime library based on the option
    if(USE_MT)
        # Runtime library setting : /MT
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

        # Interface library for runtime setting
        add_library(runtime_settings INTERFACE)
        target_compile_options(runtime_settings INTERFACE
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
        )
    else()
        # Runtime library setting : /MD
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:MultiThreadedDebugDLL>")

        # Interface library for runtime setting
        add_library(runtime_settings INTERFACE)
        target_compile_options(runtime_settings INTERFACE
            $<$<CONFIG:Debug>:/MDd>
            $<$<CONFIG:Release>:/MD>
        )
    endif()

    # WIN SDK default setting   
    set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Windows Kits/10")
    set(WINDOWS_SDK_VERSION "10.0.22621.0")

    # set windows sdk lib path        
    set(WINDOWS_SDK_LIB_PATH "${WINDOWS_SDK_PATH}/Lib/${WINDOWS_SDK_VERSION}/um/x64")
    link_directories(${WINDOWS_SDK_LIB_PATH})

    # set include dir
    include_directories("${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/um")
    include_directories("${WINDOWS_SDK_PATH}/Include/${WINDOWS_SDK_VERSION}/ucrt")

    # link windows lib
    link_libraries(
        kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32
    )

    # link Visual C++ runtime lib
    link_libraries(
        $<$<CONFIG:Debug>:msvcrtd> 
        $<$<CONFIG:Release>:msvcrt>
        $<$<CONFIG:Debug>:msvcprtd> 
        $<$<CONFIG:Release>:msvcprt>
    )
else()
    add_compile_options(-W -Wall -pthread)
    if (NOT (${USE_CLANG}))
        add_compile_options(-fstrict-volatile-bitfields)
    endif()
    if (USE_SHARED_DXRT_LIB)
        add_compile_options(-fPIC)
    endif()
endif()

if (ENABLE_DEBUG_INFO)
    if (NOT MSVC) 
        add_compile_options(-g)
        if (${USE_CLANG})
        else()
            add_compile_options(-rdynamic)
        endif()
    endif()
endif()
if (NOT MSVC)
    add_link_options(-pthread)
endif()
set(build_cfg_contents "${build_cfg_contents}" CACHE STRING "global compile def.")
file(STRINGS release.ver release_ver)
foreach(cfg ${build_cfg_contents})
    add_compile_definitions(${cfg})
endforeach()

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    message(STATUS "Enabling AddressSanitizer (ASAN)")
    set(ASAN_FLAGS "-fsanitize=address -static-libasan -fno-omit-frame-pointer -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${ASAN_FLAGS}")
endif()

# Coverage configuration
option(ENABLE_COVERAGE "Enable code coverage collection" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage collection is ENABLED")
    
    # Coverage flags for different compilers
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang coverage flags
        set(COVERAGE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping --coverage -g")
        set(COVERAGE_C_FLAGS "-fprofile-instr-generate -fcoverage-mapping --coverage -g")
        set(COVERAGE_LINKER_FLAGS "--coverage")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # GCC coverage flags
        set(COVERAGE_CXX_FLAGS "-fprofile-arcs -ftest-coverage --coverage -g -O0")
        set(COVERAGE_C_FLAGS "-fprofile-arcs -ftest-coverage --coverage -g -O0")
        set(COVERAGE_LINKER_FLAGS "--coverage")
    else()
        message(WARNING "Coverage is only supported for GCC and Clang compilers")
    endif()
    
    # Apply coverage flags
    if(COVERAGE_CXX_FLAGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_C_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINKER_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COVERAGE_LINKER_FLAGS}")
        
        # Add coverage compile definition
        add_compile_definitions(COVERAGE_ENABLED)
    endif()
    
    # Function to clean coverage files
    function(clean_coverage_files)
        file(GLOB_RECURSE GCDA_FILES "${CMAKE_BINARY_DIR}/*.gcda")
        file(GLOB_RECURSE GCNO_FILES "${CMAKE_BINARY_DIR}/*.gcno")
        file(GLOB_RECURSE PROFRAW_FILES "${CMAKE_BINARY_DIR}/*.profraw")
        file(GLOB_RECURSE PROFDATA_FILES "${CMAKE_BINARY_DIR}/*.profdata")
        
        foreach(file ${GCDA_FILES} ${GCNO_FILES} ${PROFRAW_FILES} ${PROFDATA_FILES})
            file(REMOVE ${file})
        endforeach()
    endfunction()
    
    # Add custom target for cleaning coverage files
    add_custom_target(clean-coverage
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning coverage files..."
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.gcda
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.gcno
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.profraw
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.profdata
        COMMENT "Cleaning coverage files"
    )
endif()

if(USE_ORT)
    add_onnxruntime()
    set(USE_ORT_DEFINE "#define USE_ORT")
else()
    set(USE_ORT_DEFINE "")
endif()

if(USE_SERVICE)
    set(USE_SERVICE_DEFINE "#define USE_SERVICE")
else()
    set(USE_SERVICE_DEFINE "")
endif()

if(ENABLE_SHOW_MODEL_INFO)
    add_definitions(-DSHOW_MODEL_INFO_DEFINE)
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
add_subdirectory(examples)
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