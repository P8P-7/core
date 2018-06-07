#include <goliath/controller/commands/enter_command.h>

using namespace goliath::commands;
using namespace goliath;

EnterCommand::EnterCommand(const size_t &id) // TODO: Add arguments
    : BasicCommand(id, {}) { // TODO: Add handles
}

void EnterCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    // TODO: Add implementation
}
