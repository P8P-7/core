#pragma once

#include "../handles.h"
#include "basic_command.h"

/**
 * @file dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class DanceCommand : public BasicCommand {
    public:
        explicit DanceCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
