# DXRT Examples (run async model display)
set(target dxtop)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
 
add_executable(${target} ${srcs})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  

 # Find ncurses package
 find_package(Curses REQUIRED)

 target_include_directories(${target} PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/extern/include 
    ${CURSES_INCLUDE_DIR}
  )

target_link_libraries(${target} dxrt pthread ${link_libs} ${CURSES_LIBRARIES})
  
install( TARGETS ${target} DESTINATION bin )
install( TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin )

