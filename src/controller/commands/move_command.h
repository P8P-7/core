#pragma once

#include "../handles.h"
#include "command.h"

/**
 * @file move_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class MoveCommand : public Command {
    public:
        MoveCommand();

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
