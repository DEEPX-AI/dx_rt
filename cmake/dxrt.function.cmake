macro(get_pybind11)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/pybind11")
    set(CMAKE_TLS_VERIFY false)
    execute_process(COMMAND git clone --branch v2.12 git@github.com:pybind/pybind11.git ${CMAKE_SOURCE_DIR}/${PYBIND_TARGET_DIR})
  endif()
endmacro(get_pybind11)

macro(add_googletest target)
  if(MSVC)
    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(${target} PUBLIC GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
  else()
    # release 1.12.0
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # Specify the commit you depend on and update it regularly.
      URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    target_link_libraries(${target} gtest_main gmock gmock_main)
  endif()
endmacro(add_googletest)

macro(add_mlperf_loadgen)
  LIST(APPEND link_libs mlperf_loadgen)
endmacro(add_mlperf_loadgen)

macro(add_onnxruntime)
find_library(ONNXLIB_DIRS onnxruntime HINTS ${onnxruntime_LIB_DIRS})
include_directories(${onnxruntime_INCLUDE_DIRS})
LIST(APPEND link_libs ${ONNXLIB_DIRS})
endmacro(add_onnxruntime)

macro(add_dxrt target)
  target_include_directories(${target} PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/extern/include
  )
  message("${target} PRIVATE ${ONNXLIB_DIRS}")
  target_link_directories(${target} PRIVATE ${ONNXLIB_DIRS})
  if(MSVC)
    target_link_libraries(${target} PUBLIC dxrt ${link_libs})
  else()
    target_link_libraries(${target} dxrt pthread ${link_libs})
  endif()
endmacro(add_dxrt)

macro(add_target target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRC_LIST)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${target} ${ARG_SRC_LIST})
    add_dxrt(${target})
    install(TARGETS ${target} DESTINATION bin)
    install(TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
endmacro(add_target)

macro(add_clangtidy)
  message(STATUS "clang-tidy not found, installing...")
  execute_process(
      COMMAND sudo apt-get install -y clang-tidy
      RESULT_VARIABLE INSTALL_RESULT
      OUTPUT_QUIET ERROR_QUIET
  )
  find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
  if(CLANG_TIDY_EXE)
    message(STATUS "clang-tidy successfully installed!")
  else()
    message(FATAL_ERROR "Failed to install clang-tidy")
  endif()
endmacro(add_clangtidy)