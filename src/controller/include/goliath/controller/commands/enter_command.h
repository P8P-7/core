#pragma once

#include <goliath/motor-controller.h>

#include "../handles.h"
#include "basic_command.h"
#include "move_command.h"

/**
 * @file enter_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class EnterCommand : public BasicCommand {
    public:
        EnterCommand(const size_t &id, const motor_controller::MotorDirection &direction,
                     const motor_controller::MotorSpeed &speed);

    private:
        const motor_controller::MotorDirection direction;
        const motor_controller::MotorSpeed speed;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}

