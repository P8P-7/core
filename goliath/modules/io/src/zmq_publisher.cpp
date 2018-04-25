#include <goliath/zmq_publisher.h>

using namespace goliath::io;

zmq_publisher::zmq_publisher(zmq::context_t &context, const std::string &host, const int port)
        : zmq_io(context, host, port, ZMQ_PUB) {
    connect(address());
}

bool zmq_publisher::publish(const idp::Message &message) {
    try {
        zmq::message_t address(message.channel().size());
        memcpy(address.data(), message.channel().data(), message.channel().size());

        socket.send(address, ZMQ_SNDMORE);

        std::string str;
        message.data().SerializeToString(&str);

        unsigned long sz = str.length();
        zmq::message_t data(sz);
        memcpy(data.data(), str.c_str(), sz);

        socket.send(data);
        return true;
    }
    catch (const zmq::error_t &ex) {
        return false;
    }
}
