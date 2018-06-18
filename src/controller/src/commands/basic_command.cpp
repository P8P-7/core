#include "goliath/controller/commands/basic_command.h"

using namespace goliath::commands;

BasicCommand::BasicCommand(const size_t &id, const std::vector<size_t> &requiredHandles)
    : Command(id, requiredHandles) {
}

void BasicCommand::run(goliath::handles::HandleMap &handles, const proto::CommandMessage &message) {
    running = true;
    execute(handles, message);
    handles.unlockAll();
    running = false;
}
