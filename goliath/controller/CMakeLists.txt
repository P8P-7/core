project(controller CXX)

include(flags)

set(COMMAND_SOURCE
    commands/command.h
    commands/move_command.cpp
    handles/handles.h
    handles/handle_mutex.cpp
    handles/handle_map.cpp
    )

set(CONTROLLER_SOURCE
    main.cpp
    ${COMMAND_SOURCE}
    )

set(CONTROLLER_MODULES
    core-module
    io-module
    caterpillar-movement-module
    )

add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCE})

compiler_flags(${PROJECT_NAME} 4)

target_link_libraries(${PROJECT_NAME} ${CONTROLLER_MODULES})
link_protobuf(${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/goliath/proto)