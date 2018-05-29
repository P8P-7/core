#include "transport_rebuild_command.h"

using namespace goliath;

commands::TransportRebuildCommand::TransportRebuildCommand(const size_t& id)
        : Command(id, {HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO, HANDLE_RIGHT_FRONT_WING_SERVO,
                       HANDLE_RIGHT_BACK_WING_SERVO}) { // TODO: Add motor handles
}

void commands::TransportRebuildCommand::execute(const goliath::handles::HandleMap& handles, const CommandMessage& message) {
    // TODO: Implement transport rebuild command
}
