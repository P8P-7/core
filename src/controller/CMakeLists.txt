project(goliath-controller LANGUAGES CXX)

set(SOURCES
        commands/command.cpp
        commands/move_command.cpp
        commands/wunderhorn_command.cpp
        commands/move_tower_command.cpp
        commands/move_wing_command.cpp
        commands/dance_command.cpp
        commands/line_dance_command.cpp
        commands/obstacle_course_command.cpp
        commands/transport_rebuild_command.cpp
        commands/queue_command.cpp
        commands/basic_command.cpp
        command_map.cpp
        command_executor.cpp
        handles.h
        handle_map.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-foundation-module
            goliath-zmq-messaging-module
            goliath-gpio-module
            goliath-servo-module
            goliath-vision-module
            goliath-emotions-module
            goliath-motor-controller-module
            goliath-i2c-module
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
