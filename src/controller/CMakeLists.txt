project(goliath-controller CXX)

set(SOURCES_COMMANDS
        commands/command.cpp
        commands/command_map.cpp
        commands/command_executor.cpp
        commands/move_command.cpp
        )
set(SOURCES_HANDLES
        handles/handles.h
        handles/handle_mutex.cpp
        handles/handle_map.cpp
        )
set(SOURCES
        ${SOURCES_COMMANDS}
        ${SOURCES_HANDLES}
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
            io-module
            vision-module
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
        )