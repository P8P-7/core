#pragma once

#include "commands/command.h"

/**
 * @file command_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    /**
     * @enum goliath::commands_command_status
     * @brief Defines the status of a specific command
     */
    enum class CommandStatus {
        STARTING, /**< Command is just getting started */
        STARTED, /**< Command has already started */
        STALE /**< Command is doing nothing (idle) */
    };

    struct CommandItem {
        CommandItem();
        CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status);

        std::shared_ptr<Command> instance;
        CommandStatus status;
    };

    // TODO: make thread-safe?
    class CommandMap {
    public:
        CommandMap();
        explicit CommandMap(std::map<size_t, CommandItem> commands);

        void add(size_t commandId, std::shared_ptr<Command> command);

        CommandItem& operator[](size_t id);
        const CommandItem& operator[](size_t id) const;
        bool commandExists(size_t id) const;
    private:
        std::map<size_t, CommandItem> map;
    };
}
