#pragma once

#include "command.h"
#include "../handles.h"

/**
 * @file obstacle_course_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class ObstacleCourseCommand : public Command {
    public:
        explicit ObstacleCourseCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
