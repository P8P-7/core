#pragma once

#include <thread>

#include "zmq_io.h"

namespace goliath::io {
    class zmq_subscriber : public zmq_io {
    public:
        zmq_subscriber(const std::string &host, const int port, const std::string &topic);

        void receive();

    private:
        const std::string topic;

        std::thread worker;
    };
}