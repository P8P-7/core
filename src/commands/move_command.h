#pragma once

#include <memory>
#include "goliath/movement_service.h"

#include "command.h"

namespace goliath::commands {
    class move_command : public command {
    public:
        explicit move_command(const std::shared_ptr<core::movement_service> &movement_interface);

        void execute() override;
    private:
        std::shared_ptr<core::movement_service> _movement_interface;
    };
}