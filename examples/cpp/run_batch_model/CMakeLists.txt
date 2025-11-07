# DXRT Examples (run batch model)
set(target run_batch_model)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
 
add_executable(${target} ${srcs})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  
add_dxrt(${target})

install( TARGETS ${target} DESTINATION bin/examples )
install( TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin/examples )