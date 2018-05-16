#pragma once

#include <goliath/core.h>
#include "zmq_io.h"

namespace goliath::messaging {
    class zmq_publisher : public zmq_io, public core::interfaces::publisher_service {
    public:
        zmq_publisher(zmq::context_t& context, const std::string& host, int port);

        bool publish(const MessageCarrier& carrier) override;
    };
}
