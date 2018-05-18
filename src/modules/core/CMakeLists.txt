project(goliath-core-module LANGUAGES CXX)

set(SOURCES
        handle.cpp
        repositories/proto_repository.h
        repositories/repository.h
        repositories/watcher.cpp
        repositories/battery_repository.h
        repositories/battery_repository.cpp
        handle_error.cpp handle_error.h)

add_library(${PROJECT_NAME} ${SOURCES})

find_package(Boost ${BOOST_MIN_VERSION}
        COMPONENTS
            log
        REQUIRED
        )
add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            goliath-proto
            ${Protobuf_LIBRARY}
            ${CMAKE_THREAD_LIBS_INIT}
            ${Boost_LIBRARIES}
        )