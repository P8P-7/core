#pragma once

#include "zmq_io.h"

namespace goliath::io {
    class zmq_publisher : public zmq_io {
    public:
        zmq_publisher(const std::string &host, const int port, const std::string& topic);

        bool publish(const std::string& message);

    private:
        const std::string topic;
    };
}