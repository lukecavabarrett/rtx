project(rtx)
set(CMAKE_CXX_STANDARD 17)

file(GLOB glob_SRC
        "*.h"
        "*.cpp"
        )
add_library(${PROJECT_NAME} STATIC ${glob_SRC})

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
        cxx_std_11
        )