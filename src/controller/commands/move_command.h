#pragma once

#include "command.h"

namespace goliath::commands {
    class move_command : public command {
    public:
        move_command();

        void execute() override;
    };
}