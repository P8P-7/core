#pragma once

#include <goliath/servo/wings/wing_command.h>
#include <goliath/servo/wings/wing_controller.h>
#include <goliath/servo/repositories/wing_state_repository.h>
#include <goliath/motor-controller.h>

#include "../handles.h"
#include "basic_command.h"

/**
 * @file dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class DanceCommand : public BasicCommand {
    public:
        DanceCommand(const size_t &id, const std::shared_ptr<repositories::WingStateRepository> &repository,
                     const std::uint8_t chainSawSpeedLow, const std::uint8_t chainSawSpeedMedium,
                     const std::uint8_t chainSawSpeedFast, const std::uint8_t chainSawSpeedExtraFast);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        const std::uint8_t chainSawSpeedLow;
        const std::uint8_t chainSawSpeedMedium;
        const std::uint8_t chainSawSpeedFast;
        const std::uint8_t chainSawSpeedExtraFast;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        void saw(motor_controller::MotorController &motorController, motor_controller::MotorId motor, std::uint8_t speed);
        void moveWingOverTime(const std::chrono::milliseconds &duration, servo::WingController &wingController,
                              std::vector<servo::WingCommand> commands);
        motor_controller::MotorId motorHandleToId(handles::HandleMap &handles, size_t handle);

        void stand(handles::HandleMap &handles);
    };
}
