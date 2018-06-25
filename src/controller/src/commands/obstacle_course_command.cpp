#include <goliath/controller/commands/obstacle_course_command.h>

using namespace goliath;

commands::ObstacleCourseCommand::ObstacleCourseCommand(const size_t& id, std::shared_ptr<repositories::WingStateRepository> repository)
        : BasicCommand(id, {}), repository(repository) { // TODO: Add motor handles
}

void commands::ObstacleCourseCommand::execute(handles::HandleMap& handles, const proto::CommandMessage& message) {
    // TODO: Implement obstacle course command
}
