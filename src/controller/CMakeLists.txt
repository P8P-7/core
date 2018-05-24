project(goliath-controller LANGUAGES CXX)

set(SOURCES
        commands/command.cpp
        commands/move_command.cpp
        commands/follow_line_command.cpp
        commands/move_tower_command.cpp
        commands/move_wing_command.cpp
        command_map.cpp
        command_executor.cpp
        handles.h
        handle_map.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})
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

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

configure_file(resources/core-text.txt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/core-text.txt COPYONLY)
