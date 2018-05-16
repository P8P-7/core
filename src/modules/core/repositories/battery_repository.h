#pragma once

#include <google/protobuf/message.h>
#include "repository.h"

namespace goliath::repositories {
    class battery_repository : public repository {
    public:
        int get_battery_level() const;

        std::unique_ptr<::google::protobuf::Message> get_message() override;
    };
}

