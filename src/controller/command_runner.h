#pragma once

#include <mutex>
#include <thread>
#include <utility>
#include "commands/command_map.h"
#include "handles/handle_map.h"

using namespace goliath::handles;

namespace goliath::commands {
    class command_runner {
    public:
        command_runner(std::shared_ptr<command_map> commands, std::shared_ptr<std::condition_variable> var)
                : commands(std::move(commands)), handles(var), var(var) {}

        void run(const unsigned command_id);
        void execute(const unsigned &command_id, std::shared_ptr<command> instance);
        bool can_start(const command &command) const;

    private:
        std::atomic<int> thread_count{0};

        std::shared_ptr<command_map> commands;
        handle_map handles;

        std::mutex mutex;
        std::shared_ptr<std::condition_variable> var;
    };
}
