#pragma once

#include <goliath/servo.h>
#include "../handles.h"
#include "basic_command.h"

/**
 * @file set_wing_position_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class SetWingPositionCommand : public BasicCommand {
    public:
        explicit SetWingPositionCommand(const size_t &id, std::shared_ptr<repositories::WingStateRepository> repository);

        void setFlat(handles::HandleMap &handles, std::uint16_t speed);
        void setUp(handles::HandleMap &handles, std::uint16_t speed);
        void setRamp(handles::HandleMap &handles, std::uint16_t speed);
        void setDown(handles::HandleMap &handles, std::uint16_t speed);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        void calibrate(handles::HandleMap &handles);

        servo::WingCommand buildCommand(handles::WingHandle &handle, std::uint16_t speed, double angle, servo::Direction direction);
        servo::WingCommand buildCommand(handles::WingHandle &handle, std::uint16_t speed, double angle);
    };
}
