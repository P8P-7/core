#pragma once

#include "basic_command.h"
#include "../handles.h"

/**
 * @file transport_rebuild_command.h
 * @author Group 7 - Informatica
 */

namespace goliath::commands {
    class TransportRebuildCommand : public BasicCommand {
    public:
        explicit TransportRebuildCommand(const size_t &id);

    private:
        void execute(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    };
}
