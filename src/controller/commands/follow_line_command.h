#pragma once

#include "../handles.h"
#include "command.h"

namespace goliath::commands {
    class FollowLineCommand : public Command {
    public:
        FollowLineCommand();

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}

