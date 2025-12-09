
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(source_file ${SRC_FILES})
    get_filename_component(filename ${source_file} NAME_WE)
    add_target(${filename} SRC_LIST ${source_file})
endforeach()

add_subdirectory(dxtop)
add_subdirectory(dxbenchmark)
