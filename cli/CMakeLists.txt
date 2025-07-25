
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(source_file ${SRC_FILES})
    get_filename_component(filename ${source_file} NAME_WE)

    add_target(${filename} SRC_LIST ${source_file})
endforeach()

if(NOT MSVC)

    # only available host arch compile
    if (NOT CROSS_COMPILE)
        add_subdirectory(dxtop)
    endif()
endif()
