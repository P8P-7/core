#pragma once

#include "command.h"
#include "../handles.h"

namespace goliath::commands {
    class MoveTowerCommand : public Command {
    public:
        MoveTowerCommand();

    private:
        void execute(const handles::HandleMap& handles, const CommandMessage &message) override ;

    };
}

