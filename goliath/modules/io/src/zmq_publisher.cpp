#include <goliath/zmq_publisher.h>

using namespace goliath::io;

zmq_publisher::zmq_publisher(const std::string &host, const int port, const std::string &topic)
    : zmq_io(host, port, ZMQ_PUB), topic(topic) {
    bind(address());
}

bool zmq_publisher::publish(const std::string &message) {
    try{
        zmq::message_t mess(message.size() + topic.size());
        snprintf(static_cast<char*>(mess.data()), mess.size() + topic.size(), (message + topic).c_str());

        socket.send(mess);
        return true;
    }
    catch(zmq::error_t &e) {
        return false;
    }
}