#include <goliath/controller/commands/invalidate_all_command.h>

using namespace goliath;
using namespace goliath::repositories;

commands::InvalidateAllCommand::InvalidateAllCommand(const size_t &id,  std::shared_ptr<Watcher> watcher)
        : BasicCommand(id, {}), watcher(std::move(watcher)) {
}

void commands::InvalidateAllCommand::execute(goliath::handles::HandleMap &handles, const proto::CommandMessage &message) {
    watcher->invalidateAll();
}
