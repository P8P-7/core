#pragma once

#include <google/protobuf/message.h>

#include "repository.h"

namespace goliath::repositories {
    class BatteryRepository : public Repository {
    public:
        int getBatteryLevel() const;

        std::unique_ptr<::google::protobuf::Message> getMessage() override;
    };
}

