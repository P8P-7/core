#include <goliath/io.h>

using namespace goliath::io;

zmq_publisher::zmq_publisher(const char* address, const std::string &topic)
    : zmq_io(address, ZMQ_PUB), topic("ipc://" + topic + ".ipc") {
    bind(address);
    bind(topic.c_str());
}

bool zmq_publisher::publish(const std::string &message) {
    try{
        zmq::message_t mess(message.size());
        snprintf(static_cast<char*>(mess.data()), mess.size(), message.c_str());

        socket.send(mess);
        return true;
    }
    catch(zmq::error_t &e) {
        return false;
    }
}