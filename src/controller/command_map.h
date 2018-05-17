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

    /**
     * @struct goliath::commands::CommandItem
     * @brief Tuple storing both the status of the command and the command instance itself
     */
    struct CommandItem {
        CommandItem();
        /**
         * @param commandInstance Instance of a command
         * @param status Starting status
         */
        CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status);

        std::shared_ptr<Command> instance;
        CommandStatus status;
    };

    // TODO: make thread-safe?
    /**
     * @class goliath::commands::CommandMap
     * @brief Wrapper around a map storing indexes (Command ID's) and @see goliath::commands::CommandItem
     */
    class CommandMap {
    public:
        CommandMap();
        /**
         * @param commands Initial map
         */
        explicit CommandMap(std::map<size_t, CommandItem> commands);

        /**
         * @brief Add a @see goliath::commands::CommandItem to the map
         * @param commandId Preferred ID of the new @see goliath::commmands::Command
         * @param command Pointer to @see goliath::commmands::Command instance
         */
        void add(size_t commandId, std::shared_ptr<Command> command);

        /**
         * @brief Get @see goliath::commands::CommandItem at id
         * @param id ID
         * @return @see goliath::commands::CommandItem
         */
        CommandItem& operator[](size_t id);
        const CommandItem& operator[](size_t id) const;
        /**
         * @brief Check if a @see goliath::commmands::Command exists
         * @param id ID to check
         * @return Status
         */
        bool commandExists(size_t id) const;
    private:
        std::map<size_t, CommandItem> map;
    };
}
