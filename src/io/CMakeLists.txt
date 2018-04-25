include_directories(include ${ZeroMQ_INCLUDE_DIR} ${cppzmq_INCLUDE_DIR})

set(SOURCES
        zmq_io.h
        zmq_io.cpp
        zmq_publisher.h
        zmq_publisher.cpp
        zmq_subscriber.h
        zmq_subscriber.cpp)

add_library(goliath-io ${SOURCES})

target_include_directories(goliath-io
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(goliath-io
    PUBLIC
        goliath-proto
    PRIVATE
        cppzmq
        ${cppzmq_LIBRARY}
        ${Protobuf_LIBRARY}
)