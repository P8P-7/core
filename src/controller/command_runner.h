#pragma once

#include <mutex>
#include <thread>
#include <utility>

#include <Message.pb.h>

#include "commands/command_map.h"
#include "handles/handle_map.h"

using namespace goliath::handles;

namespace goliath::commands {
    class command_runner {
    public:
        command_runner(std::shared_ptr<command_map> commands)
                : commands(std::move(commands)) {}

        void run(const unsigned command_id, Message &message);
        void execute(const unsigned &command_id, std::shared_ptr<command> instance, Message &message);
        bool can_start(const command &command) const;

    private:
        std::atomic<int> thread_count{0};

        std::shared_ptr<command_map> commands;
        handle_map handles;

        std::mutex mutex;
    };
}
