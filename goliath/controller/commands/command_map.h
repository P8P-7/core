#pragma once

#include <atomic>
#include <map>

#include "command.h"

namespace goliath::commands {

    enum class command_status {
        RUNNING = true,
        STALE = false
    };

    struct command_item {
        command_item()
            : command(nullptr), status(command_status::STALE) {}
        command_item(Command *command, command_status status)
            : command(command), status(status) {}

        Command *command;
        command_status status;
    };

    class command_map {
    public:
        command_map(const std::map<const unsigned, command_item> commands)
            : map(commands) {}

        void add_command(const unsigned command_id, Command *command);
        bool get_command_status(const unsigned command_id);
        Command* get_instance(const unsigned command_id);

    private:
        std::map<const unsigned, command_item> map;
    };
}
