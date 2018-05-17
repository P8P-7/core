#pragma once

#include "../handles.h"
#include "command.h"

namespace goliath::commands {
    class MoveCommand : public Command {
    public:
        MoveCommand();

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
