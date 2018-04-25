set(SOURCES
        command.h
        move_command.h
        move_command.cpp)

add_library(goliath-commands ${SOURCES})

target_include_directories(goliath-commands
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
)