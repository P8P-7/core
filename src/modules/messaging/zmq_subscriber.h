#pragma once

#include <thread>
#include <MessageCarrier.pb.h>

#include "zmq_io.h"

namespace goliath::messaging {
    class ZmqSubscriber : public ZmqIo {
    public:
        using zmqMessageTopic = MessageCarrier::MessageCase;
        using zmqMessageCallback = std::function<void(const MessageCarrier&)>;

        ZmqSubscriber(zmq::context_t &context, const std::string &host, const int port);
        ~ZmqSubscriber();

        void bind(const zmqMessageTopic &topic, zmqMessageCallback callback);
        void start();
        void stop();
    private:
        const std::string interrupterAddress = "inproc://interrupt";
        zmq::socket_t *interruptSocket;

        std::vector<zmq::pollitem_t> poll;
        std::map<zmqMessageTopic, zmqMessageCallback> callbacks;

        bool running = false;
        std::thread thread;

        void run();
    };
}
