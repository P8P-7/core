project(io-module CXX)

set(IO_MODULE_SRC
    src/zmq_io.cpp
    src/zmq_publisher.cpp
    src/zmq_subscriber.cpp
    )

add_library(${PROJECT_NAME} ${IO_MODULE_SRC})

link_cppzmq(${PROJECT_NAME})
multithread(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
