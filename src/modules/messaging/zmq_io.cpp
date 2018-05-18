#include "zmq_io.h"

#include <iostream>

using namespace goliath::messaging;

std::string ZmqIo::address() const {
    std::string address = PROTOCOL + "://" + host + ':' + std::to_string(port);
    return address;
}

void ZmqIo::connect() {
    socket.connect(address());
}

void ZmqIo::bind() {
    socket.bind(address());
}
