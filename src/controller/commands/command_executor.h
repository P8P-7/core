#pragma once

#include <mutex>
#include <thread>
#include <utility>
#include "command_map.h"
#include "../handles/handle_map.h"

using namespace goliath::handles;

namespace goliath::commands {
    class command_executor {
    public:
        command_executor(command_map& commands, handle_map &handles);
        ~command_executor();

        void run(size_t command_id);
        void try_execute(const size_t &command_id);
        bool can_start(const command &command) const;
    private:
        std::vector<std::thread> threads;

        command_map& commands;
        handle_map handles;

        std::mutex mutex;
    };
}
