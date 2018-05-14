#pragma once

#include "zmq_io.h"

namespace goliath::io {
    class zmq_publisher : public zmq_io {
    public:
        zmq_publisher(zmq::context_t& context, const std::string& host, int port);

        bool publish(const Message& message);
    };
}
