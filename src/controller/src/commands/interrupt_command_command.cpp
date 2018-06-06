#include <goliath/controller/commands/interrupt_command_command.h>

using namespace goliath::commands;
using namespace goliath;

InterruptCommandCommand::InterruptCommandCommand(const size_t &id, std::shared_ptr<CommandMap> &commandMap)
    : BasicCommand(id, {}), commandMap(commandMap) {
}

void InterruptCommandCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    (*commandMap)[message.interruptcommandcommand().commandid()].instance->interrupt();
}
