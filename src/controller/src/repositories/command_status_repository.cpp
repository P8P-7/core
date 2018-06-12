#include <goliath/controller/repositories/command_status_repository.h>

using namespace goliath::repositories;
using namespace goliath;

void CommandStatusRepository::addItem(size_t commandId, commands::CommandStatus status) {
    items[commandId] = status;
    invalidate();
}

void CommandStatusRepository::addItem(size_t commandId) {
    addItem(commandId, commands::CommandStatus::STALE);
}

void CommandStatusRepository::updateItem(size_t commandId, commands::CommandStatus status) {
    addItem(commandId, status);
}

std::unique_ptr<google::protobuf::Message> CommandStatusRepository::getMessage() {
    proto::repositories::CommandStatusRepository commandStatusRepository;

    for (auto item : items) {
        auto *commandStatusItem = commandStatusRepository.add_status();
        commandStatusItem->set_id(item.first);

        CommandStatusItem::CommandStatus status = CommandStatusItem::STALE;

        switch (item.second) {
            case commands::CommandStatus::STALE:
                status = CommandStatusItem::STALE;
                break;
            case commands::CommandStatus::STARTING:
                status = CommandStatusItem::STARTING;
                break;
            case commands::CommandStatus::STARTED:
                status = CommandStatusItem::STARTED;
                break;
        }

        commandStatusItem->set_commandstatus(status);
    }

    return std::make_unique<proto::repositories::CommandStatusRepository>(commandStatusRepository);
}
