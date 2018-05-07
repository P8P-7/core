#pragma once

#include <atomic>
#include <map>
#include <memory>

#include <Message.pb.h>

#include "commands/command.h"

namespace goliath::commands {
    enum class command_status {
        RUNNING,
        STALE
    };

    struct command_item {
        command_item();
        command_item(std::shared_ptr<command> command, command_status status);

        std::shared_ptr<command> instance;
        command_status status;
    };

    // TODO: make thread-safe?
    class command_map {
    public:
        command_map();
        explicit command_map(std::map<size_t, command_item> commands);

        void add(size_t command_id, std::shared_ptr<command> command);

        command_item& operator[](size_t id);
        const command_item& operator[](size_t id) const;
        bool command_exists(size_t id) const;
    private:
        std::map<size_t, command_item> map;
    };
}
