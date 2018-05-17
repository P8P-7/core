#pragma once

#include <MessageCarrier.pb.h>

namespace goliath::core::interfaces {
    class PublisherService {
    public:
        /**
         * @param carrier message to send
         * @return true if the carrier has successfully been sent to the publisher
         */
        virtual bool publish(const MessageCarrier& carrier) = 0;
    };
}
