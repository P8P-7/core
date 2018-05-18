#include "zmq_subscriber.h"

#include <unistd.h>
#include <functional>
#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

using namespace goliath::messaging;

ZmqSubscriber::ZmqSubscriber(zmq::context_t &context, const std::string &host, const int port)
        : ZmqIo(context, host, port, ZMQ_SUB) {
    connect();
    BOOST_LOG_TRIVIAL(info) << "Subscriber is listening to " << address();

    interruptSocket = new zmq::socket_t(context, ZMQ_PAIR);
    interruptSocket->bind(interrupterAddress);

    // To use zmq_poll correctly, we construct this vector of poll items
    poll = {
            { *interruptSocket, 0, ZMQ_POLLIN, 0 },
            { socket,  0, ZMQ_POLLIN, 0 } // Polling on a ZMQ socket
    };
}
ZmqSubscriber::~ZmqSubscriber() {
    if (thread.joinable()) {
        stop();
    }

    delete interruptSocket;
}

void ZmqSubscriber::bind(const MessageCarrier::MessageCase &messageType, std::function<void(const MessageCarrier&)> callback) {
    if (callbacks.find(messageType) == callbacks.end()) {
        std::string topic = std::to_string(messageType);
        socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    }

    callbacks[messageType] = callback;
}

void ZmqSubscriber::start() {
    if (running) {
        throw std::runtime_error("Subscriber is already running");
    }

    running = true;
    thread = std::thread(&ZmqSubscriber::run, this);
}

void ZmqSubscriber::stop() {
    if (!running) {
        throw std::runtime_error("Subscriber needs to be started before it can be stopped");
    }

    zmq::socket_t interrupter(context, ZMQ_PAIR);
    interrupter.connect(interrupterAddress);
    interrupter.send("42", 2);

    thread.join();
    running = false;
}

void ZmqSubscriber::run() {
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

