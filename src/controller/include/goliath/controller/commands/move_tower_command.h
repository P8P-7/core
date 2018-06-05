#pragma once

#include "basic_command.h"
#include "../handles.h"

/**
 * @file move_tower_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class MoveTowerCommand : public BasicCommand {
    public:
        MoveTowerCommand(const size_t &id);

    private:
        void execute(handles::HandleMap& handles, const proto::CommandMessage &message) override;

    };
}

