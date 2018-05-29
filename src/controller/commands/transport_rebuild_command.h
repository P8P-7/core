#pragma once

#include "command.h"
#include "../handles.h"

/**
 * @file transport_rebuild_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class TransportRebuildCommand : public Command {
    public:
        explicit TransportRebuildCommand(const size_t &id);

    private:
        void execute(const handles::HandleMap &handles, const CommandMessage &message) override;
    };
}
