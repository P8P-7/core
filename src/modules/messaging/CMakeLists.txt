project(goliath-messaging-module LANGUAGES CXX)

set(SOURCES
    zmq_io.cpp
    zmq_publisher.cpp
    zmq_subscriber.cpp
    )

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
    PRIVATE
        ${ZeroMQ_INCLUDE_DIR}
        ${cppzmq_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        goliath-proto
        goliath-core-module
    PRIVATE
        cppzmq
        ${cppzmq_LIBRARY}
        ${Protobuf_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES}
    )
