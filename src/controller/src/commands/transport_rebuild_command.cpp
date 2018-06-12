#include <goliath/controller/commands/transport_rebuild_command.h>

using namespace goliath;

commands::TransportRebuildCommand::TransportRebuildCommand(const size_t& id)
        : BasicCommand(id, {}) { // TODO: Add motor handles
}

void commands::TransportRebuildCommand::execute(handles::HandleMap& handles, const proto::CommandMessage& message) {
    // TODO: Implement transport rebuild command
}
