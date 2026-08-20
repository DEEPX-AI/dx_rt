
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(source_file ${SRC_FILES})
    get_filename_component(filename ${source_file} NAME_WE)
    if(filename STREQUAL "dxparse" OR filename STREQUAL "dxrun" OR filename STREQUAL "dxrtd")
        # These binaries use only the public C/C++ API — link against shared library.
        add_executable(${filename} ${source_file})
        add_dxrt(${filename})
        install(TARGETS ${filename} DESTINATION bin)
        install(TARGETS ${filename} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
    else()
        # Other CLI tools need C++ internals — link against static library.
        add_target(${filename} SRC_LIST ${source_file})
    endif()
endforeach()

if(WIN32)
    add_executable(dxrt-cli ${CMAKE_CURRENT_SOURCE_DIR}/dxcli.cpp)
    add_dxrt_static(dxrt-cli)
    install(TARGETS dxrt-cli DESTINATION bin)
    install(TARGETS dxrt-cli DESTINATION ${CMAKE_SOURCE_DIR}/bin)

    add_executable(parse_model ${CMAKE_CURRENT_SOURCE_DIR}/dxparse.cpp)
    add_dxrt(parse_model)
    install(TARGETS parse_model DESTINATION bin)
    install(TARGETS parse_model DESTINATION ${CMAKE_SOURCE_DIR}/bin)

    add_executable(run_model ${CMAKE_CURRENT_SOURCE_DIR}/dxrun.cpp)
    add_dxrt(run_model)
    install(TARGETS run_model DESTINATION bin)
    install(TARGETS run_model DESTINATION ${CMAKE_SOURCE_DIR}/bin)

    if(TARGET dxcli-internal)
        add_executable(dxrt-cli-internal ${CMAKE_CURRENT_SOURCE_DIR}/dxcli-internal.cpp)
        add_dxrt_static(dxrt-cli-internal)
        install(TARGETS dxrt-cli-internal DESTINATION bin)
        install(TARGETS dxrt-cli-internal DESTINATION ${CMAKE_SOURCE_DIR}/bin)
    endif()

    add_dxrt_windows_version_info(dxcli "DX Command Line Interface")
    add_dxrt_windows_version_info(dxrt-cli "DX Runtime CLI")
    add_dxrt_windows_version_info(dxrtd "DX Runtime Service")
    add_dxrt_windows_version_info(dxparse "DX Model Parser")
    add_dxrt_windows_version_info(parse_model "DX Model Parser")
    add_dxrt_windows_version_info(dxrun "DX Model Runner")
    add_dxrt_windows_version_info(run_model "DX Model Runner")
    if(TARGET dxcli-internal)
        add_dxrt_windows_version_info(dxcli-internal "DX Command Line Interface Internal")
        add_dxrt_windows_version_info(dxrt-cli-internal "DX Runtime CLI Internal")
    endif()
else()
    function(add_build_compat_alias target old_name)
        if(NOT TARGET ${target})
            return()
        endif()
        set(_old_file "${old_name}${CMAKE_EXECUTABLE_SUFFIX}")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove -f "$<TARGET_FILE_DIR:${target}>/${_old_file}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${target}>" "$<TARGET_FILE_DIR:${target}>/${_old_file}"
            VERBATIM
        )
    endfunction()

    add_build_compat_alias(dxcli dxrt-cli)
    add_build_compat_alias(dxcli-internal dxrt-cli-internal)
    add_build_compat_alias(dxparse parse_model)
    add_build_compat_alias(dxrun run_model)

    set(_COMPAT_ALIASES
        dxcli          dxrt-cli
        dxcli-internal dxrt-cli-internal
        dxparse        parse_model
        dxrun          run_model
    )

    install(CODE "
        set(_exe_suffix \"${CMAKE_EXECUTABLE_SUFFIX}\")
        set(_install_prefix_bin \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin\")
        set(_bin_dirs
            \"\${_install_prefix_bin}\"
            \"${CMAKE_SOURCE_DIR}/bin\"
        )
        set(_pairs \"${_COMPAT_ALIASES}\")
        list(LENGTH _pairs _len)
        foreach(_bin_dir IN LISTS _bin_dirs)
            set(_i 0)
            while(_i LESS _len)
                list(GET _pairs \${_i} _new)
                math(EXPR _i_next \"\${_i} + 1\")
                list(GET _pairs \${_i_next} _old)
                set(_new_file \"\${_new}\${_exe_suffix}\")
                set(_old_file \"\${_old}\${_exe_suffix}\")
                set(_new_path \"\${_bin_dir}/\${_new_file}\")
                set(_old_path \"\${_bin_dir}/\${_old_file}\")
                if(EXISTS \"\${_new_path}\")
                    if(EXISTS \"\${_old_path}\" OR IS_SYMLINK \"\${_old_path}\")
                        file(REMOVE \"\${_old_path}\")
                    endif()
                    # This non-Windows branch preserves compatibility aliases as symbolic links.
                    file(CREATE_LINK \"\${_new_file}\" \"\${_old_path}\" SYMBOLIC)
                    if(_bin_dir STREQUAL \"\${_install_prefix_bin}\")
                        list(APPEND CMAKE_INSTALL_MANIFEST_FILES \"\${_old_path}\")
                    endif()
                    message(STATUS \"Created install alias in \${_bin_dir}: \${_old_file} -> \${_new_file}\")
                else()
                    message(WARNING \"Alias skipped in \${_bin_dir}: \${_new_path} not found\")
                endif()
                math(EXPR _i \"\${_i} + 2\")
            endwhile()
        endforeach()
    ")
endif()

add_subdirectory(dxtop)
add_subdirectory(dxbenchmark)
