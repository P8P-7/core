project(goliath-controller LANGUAGES CXX)

find_package(Boost ${BOOST_MIN_VERSION}
    COMPONENTS
        log
    REQUIRED
    )

set(SOURCES
        command_map.cpp
        command_executor.cpp
        commands/command.cpp
        commands/move_command.cpp
        commands/follow_line_command.cpp
        commands/move_tower_command.cpp
        util/colored_console.cpp
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
            goliath-vision-module
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )
