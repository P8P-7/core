#pragma once

#include "basic_command.h"
#include "../command_map.h"

/**
 * @file interrupt_command_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class InterruptCommandCommand : public BasicCommand {
    public:
        InterruptCommandCommand(const size_t &id, std::shared_ptr<CommandMap> &commandMap);

    private:
        std::shared_ptr<CommandMap> commandMap;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
