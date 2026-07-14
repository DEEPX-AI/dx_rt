cmake_minimum_required(VERSION 3.15)

project(pydxrt)
set(target _pydxrt)

file(REMOVE "_pydxrt.so")

if(NOT MSVC)
    # Linux build

    # --- Python ---
    # Development.Module (not full Development) so manylinux Pythons — built
    # without --enable-shared and therefore lacking libpython.so — still
    # satisfy the requirement. Extension modules never link libpython.
    find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

    # Define project root relative to current source directory
    # Override with -DDX_ROOT_DIR=... when building outside the normal
    # dx_rt_fork tree (e.g. cibuildwheel stages lib/include and extern/include
    # inside /project and passes DX_ROOT_DIR=/project).
    if(NOT DEFINED DX_ROOT_DIR)
        set(DX_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")
    endif()

    # --- pybind11 Configuration ---
    # 1st: pip build dependency (PEP 517 isolated build, pybind11 in build-system.requires)
    find_package(pybind11 CONFIG QUIET)
    if(pybind11_FOUND)
        if(NOT pybind11_INCLUDE_DIR)
            message(FATAL_ERROR "find_package found pybind11 but pybind11_INCLUDE_DIR is empty. "
                                "Reinstall pybind11: pip install --force-reinstall pybind11")
        endif()
        set(PYBIND11_INCLUDE_DIR "${pybind11_INCLUDE_DIR}")
        message(STATUS "Found pybind11 via cmake package (v${pybind11_VERSION}): ${PYBIND11_INCLUDE_DIR}")
    else()
        # 2nd: bundled pybind11 at dx_rt/extern/pybind11 (full repo build)
        set(PYBIND11_DIR "${DX_ROOT_DIR}/extern/pybind11")
        if(EXISTS "${PYBIND11_DIR}/include/pybind11/pybind11.h")
            set(PYBIND11_INCLUDE_DIR "${PYBIND11_DIR}/include")
            message(STATUS "Found bundled pybind11: ${PYBIND11_INCLUDE_DIR}")
        else()
            message(FATAL_ERROR "pybind11 headers not found. Neither find_package(pybind11) nor bundled "
                                "path '${PYBIND11_DIR}/include' resolved. Install with: pip install pybind11")
        endif()
    endif()

    # --- Source files ---
    set(BINDING_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(SOURCES
        ${BINDING_CPP_DIR}/py_inference_engine.cpp
        ${BINDING_CPP_DIR}/py_configuration.cpp
        ${BINDING_CPP_DIR}/py_profiler.cpp
        ${BINDING_CPP_DIR}/py_device_status.cpp
        ${BINDING_CPP_DIR}/py_runtime_event_dispatcher.cpp
    )

    # --- Build as Python extension module ---
    # Uses manual PEP 489 multi-phase init
    # machinery for other C extensions (e.g., matplotlib.ft2font).
    Python_add_library(${target} MODULE ${SOURCES})

    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
    )

    # --- Include directories ---
    # Equivalent to:
    #   -I extern/pybind11/include
    #   -I $(python3 -c "import sysconfig; print(sysconfig.get_path('include'))")
    #   -I lib/include
    #   -I extern/include
    target_include_directories(${target} PRIVATE
        ${PYBIND11_INCLUDE_DIR}
        ${Python_INCLUDE_DIRS}
        ${DX_ROOT_DIR}/lib/include
        ${DX_ROOT_DIR}/extern/include
    )

    # --- Compile definitions ---
    # Equivalent to: -D_GLIBCXX_USE_CXX11_ABI=1
    target_compile_definitions(${target} PRIVATE _GLIBCXX_USE_CXX11_ABI=1)

    if (Python_EXECUTABLE MATCHES "pypy")
        target_compile_definitions(${target} PRIVATE PYBIND11_IS_PYPY)
    endif()

    # --- Link libraries ---
    # Equivalent to: -ldxrt -lpthread
    target_link_libraries(${target} PRIVATE
        dxrt
        pthread
        ${link_libs}
    )

    # --- Visibility ---
    target_compile_options(${target} PRIVATE -fvisibility=hidden)

    # --- Size optimization ---
    # scikit-build-core defaults CMAKE_BUILD_TYPE to Release; enforce it so
    # standalone cmake invocations (e.g. developer builds) also opt in.
    # -ffunction-sections/-fdata-sections + --gc-sections drops unreferenced
    # code/data. --strip-all removes the symbol table from _pydxrt.so. The
    # bundled prebuilt libs are stripped separately by `auditwheel repair
    # --strip` during the cibuildwheel repair step.
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    endif()
    target_compile_options(${target} PRIVATE
        $<$<CONFIG:Release>:-ffunction-sections -fdata-sections>
    )
    target_link_options(${target} PRIVATE
        $<$<CONFIG:Release>:-Wl,--gc-sections -Wl,--strip-all>
    )

    # --- Install (for scikit-build-core / pip install) ---
    install(TARGETS ${target}
        LIBRARY DESTINATION dx_engine/capi
        RUNTIME DESTINATION dx_engine/capi
    )

    # --- Copy to source dir (for standalone cmake build / development) ---
    set(DX_PYDXRT_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_target(pydxrt_target ALL
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}> ${DX_PYDXRT_TARGET_DIR}
    )

else()
    # Windows MSVC build

    if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8) # detect x64 using pointer size
        message(FATAL_ERROR "Please use Visual Studio x64 Native Tools Command Prompt")
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


    set(PYTHON_MODULE_EXTENSION ".pyd")
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
    ${BINDING_CPP_DIR}/py_profiler.cpp
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

    target_link_libraries(${target} PRIVATE ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/lib/dxrt.lib ${link_libs})

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
    install(FILES 
        ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/bin/dxrt.dll
        ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/bin/onnxruntime.dll
        ${CMAKE_SOURCE_DIR}/../../../../out/install/x64-Release/bin/onnxruntime_providers_shared.dll
        DESTINATION ${DX_PYDXRT_TARGET_DIR}
        CONFIGURATIONS Release
        OPTIONAL
    )
endif()
