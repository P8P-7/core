#pragma once

#include <goliath/servo.h>

#include "basic_command.h"
#include "../handles.h"


namespace goliath::commands {
    class MoveWingCommand : public BasicCommand {
    public:
        MoveWingCommand(const size_t &id, std::shared_ptr<repositories::WingStateRepository> repository);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        void executeServoCommand(std::shared_ptr<handles::WingHandle> handle,
                                 const proto::commands::ServoCommand &command);

        static size_t enumToHandle(proto::commands::ServoCommand_Motor motor);
    };
}

