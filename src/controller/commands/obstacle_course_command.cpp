#include "obstacle_course_command.h"

using namespace goliath;

commands::ObstacleCourseCommand::ObstacleCourseCommand(const size_t& id)
        : BasicCommand(id, {HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO, HANDLE_RIGHT_FRONT_WING_SERVO,
                       HANDLE_RIGHT_BACK_WING_SERVO}) { // TODO: Add motor handles
}

void commands::ObstacleCourseCommand::execute(handles::HandleMap& handles, const CommandMessage& message) {
    // TODO: Implement obstacle course command
}
