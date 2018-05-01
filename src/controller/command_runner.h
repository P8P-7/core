#pragma once

#include <mutex>
#include <thread>

#include "commands/command_map.h"
#include "handles/handle_map.h"

using namespace goliath::handles;

namespace goliath::commands {
    class command_runner {
    public:
        command_runner(std::shared_ptr<command_map> cm, std::shared_ptr<std::condition_variable> var)
                : cm(cm), hm(var), var(var) {}

        void run(const unsigned command_id);
        void execute(const unsigned &command_id, std::shared_ptr<Command> instance);
        bool can_start(const Command &command) const;

    private:
        std::atomic<int> thread_count{0};

        std::weak_ptr<command_map> cm;
        handle_map hm;

        std::mutex mutex;
        std::shared_ptr<std::condition_variable> var;
    };
}
