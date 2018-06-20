#pragma once

#include "../watcher.h"
#include "../handles.h"
#include "basic_command.h"

/**
 * @file invalidate_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class InvalidateAllCommand : public BasicCommand {
    public:
        InvalidateAllCommand(const size_t &id, std::shared_ptr<repositories::Watcher> watcher);

    private:
        std::shared_ptr<repositories::Watcher> watcher;

        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
