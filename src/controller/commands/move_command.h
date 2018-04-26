#pragma once

#include <memory>

#include "command.h"

namespace goliath::commands {
    class move_command : public command {
    public:
        explicit move_command();

        void execute() override;
    private:
    };
}