#pragma once

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
        void execute(handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
