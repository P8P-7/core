#pragma once

#include <goliath/core.h>

#include "zmq_io.h"

namespace goliath::messaging {
    class ZmqPublisher : public ZmqIo, public core::interfaces::PublisherService {
    public:
        ZmqPublisher(zmq::context_t& context, const std::string& host, int port);

        bool publish(const MessageCarrier& carrier) override;
    };
}
