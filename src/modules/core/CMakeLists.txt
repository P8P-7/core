project(goliath-core-module LANGUAGES CXX)

set(SOURCES
        include/goliath/handle.h
        handle.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )