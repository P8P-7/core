#pragma once

#include <thread>
#include <atomic>

#include "zmq_io.h"
#include <goliath/message.pb.h>

namespace goliath::io {
    class zmq_subscriber : public zmq_io {
    public:
        zmq_subscriber(zmq::context_t &context, const std::string &host, const int port, const std::string &topic);

        void receive();

        void stop();

    private:
        const std::string topic;

        std::vector<zmq::pollitem_t> p;

        std::thread subs_thread;

        std::atomic<bool> stop_flag{false};
    };
}