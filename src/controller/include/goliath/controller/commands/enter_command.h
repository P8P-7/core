#pragma once

#include <goliath/motor-controller.h>

#include "../handles.h"
#include "basic_command.h"

/**
 * @file enter_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class EnterCommand : public BasicCommand {
    public:
        EnterCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}

