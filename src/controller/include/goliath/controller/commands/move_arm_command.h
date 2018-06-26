#pragma once

#include <goliath/servo.h>

#include "basic_command.h"
#include "../handles.h"

namespace goliath::commands {
    class MoveArmCommand : public BasicCommand {
    public:
        explicit MoveArmCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}

