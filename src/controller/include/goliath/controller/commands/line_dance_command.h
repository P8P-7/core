#pragma once

#include "basic_command.h"
#include "../handles.h"

#include <thread>

/**
 * @file line_dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class LineDanceCommand : public BasicCommand {
    public:
        explicit LineDanceCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
