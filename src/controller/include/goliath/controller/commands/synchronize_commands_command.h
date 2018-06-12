#pragma once

#include "basic_command.h"
#include "../repositories/command_status_repository.h"

/**
 * @file synchronize_commands_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class SynchronizeCommandsCommand : public BasicCommand {
    public:
        SynchronizeCommandsCommand(const size_t &id,
                                   std::shared_ptr<repositories::CommandStatusRepository> &commandStatusRepository);

    private:
        std::shared_ptr<repositories::CommandStatusRepository> commandStatusRepository;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
