# DXRT Examples (run async input_release)
set(target run_async_input_release)
file(GLOB_RECURSE srcs "*.cpp" "*.cc" "*.c")

add_executable(${target} ${srcs})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)  
add_dxrt(${target})
