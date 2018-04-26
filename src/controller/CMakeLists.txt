set(SOURCES
        commands/command.h
        commands/move_command.h
        commands/move_command.cpp
        controller.cpp)

add_executable(goliath-controller ${SOURCES})
target_link_libraries(goliath-controller
   PUBLIC
        goliath-io
)