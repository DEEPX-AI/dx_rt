# DXTOP
set(target dxtop)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
 
add_executable(${target} ${srcs})

 # Find ncurses package
if (NOT CROSS_COMPILE)
  find_package(Curses REQUIRED)
else()
  # only available host arch compile
  set(CURSES_LIBRARIES ${CMAKE_SOURCE_DIR}/extern/ncurses/lib_aarch64/libncursesw.a)
  set(CURSES_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/ncurses/include)
endif()

target_include_directories(${target} PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/extern/include 
    ${CURSES_INCLUDE_DIR}
  )

target_link_libraries(${target} dxrt pthread ${link_libs} ${CURSES_LIBRARIES})
  
install( TARGETS ${target} DESTINATION bin )
install( TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin )

