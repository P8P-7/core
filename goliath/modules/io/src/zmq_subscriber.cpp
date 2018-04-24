#include <goliath/zmq_subscriber.h>

using namespace goliath::io;

zmq_subscriber::zmq_subscriber(const std::string &host, const int port, const std::string &topic)
    : zmq_io(host, port, ZMQ_SUB), topic(topic) {
    connect(address());
    socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    worker = std::thread(&zmq_subscriber::receive, this);
}

void zmq_subscriber::receive() {
    while(true) {
        zmq::message_t message;
        socket.recv(&message);

        std::string data = std::string(static_cast<char*>(message.data()));
        std::cout << data << std::endl;
    }
}