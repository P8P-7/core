#pragma once

#include "commands/command.h"

/**
 * @file command_item.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
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
}
