#include <goliath/controller/repositories/command_status_repository.h>

using namespace goliath::repositories;
using namespace goliath;

void CommandStatusRepository::addItem(size_t commandId, CommandStatusItem::CommandStatus status) {
    items[commandId] = status;
    invalidate();
}

void CommandStatusRepository::addItem(size_t commandId) {
    addItem(commandId, CommandStatusItem::STALE);
}

void CommandStatusRepository::updateItem(size_t commandId, CommandStatusItem::CommandStatus status) {
    items[commandId] = status;
    invalidate();
}

std::unique_ptr<google::protobuf::Message> CommandStatusRepository::getMessage() {
    proto::repositories::CommandStatusRepository commandStatusRepository;

    for (auto item : items) {
        auto *commandStatusItem = commandStatusRepository.add_status();
        commandStatusItem->set_id(item.first);
        commandStatusItem->set_commandstatus(item.second);
    }

    return std::make_unique<proto::repositories::CommandStatusRepository>(commandStatusRepository);
}
