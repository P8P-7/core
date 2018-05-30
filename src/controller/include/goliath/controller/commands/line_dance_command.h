#pragma once

#include "command.h"
#include "../handles.h"

/**
 * @file line_dance_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class LineDanceCommand : public Command {
    public:
        explicit LineDanceCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
