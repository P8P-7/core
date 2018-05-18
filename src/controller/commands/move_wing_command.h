#pragma once

#include "command.h"
#include "../handles.h"
#include <goliath/servo.h>

namespace goliath::commands {
    class MoveWingCommand : public Command {
    public:
        MoveWingCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;

        void executeServoCommand(std::shared_ptr<handles::ServoHandle> servoHandle, const ServoCommand &servoCommand);

        int enumToHandle(ServoCommand_Motor motor);
    };
}

