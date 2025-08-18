# DXRT Examples (multi_input_model_inference)
set(target multi_input_model_inference)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")
 
add_executable(${target} ${srcs})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  
add_dxrt(${target})

install( TARGETS ${target} DESTINATION ${CMAKE_SOURCE_DIR}/bin/examples )