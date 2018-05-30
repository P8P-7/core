#pragma once

#include <goliath/servo.h>

#include "basic_command.h"
#include "../handles.h"


namespace goliath::commands {
    class MoveWingCommand : public BasicCommand {
    public:
        MoveWingCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const CommandMessage &message) override;

        void executeServoCommand(std::shared_ptr<handles::ServoHandle> servoHandle, const ServoCommand &servoCommand);

        int enumToHandle(ServoCommand_Motor motor);
    };
}

