project(goliath-io-module LANGUAGES CXX)

set(SOURCES
        zmq_io.cpp
        zmq_publisher.cpp
        zmq_subscriber.cpp
        )

add_library(${PROJECT_NAME} ${SOURCES})

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
        PRIVATE
            cppzmq
            ${cppzmq_LIBRARY}
            ${Protobuf_LIBRARY}
        )
