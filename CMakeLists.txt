project(ray-tracing)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -Wextra")

add_subdirectory(rtx)
add_subdirectory(test)

add_executable(sample sample.cpp)
target_link_libraries(sample rtx)