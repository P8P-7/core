project(io-module CXX)

set(SOURCES
        include/goliath/zmq_io.h
        zmq_io.cpp
        include/goliath/zmq_publisher.h
        zmq_publisher.cpp
        include/goliath/zmq_subscriber.h
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