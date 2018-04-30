#pragma once

#include <atomic>
#include <map>
#include <memory>

#include "command.h"

namespace goliath::commands {

    enum class command_status {
        RUNNING = true,
        STALE = false
    };

    struct command_item {
        command_item()
            : status(command_status::STALE) {}
        command_item(std::shared_ptr<Command> command, command_status status)
            : command(command), status(status) {}

        std::weak_ptr<Command> command;
        command_status status;
    };

    class command_map {
    public:
        command_map(const std::map<const unsigned, command_item> commands)
            : map(commands) {}

        void add(const unsigned command_id, std::shared_ptr<Command> command);

        command_status get_command_status(const unsigned command_id) const;
        void set_command_status(const unsigned command_id, const command_status status) const;

        std::weak_ptr<Command> get_instance(const unsigned command_id);

    private:
        mutable std::map<const unsigned, command_item> map;
    };
}
