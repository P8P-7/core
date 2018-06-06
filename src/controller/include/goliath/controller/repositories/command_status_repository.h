#pragma once

#include <string>

#include <repositories/CommandStatusRepository.pb.h>
#include <goliath/foundation/repositories/repository.h>

/**
 * @file command_status_repository.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    class CommandStatusRepository : public Repository {
    public:
        using CommandStatusItem = goliath::proto::repositories::CommandStatusItem;

        void addItem(size_t commandId);
        void addItem(size_t commandId, CommandStatusItem::CommandStatus status);

        void updateItem(size_t commandId, CommandStatusItem::CommandStatus status);

        std::unique_ptr<google::protobuf::Message> getMessage() override;

    private:
        std::map<int, CommandStatusItem::CommandStatus> items;
    };
}
