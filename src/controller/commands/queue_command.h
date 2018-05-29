#pragma once

#include "command.h"

namespace goliath::commands {
    class QueueCommand : public Command {
    public:
        QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles);

        ~QueueCommand();

        void run(handles::HandleMap &handles, const CommandMessage &message) override;

    protected:
        virtual void execute(handles::HandleMap &handles, const CommandMessage &message) = 0;

    private:
        void work();

        handles::HandleMap handles;
        std::queue<CommandMessage> queue;

        bool isWorking;
        std::thread worker;

        std::mutex mutex;
        std::condition_variable cv;
    };
}
