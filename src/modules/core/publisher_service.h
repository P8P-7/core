#pragma once

#include <MessageCarrier.pb.h>

namespace goliath::core::interfaces {
    class publisher_service {
    public:
        /**
         * @param carrier message to send
         * @return true if the carrier has successfully been sent to the publisher
         */
        virtual bool publish(const MessageCarrier& carrier) = 0;
    };
}