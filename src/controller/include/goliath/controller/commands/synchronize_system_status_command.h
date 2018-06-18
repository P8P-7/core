#pragma once

#include "../handles.h"
#include "../repositories/system_status_repository.h"
#include "basic_command.h"

/**
 * @file fan_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class SynchronizeSystemStatusCommand : public BasicCommand {
    public:
        SynchronizeSystemStatusCommand(const size_t &id,
                               std::shared_ptr<repositories::SystemStatusRepository> systemStatusRepository);

    private:
        static const std::string TEMPERATURE_FILE;

        std::shared_ptr<repositories::SystemStatusRepository> systemStatusRepository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
