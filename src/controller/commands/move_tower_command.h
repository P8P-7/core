#pragma once

#include "command.h"
#include "../handles.h"

/**
 * @file move_tower_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class MoveTowerCommand : public Command {
    public:
        MoveTowerCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap& handles, const CommandMessage &message) override ;

    };
}

