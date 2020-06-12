project(rtx)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -std=c++2a)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -Wall -Wextra")
add_library(${PROJECT_NAME} STATIC vector2.cc vector3.cc color.cc screen.cc)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        X11
        pthread
        )

add_library(libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )


target_compile_features(${PROJECT_NAME}
        PRIVATE
        cxx_std_17
        )