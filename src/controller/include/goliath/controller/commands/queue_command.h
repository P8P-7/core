#pragma once

#include "command.h"

namespace goliath::commands {
    class QueueCommand : public Command {
    public:
        QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles);

        ~QueueCommand();

        void run(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        bool canRunParallel() const override;

    protected:
        virtual void execute(handles::HandleMap &handles, const proto::CommandMessage &message) = 0;

        void onInterrupt() override;
    private:
        void work();

        handles::HandleMap handles;
        std::queue<proto::CommandMessage> queue;

        std::mutex mutex;
        std::condition_variable cv;

        bool isWorking;
        std::thread worker;
    };
}
