project(controller CXX)

include(flags)

set(COMMAND_SOURCE
    commands/command.h
    commands/move_command.h
    commands/move_command.cpp
    )

set(CONTROLLER_SOURCE
    main.cpp
    )

add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCE} ${COMMAND_SOURCE})

compiler_flags(${PROJECT_NAME} 4)

target_link_libraries(${PROJECT_NAME} caterpillar-movement-module)
link_opencv(${PROJECT_NAME})
