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
        MoveCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const CommandMessage &message) override;

        const std::map<::MotorCommand_Motor, size_t> commandMotorToHandleMap = {{
                                                                    {::MotorCommand::Motor::MotorCommand_Motor_LEFT_FRONT, HANDLE_LEFT_FRONT_MOTOR},
                                                                    {::MotorCommand::Motor::MotorCommand_Motor_LEFT_BACK, HANDLE_LEFT_BACK_MOTOR},
                                                                    {::MotorCommand::Motor::MotorCommand_Motor_RIGHT_FRONT, HANDLE_RIGHT_FRONT_MOTOR},
                                                                    {::MotorCommand::Motor::MotorCommand_Motor_RIGHT_BACK, HANDLE_RIGHT_BACK_MOTOR},
                                                            }};
    };
}
