#pragma once

#include "command.h"

namespace goliath::commands {
    class follow_line_command: public command {
    public:
        follow_line_command();

        void execute(const handles::handle_map &handles, const Message &message) override;
    };
}

