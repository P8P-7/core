#include <goliath/controller/commands/interrupt_command_command.h>

using namespace goliath::commands;
using namespace goliath;

InterruptCommandCommand::InterruptCommandCommand(const size_t &id, std::shared_ptr<CommandMap> &commandMap)
    : BasicCommand(id, {}), commandMap(commandMap) {
}

void InterruptCommandCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    if ((*commandMap).commandExists(static_cast<size_t>(message.interruptcommandcommand().commandid()))) {
        CommandItem item = (*commandMap)[message.interruptcommandcommand().commandid()];

        if (!item.instance->isInterrupted()) {
            item.instance->interrupt();
        }
    }
}
