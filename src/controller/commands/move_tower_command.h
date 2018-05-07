#pragma once

#include "command.h"

namespace goliath::commands {
    class move_tower_command: public command {
    public:
        move_tower_command();

    private:
        void execute(const handles::handle_map& handles, const Message &message) override ;

    };
}

