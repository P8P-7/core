project(goliath-controller LANGUAGES CXX)

set(SOURCES
        command_map.cpp
        command_executor.cpp
        commands/command.cpp
        commands/move_command.cpp
        commands/follow_line_command.cpp
        commands/move_tower_command.cpp
        commands/move_wing_command.cpp
        handles.h
        handle_map.cpp
        controller.cpp
        )

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
            goliath-zmq-messaging-module
            goliath-gpio-module
            goliath-servo-module
            goliath-vision-module
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )

configure_file(resources/core-text.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core-text.txt COPYONLY)
