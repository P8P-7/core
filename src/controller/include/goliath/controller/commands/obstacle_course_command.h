#pragma once

#include <goliath/servo.h>

#include "basic_command.h"
#include "../handles.h"

/**
 * @file obstacle_course_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class ObstacleCourseCommand : public BasicCommand {
    public:
        explicit ObstacleCourseCommand(const size_t &id);

    private:
        std::shared_ptr<repositories::WingStateRepository> repository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
