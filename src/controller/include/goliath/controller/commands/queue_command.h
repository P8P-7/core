#pragma once

#include "command.h"

namespace goliath::commands {
    class QueueCommand : public Command {
    public:
        QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles);
        ~QueueCommand();

        void run(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        bool canRunParallel() const override;
        void interrupt() override;
    protected:
        std::mutex mutex;
        std::condition_variable cv;

        handles::HandleMap handles;
        std::queue<proto::CommandMessage> queue;

        virtual void process() = 0;
    private:
        void work();

        bool isWorking;
        std::thread worker;
    };
}
