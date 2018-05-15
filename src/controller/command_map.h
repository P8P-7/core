#pragma once

#include <atomic>
#include <map>
#include <memory>

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
    enum class command_status {
        STARTING, /**< Command is just getting started */
        STARTED, /**< Command has already started */
        STALE /**< Command is doing nothing (idle) */
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
