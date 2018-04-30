#pragma once

#include <thread>

#include "commands/command_map.h"
#include "handles/handle_map.h"

using namespace goliath::handles;

namespace goliath::commands {
    class command_runner {
    public:
        command_runner(std::shared_ptr<command_map> cm, std::shared_ptr<handle_map> hm)
                : cm(cm), hm(hm) {}

        void run(const unsigned command_id) const;
        bool can_start(const Command &command) const;

    private:
        std::weak_ptr<command_map> cm;
        std::weak_ptr<handle_map> hm;
    };
}
