#pragma once

#include <goliath/servo.h>

#include "basic_command.h"
#include "../handles.h"


namespace goliath::commands {
    class MoveWingCommand : public BasicCommand {
    public:
        explicit MoveWingCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        void executeServoCommand(std::shared_ptr<handles::ServoHandle> servoHandle,
                                 const proto::commands::ServoCommand &servoCommand);

        int enumToHandle(proto::commands::ServoCommand_Motor motor);
    };
}

