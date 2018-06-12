#include <goliath/controller/commands/line_dance_command.h>

using namespace goliath;

commands::LineDanceCommand::LineDanceCommand(const size_t& id)
        : BasicCommand(id, {}) { // TODO: Add motor handles
}

void commands::LineDanceCommand::execute(handles::HandleMap& handles, const proto::CommandMessage& message) {
    // TODO: Implement line dance
}
