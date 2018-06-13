#pragma once

#include "../handles.h"
#include "queue_command.h"

/**
 * @file move_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class MoveCommand : public QueueCommand {
    public:
        using MotorProto = proto::commands::MotorCommand::Motor;

        const static std::map<proto::commands::MotorCommand_Motor, size_t> COMMAND_MOTOR_TO_HANDLE_MAP;

        explicit MoveCommand(const size_t &id);

    private:
        void process() override;
        void execute(const std::vector<proto::commands::MotorCommand> &commandMessages);
    };
}
