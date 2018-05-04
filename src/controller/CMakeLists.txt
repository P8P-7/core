project(goliath-controller CXX)

set(SOURCES
        commands/command.cpp
        commands/command_map.cpp
        handles/handles.h
        handles/handle_mutex.cpp
        handles/handle_map.cpp
        controller.cpp
        command_runner.cpp
)

compiler_flags(${PROJECT_NAME} 4)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        io-module
        vision-module
        ${CMAKE_THREAD_LIBS_INIT}
)