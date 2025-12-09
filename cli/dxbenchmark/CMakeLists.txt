# DXRT benchmark 
set(target dxbenchmark)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
 
add_executable(${target} ${srcs})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  

 target_include_directories(${target} PUBLIC 
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_SOURCE_DIR}/extern/include 
    ${CURSES_INCLUDE_DIR}
  )

if (MSVC)
    target_link_libraries(${target} PRIVATE user32 advapi32 dxrt ${link_libs})
    target_link_options(${target} PRIVATE "/SUBSYSTEM:CONSOLE")
else()
    target_link_libraries(${target} dxrt ${link_libs})
endif()
  
install( TARGETS ${target} DESTINATION bin )
install( TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin )

