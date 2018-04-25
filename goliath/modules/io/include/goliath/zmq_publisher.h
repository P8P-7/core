#pragma once

#include "zmq_io.h"
#include <goliath/message.pb.h>

namespace goliath::io {
    class zmq_publisher : public zmq_io {
    public:
        zmq_publisher(zmq::context_t &context, const std::string &host, const int port);

        bool publish(const idp::Message &message);

    private:
    };
}