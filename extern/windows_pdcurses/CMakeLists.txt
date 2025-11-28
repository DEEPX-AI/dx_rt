

cmake_minimum_required(VERSION 3.10) # Using 3.10 for modern CMake features like target_link_options
project(pdcurses C CXX)

# Define source directories based on your Makefile structure
# PDCURSES_SRCDIR is effectively '..' from the win32 directory
set(PDCURSES_SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OSDIR ${PDCURSES_SRCDIR}/wincon)
set(COMMON_DIR ${PDCURSES_SRCDIR}/common)
set(PDCLIB_DIR ${PDCURSES_SRCDIR}/pdcurses) # Assuming core C files are here

# Define object files based on Makefile.vc's LIBOBJS and PDCOBJS
# You'll need to manually list the .c files from libobjs.mif and pdcscrn.c, pdcutil.c
# from wincon directory, and panel.c (if panel library is used).
# I'll provide a common list, but you might need to adjust it based on common/libobjs.mif's actual content.

# --- Core PDCurses Files (from common/libobjs.mif likely) ---
FILE(GLOB_RECURSE PDCURSES_CORE_SOURCES
    ${PDCLIB_DIR}/*.c
)

# --- Win32 Specific PDCurses Files (from wincon) ---
FILE(GLOB_RECURSE PDCURSES_WIN_SOURCES
    ${OSDIR}/*.c
)
FILE(GLOB_RECURSE PDCURSES_COMMON_SOURCES
    ${COMMON_DIR}/*.c 
)

# Combine all source files
set(PDCURSES_ALL_SOURCES
    ${PDCURSES_CORE_SOURCES}
    ${PDCURSES_WIN_SOURCES}
    ${PDCURSES_COMMON_SOURCES}
)

# RC file for DLL version
set(PDCURSES_RC_FILE ${COMMON_DIR}/pdcurses.rc)

# Conditional for DLL or Static library
option(BUILD_SHARED_LIBS "Build PDCurses as a shared library (DLL)" OFF) # CMake standard option

if(BUILD_SHARED_LIBS)
    add_library(pdcurses SHARED ${PDCURSES_ALL_SOURCES} ${PDCURSES_RC_FILE})
    target_compile_definitions(pdcurses PRIVATE PDC_DLL_BUILD)
    # The Makefile directly links user32.lib advapi32.lib for DLLs
    target_link_libraries(pdcurses PRIVATE user32 advapi32)
else()
    add_library(pdcurses STATIC ${PDCURSES_ALL_SOURCES} ${PDCURSES_RC_FILE})
    # Static libs typically don't need explicit linkage to system libs for PDCurses
endif()

# Define build type flags (based on DEBUG in Makefile.vc)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-Z7 -DPDCDEBUG)
    add_link_options(-debug)
else() # Release or other
    add_compile_options(-O1)
    # LDFLAGS is empty for Release in Makefile.vc
endif()

# Define compile options based on WIDE, UTF8, INFOEX
option(PDCURSES_WIDE "Enable wide character support (PDC_WIDE)" OFF)
if(PDCURSES_WIDE)
    target_compile_definitions(pdcurses PUBLIC PDC_WIDE)
endif()

option(PDCURSES_UTF8 "Force UTF8 mode (PDC_FORCE_UTF8)" OFF)
if(PDCURSES_UTF8)
    target_compile_definitions(pdcurses PUBLIC PDC_FORCE_UTF8)
endif()

option(PDCURSES_NO_INFOEX "Disable INFOEX (HAVE_NO_INFOEX)" OFF) # Makefile uses INFOEX for -DHAVE_NO_INFOEX
if(PDCURSES_NO_INFOEX)
    target_compile_definitions(pdcurses PUBLIC HAVE_NO_INFOEX)
endif()



# Include directories
# -I$(PDCURSES_SRCDIR)
# -I$(demodir) (for demos, but we are only building the library here)
target_include_directories(pdcurses PUBLIC
    ${PDCURSES_SRCDIR} # Root PDCurses directory
    ${COMMON_DIR}      # Common headers if any
    ${OSDIR}           # Wincon specific headers (like pdcwin.h)
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> # For curses.h and pdcurses.h in win32
)

# Install rules (for when you call 'cmake --install .')
install(TARGETS pdcurses DESTINATION lib)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/curses.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pdcurses.h
    DESTINATION include)

# --- Demos (Optional, consider building them as separate executables) ---
# The Makefile also builds demos. For a library's CMakeLists.txt, it's better
# to only build the library. If you want to build demos, you should add them
# as separate executables in this CMakeLists.txt or in a separate one.
# This would involve listing DEMOOBJS, DEMOS, and their sources similar to how
# PDCURSES_ALL_SOURCES was done, and then adding executables.
# For example:
# add_executable(tuidemo ${DEMODIR}/tuidemo.c ${DEMODIR}/tui.c)
# target_include_directories(tuidemo PRIVATE ${DEMODIR})
# target_link_libraries(tuidemo PRIVATE pdcurses user32 advapi32) # Link with the built PDCurses