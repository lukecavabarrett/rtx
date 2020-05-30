project(sample)
cmake_minimum_required(VERSION 3.16)
add_subdirectory(src)

add_executable(sample sample.cpp)
target_link_libraries(sample rtx)