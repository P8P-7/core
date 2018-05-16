#include <unistd.h>
#include <functional>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

#include "zmq_subscriber.h"

using namespace goliath::messaging;

zmq_subscriber::zmq_subscriber(zmq::context_t &context, const std::string &host, const int port)
        : zmq_io(context, host, port, ZMQ_SUB) {
    connect();
    BOOST_LOG_TRIVIAL(info) << "Subscriber is listening to " << address();

    interrupt_socket = new zmq::socket_t(context, ZMQ_PAIR);
    interrupt_socket->bind(interrupter_address);

    // To use zmq_poll correctly, we construct this vector of poll items
    poll = {
            { *interrupt_socket, 0, ZMQ_POLLIN, 0 },
            { socket,  0, ZMQ_POLLIN, 0 } // Polling on a ZMQ socket
    };
}
zmq_subscriber::~zmq_subscriber() {
    if (thread.joinable()) {
        stop();
    }

    delete interrupt_socket;
}

void zmq_subscriber::bind(const MessageCarrier::MessageCase &message_type, std::function<void(const MessageCarrier &)> callback) {
    if (callbacks.find(message_type) == callbacks.end()) {
        std::string topic = std::to_string(message_type);
        socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    }

    callbacks[message_type] = callback;
}

void zmq_subscriber::start() {
    if (running) {
        throw std::runtime_error("Subscriber is already running");
    }

    running = true;
    thread = std::thread(&zmq_subscriber::run, this);
}

void zmq_subscriber::stop() {
    if (!running) {
        throw std::runtime_error("Subscriber needs to be started before it can be stopped");
    }

    zmq::socket_t interrupter(context, ZMQ_PAIR);
    interrupter.connect(interrupter_address);
    interrupter.send("42", 2);

    thread.join();
    running = false;
}

void zmq_subscriber::run() {
    while (true) {
        zmq::message_t address;
        zmq::message_t data;

        // When timeout (the third argument here) is -1,
        // then block until ready to receive
        zmq::poll(poll.data(), poll.size(), -1);

        if (poll[0].revents == ZMQ_POLLIN) {
            BOOST_LOG_TRIVIAL(info) << "Subscriber has been interrupted. Closing connection...";
            return;
        }

        if (poll[1].revents == ZMQ_POLLIN) {
            //  Read envelope with address
            socket.recv(&address);

            //  Read message contents
            socket.recv(&data);

            MessageCarrier carrier;
            carrier.ParseFromArray(data.data(), static_cast<int>(data.size()));

            if (carrier.message_case() != MessageCarrier::MESSAGE_NOT_SET) {
                callbacks[carrier.message_case()](carrier);
            }
        }
    }
}

