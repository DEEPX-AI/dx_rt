cmake_minimum_required(VERSION 3.15)

project(pydxrt)
set(target _pydxrt)

file(REMOVE "_pydxrt.so")

if(NOT MSVC)
    # Linux build

    # --- Python ---
    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    # Define project root relative to current source directory
    set(DX_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")

    # --- pybind11 Configuration ---
    # Use the bundled pybind11 source located in the extern directory
    set(PYBIND11_DIR "${DX_ROOT_DIR}/extern/pybind11")

    # Verify if pybind11 exists and add its include directory
    if(EXISTS "${PYBIND11_DIR}/include/pybind11/pybind11.h")
        set(PYBIND11_INCLUDE_DIR "${PYBIND11_DIR}/include")
    else()
        message(FATAL_ERROR "pybind11 headers not found at: ${PYBIND11_DIR}/include")
    endif()

    # --- Source files ---
    set(BINDING_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(SOURCES
        ${BINDING_CPP_DIR}/py_inference_engine.cpp
        ${BINDING_CPP_DIR}/py_configuration.cpp
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
