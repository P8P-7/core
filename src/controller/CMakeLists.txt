project(goliath-controller CXX)

set(SOURCES
        commands/command.cpp
        commands/command_map.cpp
        handles/handles.h
        handles/handle_mutex.cpp
        handles/handle_map.cpp
        controller.cpp
        command_runner.cpp
        commands/move_command.cpp commands/move_command.h)

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
        io-module
        vision-module
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
)