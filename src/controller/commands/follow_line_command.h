#pragma once

#include "command.h"

namespace goliath::commands {
    class follow_line_command: public command {
    public:
        follow_line_command();

    private:
        void execute(const handles::handle_map &handles, const CommandMessage &message) override;
    };
}

