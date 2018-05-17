#pragma once

#include "command_map.h"
#include "handle_map.h"

/**
 * @file command_executor.h
 * @author Group 7 - Informatica
 */

using namespace goliath::handles;

namespace goliath::commands {
    class CommandExecutor {
    public:
        CommandExecutor(CommandMap& commands, HandleMap &handles);
        ~CommandExecutor();

        void run(size_t commandId, const CommandMessage &message);
        void tryExecute(const size_t &commandId, const CommandMessage &message);
        bool canStart(const Command &command) const;
    private:
        std::vector<std::thread> threads;

        CommandMap& commands;
        HandleMap& handles;

        std::mutex mutex;
    };
}
