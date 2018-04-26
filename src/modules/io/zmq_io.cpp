#include "goliath/zmq_io.h"

using namespace goliath::io;

std::string zmq_io::address() const {
    std::string address = PROTOCOL + "://" + host + ':' + std::to_string(port);
    return address;
}

void zmq_io::connect(const std::string &address) {
    socket.connect(address);
}

void zmq_io::bind(const std::string &address) {
    socket.bind(address);
}