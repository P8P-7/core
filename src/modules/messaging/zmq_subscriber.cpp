#include <unistd.h>
#include <functional>

#include "zmq_subscriber.h"

using namespace goliath::messaging;

zmq_subscriber::zmq_subscriber(zmq::context_t &context, const std::string &host, const int port,
                               const std::string &topic, std::function<void(size_t, const Message&)> callback)
        : zmq_io(context, host, port, ZMQ_SUB), topic(topic), callback(callback) {
    connect(address());
    socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    // To use zmq_poll correctly, we construct this vector of poll items
    poll = {
            {nullptr, 0, ZMQ_POLLIN, 0}, // Not polling a ZMQ socket, poll on stdin. 0 is the fd for stdin.
            {socket,  0, ZMQ_POLLIN, 0} // Polling on a ZMQ socket
    };
}

void zmq_subscriber::receive() {
    zmq::message_t address;
    zmq::message_t data;

    // When timeout (the third argument here) is -1,
    // then block until ready to receive
    zmq::poll(poll.data(), poll.size(), -1);

    if (poll[0].revents == ZMQ_POLLIN) {
        // A key has been pressed, read it
        char c;
        read(0, &c, 1);

        if (c == 'c') {
            std::cout << "Interrupt received, will exit…" << std::endl;
            return;
        }
    }
    if (poll[1].revents == ZMQ_POLLIN) {
        //  Read envelope with address
        socket.recv(&address);

        std::string topic = std::string(static_cast<char *>(address.data()), address.size());

        //  Read message contents
        socket.recv(&data);

        Message message;
        message.ParseFromArray(data.data(), static_cast<int>(data.size()));

        if(message.data_case() != Message::DATA_NOT_SET) {
            callback(message.data_case(), message);
        }
    }
    receive();
}
