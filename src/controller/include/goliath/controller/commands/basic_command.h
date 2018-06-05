#pragma once

#include "command.h"

namespace goliath::commands {
    class BasicCommand : public Command {
    public:
        BasicCommand(const size_t &id, const std::vector<size_t> &requiredHandles);

        void run(handles::HandleMap &handles, const proto::CommandMessage &message) override;
    protected:
        /**
         * @brief The actual execute method implemented by underlying commands
         * @param handles Handle map to be passed to implementation to be unlocked dynamically
         * @param message Arguments to be passed to implementers
         */
        virtual void execute(handles::HandleMap &handles, const proto::CommandMessage &message) = 0;
    };
}
