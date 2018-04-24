#include <goliath/io.h>

using namespace goliath::io;

zmq_subscriber::zmq_subscriber(const char* address, const std::string& topic)
    : zmq_io(address, ZMQ_SUB), topic(topic) {
    connect(address);
    socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());
}

char* zmq_subscriber::get() {
    zmq::message_t message;
    socket.recv(&message);
    return static_cast<char*>(message.data());
}