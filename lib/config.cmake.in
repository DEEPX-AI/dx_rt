find_library(DXRT_LIBRARY
    NAMES dxrt
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
)

if(DXRT_LIBRARY)
    set(DXRT_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(dxrt DEFAULT_MSG DXRT_LIBRARY)

mark_as_advanced(DXRT_LIBRARY)
