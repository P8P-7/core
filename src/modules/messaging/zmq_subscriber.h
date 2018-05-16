#pragma once

#include <thread>
#include <atomic>

#include "zmq_io.h"

namespace goliath::messaging {
    class zmq_subscriber : public zmq_io {
    public:
        using zmq_message_topic = MessageCarrier::MessageCase;
        using zmq_message_callback = std::function<void(const MessageCarrier&)>;

        zmq_subscriber(zmq::context_t &context, const std::string &host, const int port);
        ~zmq_subscriber();

        void bind(const zmq_message_topic &topic, zmq_message_callback callback);
        void start();
        void stop();
    private:
        const std::string interrupter_address = "inproc://interrupt";
        zmq::socket_t *interrupt_socket;

        std::vector<zmq::pollitem_t> poll;
        std::map<zmq_message_topic, zmq_message_callback> callbacks;

        bool running = false;
        std::thread thread;

        void run();
    };
}
