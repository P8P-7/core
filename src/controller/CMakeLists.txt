project(goliath-controller CXX)

set(SOURCES
        command_map.cpp
        command_executor.cpp
        commands/command.cpp
        commands/move_command.cpp
        commands/follow_line_command.cpp
        commands/move_tower_command.cpp
        handles.h
        handle_map.cpp
        controller.cpp
        )

compiler_flags(${PROJECT_NAME} 4)

find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
            log
        REQUIRED
        )
add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(${PROJECT_NAME} ${SOURCES})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
        PUBLIC
            core-module
            io-module
            vision-module
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
        )