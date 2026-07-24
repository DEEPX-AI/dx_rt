
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

function(add_build_compat_alias target old_name)
    if(NOT TARGET ${target})
        return()
    endif()
    set(_old_file "${old_name}${CMAKE_EXECUTABLE_SUFFIX}")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove -f "$<TARGET_FILE_DIR:${target}>/${_old_file}"
        # CI prebuilt checker ships build_*/bin through artifact upload/download.
        # Use real file copies here (not symlinks) to avoid metadata-loss across
        # artifact boundaries and guarantee old-name executables exist.
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${target}>" "$<TARGET_FILE_DIR:${target}>/${_old_file}"
        VERBATIM
    )
endfunction()

# Create backward-compat aliases in build output (build_*/bin) so CI jobs that
# copy prebuilt binaries directly from build output can still run old names.
add_build_compat_alias(dxcli dxrt-cli)
add_build_compat_alias(dxcli-internal dxrt-cli-internal)
add_build_compat_alias(dxparse parse_model)
add_build_compat_alias(dxrun run_model)

# Backward-compatibility aliases for renamed binaries.
# Linux/macOS: symbolic links. Windows: hard links (COPY_ON_ERROR for non-NTFS).
set(_COMPAT_ALIASES
    dxcli          dxrt-cli
    dxcli-internal dxrt-cli-internal
    dxparse        parse_model
    dxrun          run_model
)

# CMAKE_EXECUTABLE_SUFFIX (".exe" on Windows, "" elsewhere) is only known at
# configure time, so bake it into the install script string here.
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
                if(CMAKE_HOST_WIN32)
                    file(CREATE_LINK \"\${_new_path}\" \"\${_old_path}\" COPY_ON_ERROR)
                else()
                    file(CREATE_LINK \"\${_new_file}\" \"\${_old_path}\" SYMBOLIC)
                endif()
                # Register the install-prefix alias in CMake's install manifest
                # so `ninja uninstall` / cmake_uninstall.cmake removes it. Aliases
                # under \${CMAKE_SOURCE_DIR}/bin are build-output convenience copies
                # and intentionally left out of the manifest.
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

add_subdirectory(dxtop)
add_subdirectory(dxbenchmark)
