#pragma once

#include "commands/command.h"

/**
 * @file command_map.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    /**
     * @enum goliath::commands::CommandStatus
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
         * @brief Add a goliath::commands::CommandItem to the map
         * @param command Pointer to @see goliath::commands::Command instance
         */
        CommandItem& add(std::shared_ptr<Command> command);

        /**
         * @brief Add a goliath::commands::CommandItem to the map.
         * Creates an instance of CommandType with handleId and args passed to it as arguments. CommandType's constructor should accept an commandId as its first argument.
         *
         * @tparam CommandType type to create an Command of
         * @tparam Targs types of arguments passed to
         * @param commandId passed to the command created
         * @param args arguments passed the command in the constructor after the commandId
         * @return goliath::commands::CommandItem with
         */
        template<typename CommandType, typename ...Targs>
        CommandItem& add(size_t commandId, Targs... args);

        /**
         * @brief Get goliath::commands::CommandItem at id
         * @param id ID
         * @return @see goliath::commands::CommandItem
         */
        CommandItem& operator[](size_t id);
        const CommandItem& operator[](size_t id) const;
        /**
         * @brief Check if a goliath::commands::Command exists
         * @param id ID to check
         * @return Status
         */
        bool commandExists(size_t id) const;

        /**
         * @brief Begin iterator
         * @return std::map iterator
         */
        std::map<size_t, CommandItem>::iterator begin();
        /**
         * @brief End iterator
         * @return std::map iterator
         */
        std::map<size_t, CommandItem>::iterator end();
    private:
        std::map<size_t, CommandItem> map;
    };

    template<typename CommandType, typename ...Targs>
    CommandItem& CommandMap::add(size_t commandId, Targs... args) {
        auto command = std::make_shared<CommandType>(commandId, args...);

        return add(command);
    }
}
