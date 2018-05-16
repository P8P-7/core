#include "zmq_io.h"

using namespace goliath::messaging;

std::string zmq_io::address() const {
    std::string address = PROTOCOL + "://" + host + ':' + std::to_string(port);
    return address;
}

void zmq_io::connect() {
    socket.connect(address());
}

void zmq_io::bind() {
    socket.bind(address());
}
