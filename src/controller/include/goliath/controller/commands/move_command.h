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

        /**
         * @bief map of a protobuf motor id to a handle id representing the id
         */
        const static std::map<proto::commands::MotorCommand_Motor, size_t> COMMAND_MOTOR_TO_HANDLE_MAP;

        explicit MoveCommand(const size_t &id);

    private:
        /**
         * Process queue items and combine multiple items in the queue to reduce traffic on the i2c bus
         */
        void process() override;
        /**
         * Execute multiple commands and send to motor controller
         * @param commandMessages commands to process
         */
        void execute(const std::vector<proto::commands::MotorCommand> &commandMessages);
    };
}
