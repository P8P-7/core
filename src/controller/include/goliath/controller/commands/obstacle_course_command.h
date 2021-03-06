#pragma once

#include <goliath/vision.h>
#include <goliath/servo.h>
#include <goliath/motor-controller.h>

#include "basic_command.h"
#include "../handles.h"

/**
 * @file obstacle_course_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class ObstacleCourseCommand : public BasicCommand {
    public:
        ObstacleCourseCommand(const size_t &id, std::shared_ptr<repositories::WingStateRepository> repository);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        handles::HandleMap handleMap;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        void follow_line(vision::FollowLineDetector &followLineDetector, vision::Webcam &camera,
                motor_controller::MotorController &motorController);

        void move(double direction, int speed, motor_controller::MotorController &motorController);
    };
}
