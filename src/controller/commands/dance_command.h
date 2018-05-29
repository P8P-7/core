#pragma once

#include "../handles.h"
#include "command.h"

/**
 * @file dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class DanceCommand : public Command {
    public:
        DanceCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
