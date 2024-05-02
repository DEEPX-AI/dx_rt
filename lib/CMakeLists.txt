# DXRT LIB
set(target dxrt)
if (USE_SHARED_DXRT_LIB)
    set(dxrt_lib_file "${CMAKE_CURRENT_SOURCE_DIR}/lib/libdxrt_${CMAKE_SYSTEM_PROCESSOR}.so"
    CACHE STRING "dxrt lib. file")
else()
    set(dxrt_lib_file "${CMAKE_CURRENT_SOURCE_DIR}/lib/libdxrt_${CMAKE_SYSTEM_PROCESSOR}.a"
        CACHE STRING "dxrt lib. file")
endif()

FILE(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
FILE(GLOB_RECURSE headers "include/dxrt/*.h")

## Remove pybinding source files
list(REMOVE_ITEM srcs "${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/src/py_inference_engine.cpp")
message("DXRT LIB Source files : " ${srcs})
message("DXRT LIB Header files : " ${headers})
if (USE_SHARED_DXRT_LIB)
    add_library(${target} SHARED ${srcs})
else()
    add_library(${target} STATIC ${srcs})
endif()
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/cfg/common.cfg cfgs)
foreach(cfg ${cfgs})
    target_compile_definitions(${target} PUBLIC "${cfg}")
endforeach()
target_include_directories(
    ${target} 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/extern/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
)
# if (USE_PYTHON)
#     set_target_properties(${target} PROPERTIES
#         POSITION_INDEPENDENT_CODE ON
#         PUBLIC_HEADER "${dxrt_lib_public_headers}"
#     )
# else()
#     set_target_properties(${target} PROPERTIES
#         PUBLIC_HEADER "${dxrt_lib_public_headers}"
#     OUTPUT_NAME "dxrt_${CMAKE_SYSTEM_PROCESSOR}"
#     )
# endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/dxrt/gen.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/dxrt/gen.h @ONLY)

add_subdirectory(bindings)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/dxrtConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dxrt
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/dxrtConfigVersion.cmake
    VERSION ${release_ver}
    COMPATIBILITY SameMajorVersion
)

install(
    TARGETS ${target}
    EXPORT ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/dxrt
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/dxrt DESTINATION include)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extern/include/rapidjson DESTINATION include)
install(FILES ${CMAKE_SOURCE_DIR}/extern/include/cxxopts.hpp DESTINATION include)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dxrtConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/dxrtConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dxrt
)
