project(ray-tracing)
cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -Wextra")

find_package(Boost
        1.0.0      # Minimum or EXACT version e.g. 1.67.0
        REQUIRED             # Fail with error if Boost is not found
        COMPONENTS filesystem regex system  # Boost libraries by their canonical name
        # e.g. "date_time" for "libboost_date_time"
        #[OPTIONAL_COMPONENTS <libs>...]
        # Optional Boost libraries by their canonical name)
        )

add_subdirectory(rtx)
add_subdirectory(test)

add_executable(sample sample.cpp)
target_link_libraries(sample rtx)