#include <goliath/io.h>
#include <iostream>

using namespace goliath::io;

void zmq_io::connect(const char* address) {
    socket.connect(address);
}

void zmq_io::bind(const char* address) {
    socket.bind(address);
}