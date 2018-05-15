#pragma once

#include <thread>
#include <atomic>

#include "zmq_io.h"

namespace goliath::messaging {
    class zmq_subscriber : public zmq_io {
    public:
        zmq_subscriber(zmq::context_t &context, const std::string &host,
                       const int port, const std::string &topic, std::function<void(size_t, const Message&)> callback);

        void receive();

    private:
        const std::string topic;
        std::vector<zmq::pollitem_t> poll;
        const std::function<void(size_t, const Message &message)> callback;

    };
}
