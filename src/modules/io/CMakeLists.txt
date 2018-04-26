set(SOURCES
        include/zmq_io.h
        zmq_io.cpp
        include/zmq_publisher.h
        zmq_publisher.cpp
        include/zmq_subscriber.h
        zmq_subscriber.cpp)

add_library(goliath-io ${SOURCES})

target_include_directories(goliath-io
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${ZeroMQ_INCLUDE_DIR}
        ${cppzmq_INCLUDE_DIR}
)

target_link_libraries(goliath-io
    PUBLIC
        goliath-proto
    PRIVATE
        cppzmq
        ${cppzmq_LIBRARY}
        ${Protobuf_LIBRARY}
)