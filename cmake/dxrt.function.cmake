macro(get_pybind11)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/pybind11")
    set(CMAKE_TLS_VERIFY false)
    execute_process(COMMAND git clone --branch v2.12 git@github.com:pybind/pybind11.git ${CMAKE_SOURCE_DIR}/${PYBIND_TARGET_DIR})
  endif()
endmacro(get_pybind11)

# All headers are now consolidated in lib/include/dxrt/ (ORT-style).
# DXRT_INTERNAL_INCLUDE_DIRS kept as empty list for backward compat with
# targets that still reference it, but lib/include/ already covers everything.
set(DXRT_INTERNAL_INCLUDE_DIRS)

# ── Public-only include staging directory ──────────────────────────────────
# add_dxrt() (shared linkage) targets must only see SDK-level public headers.
# We create symlinks in a staging directory that mirrors /usr/local/include/dxrt/.
# Symlinks always reflect current source — no stale-header issues on incremental builds.
set(DXRT_PUBLIC_INCLUDE_STAGING ${CMAKE_BINARY_DIR}/public_include)
set(_DXRT_SRC_INCLUDE ${CMAKE_SOURCE_DIR}/lib/include)
set(_DXRT_EXTERN_INCLUDE ${CMAKE_SOURCE_DIR}/extern/include)

file(MAKE_DIRECTORY ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt)
file(MAKE_DIRECTORY ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt/exception)

# Core headers — symlink
foreach(_hdr dxrt_c_api.h dxrt_cxx_api.h gen.h)
    file(CREATE_LINK ${_DXRT_SRC_INCLUDE}/dxrt/${_hdr}
         ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt/${_hdr} SYMBOLIC)
endforeach()

# Legacy headers (installed flat as dxrt/*.h for backward compat) — symlink each
file(GLOB _LEGACY_HEADERS ${_DXRT_SRC_INCLUDE}/dxrt/legacy/*.h)
foreach(_hdr ${_LEGACY_HEADERS})
    get_filename_component(_fname ${_hdr} NAME)
    file(CREATE_LINK ${_hdr}
         ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt/${_fname} SYMBOLIC)
endforeach()

# Exception subdir — symlink
file(CREATE_LINK ${_DXRT_SRC_INCLUDE}/dxrt/legacy/exception/exception.h
     ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt/exception/exception.h SYMBOLIC)

# Extern vendored headers (cxxopts, rapidjson) — symlink the directory
file(CREATE_LINK ${_DXRT_EXTERN_INCLUDE}/dxrt/extern
     ${DXRT_PUBLIC_INCLUDE_STAGING}/dxrt/extern SYMBOLIC)

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
    target_link_libraries(${target} PRIVATE gtest_main gmock gmock_main)
  endif()
endmacro(add_googletest)

macro(add_mlperf_loadgen)
  LIST(APPEND link_libs mlperf_loadgen)
endmacro(add_mlperf_loadgen)

macro(add_onnxruntime)
find_library(ONNXLIB_DIRS onnxruntime HINTS ${onnxruntime_LIB_DIRS})
include_directories(${onnxruntime_INCLUDE_DIRS})
LIST(APPEND link_libs ${ONNXLIB_DIRS})
if(MSVC)
    LIST(APPEND link_libs ${onnxruntime_LIB_DIRS}/onnxruntime.lib)
endif()
endmacro(add_onnxruntime)

macro(add_dxrt target)
  # Shared-link targets see only public SDK headers (mirrors prebuilt install).
  # Internal headers are NOT accessible — use add_dxrt_static() if needed.
  target_include_directories(${target} PUBLIC
    ${DXRT_PUBLIC_INCLUDE_STAGING}
  )
  target_link_directories(${target} PRIVATE ${ONNXLIB_DIRS})
  if(MSVC)
    if(NOT USE_SHARED_DXRT_LIB)
      target_compile_definitions(${target} PRIVATE DXRT_STATIC)
    endif()
    target_link_libraries(${target} PUBLIC dxrt ${link_libs})
  else()
    target_link_libraries(${target} PUBLIC dxrt pthread ${link_libs})
  endif()
endmacro(add_dxrt)

macro(add_dxrt_static target)
  target_include_directories(${target} PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/extern/include
  )
  target_include_directories(${target} PRIVATE ${DXRT_INTERNAL_INCLUDE_DIRS})
  target_link_directories(${target} PRIVATE ${ONNXLIB_DIRS})
  if(MSVC)
    # Windows: dxrt is already static, no separate dxrt_static target
    if(NOT USE_SHARED_DXRT_LIB)
      target_compile_definitions(${target} PRIVATE DXRT_STATIC)
    endif()
    target_link_libraries(${target} PUBLIC dxrt ${link_libs})
  else()
    target_link_libraries(${target} PUBLIC dxrt_static pthread rt ${link_libs})
  endif()
endmacro(add_dxrt_static)

macro(add_target target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRC_LIST)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${target} ${ARG_SRC_LIST})
    add_dxrt_static(${target})
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
