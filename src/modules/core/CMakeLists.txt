project(goliath-core-module LANGUAGES CXX)

find_package(Boost
        COMPONENTS
            log
        REQUIRED
        )

set(SOURCES
        handle.cpp
        repositories/proto_repository.h
        repositories/repository.h
        repositories/watcher.cpp
        repositories/battery_repository.h
        repositories/battery_repository.cpp
        publisher_service.h
        )

include_directories(${Boost_INCLUDE_DIR})

add_library(${PROJECT_NAME} ${SOURCES})

add_definitions(-DBOOST_LOG_DYN_LINK)

target_link_libraries(${PROJECT_NAME}
        goliath-proto
        ${Protobuf_LIBRARY}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
