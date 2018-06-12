#include <goliath/controller/commands/synchronize_commands_command.h>

using namespace goliath::commands;
using namespace goliath;

SynchronizeCommandsCommand::SynchronizeCommandsCommand(const size_t &id, std::shared_ptr<repositories::CommandStatusRepository> &commandStatusRepository)
    : BasicCommand(id, {}), commandStatusRepository(commandStatusRepository) {
}

void SynchronizeCommandsCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    commandStatusRepository->invalidate();
}
