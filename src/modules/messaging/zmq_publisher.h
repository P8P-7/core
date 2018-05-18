#pragma once

#include <goliath/core.h>

#include "zmq_io.h"

/**
 * @file zmq_publisher.h
 * @author Group 7 - Informatica
 */

namespace goliath::messaging {
    /**
     * @class goliath::messaging::ZmqPublisher
     * @brief Publishes messages
     */
    class ZmqPublisher : public ZmqIo, public core::interfaces::PublisherService {
    public:
        /**
         * @param context @see goliath::messaging::ZmqIo
         * @param host @see goliath::messaging::ZmqIo
         * @param port @see goliath::messaging::ZmqIo
         */
        ZmqPublisher(zmq::context_t& context, const std::string& host, int port);

        /**
         * @brief Publishes message
         * @param carrier Message
         * @return Status
         */
        bool publish(const MessageCarrier& carrier) override;
    };
}
