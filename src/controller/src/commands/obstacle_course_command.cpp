#include <goliath/controller/commands/obstacle_course_command.h>

using namespace goliath;

commands::ObstacleCourseCommand::ObstacleCourseCommand(const size_t& id)
        : BasicCommand(id, {}) { // TODO: Add motor handles
}

void commands::ObstacleCourseCommand::execute(handles::HandleMap& handles, const proto::CommandMessage& message) {
    // TODO: Implement obstacle course command
}
