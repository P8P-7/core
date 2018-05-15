#pragma once

#include <mutex>
#include <thread>
#include <utility>

#include "command_map.h"
#include "handle_map.h"

/**
 * @file command_executor.h
 * @author Group 7 - Informatica
 */

using namespace goliath::handles;

namespace goliath::commands {
    class command_executor {
    public:
        command_executor(command_map& commands, handle_map &handles);
        ~command_executor();

        void run(size_t command_id, const Message &message);
        void try_execute(const size_t &command_id, const Message &message);
        bool can_start(const command &command) const;
    private:
        std::vector<std::thread> threads;

        command_map& commands;
        handle_map& handles;

        std::mutex mutex;
    };
}
