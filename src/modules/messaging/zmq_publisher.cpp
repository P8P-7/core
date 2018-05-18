#include "zmq_publisher.h"

using namespace goliath::messaging;

ZmqPublisher::ZmqPublisher(zmq::context_t &context, const std::string &host, const int port)
        : ZmqIo(context, host, port, ZMQ_PUB) {
    connect();
}

bool ZmqPublisher::publish(const MessageCarrier &carrier) {
    try {
        std::string channel = std::to_string(carrier.message_case());

        zmq::message_t address(channel.size());
        std::memcpy(address.data(), channel.data(), channel.size());

        socket.send(address, ZMQ_SNDMORE);

        std::string str;
        carrier.SerializeToString(&str);

        unsigned long sz = str.length();
        zmq::message_t data(sz);
        std::memcpy(data.data(), str.c_str(), sz);

        socket.send(data);
    } catch (const zmq::error_t &ex) {
        return false;
    }

    return true;
}
