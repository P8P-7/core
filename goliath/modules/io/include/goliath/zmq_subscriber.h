#pragma once

#include "zmq_io.h"

namespace goliath::io {
    class zmq_subscriber : public zmq_io {
    public:
        zmq_subscriber(const std::string &host, const int port, const std::string &topic);

        std::string get();

    private:
        const std::string topic;
    };
}