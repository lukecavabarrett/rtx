project(test)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -Wextra")

add_executable(test test.cc)
target_link_libraries(test boost_system png jpeg rtx)

target_include_directories(test
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        )