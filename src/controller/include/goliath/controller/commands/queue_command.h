#pragma once

#include <thread>

#include "command.h"

namespace goliath::commands {
    /**
     * @class goliath::commands::QueueCommand executes the incoming command in a worker thread.
     */
    class QueueCommand : public Command {
    public:
        QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles);
        ~QueueCommand();

        void run(handles::HandleMap &handles, const proto::CommandMessage &message) override;

        /**
         * @brief always true because a QueueCommand can receive command in parallel
         * @return true
         */
        bool canRunParallel() const override;

        void interrupt() override;
    protected:
        /**
         * @brief mutex protecting the queue
         */
        std::mutex queueMutex;
        std::condition_variable cv;

        handles::HandleMap handles;
        std::queue<proto::CommandMessage> queue;

        /**
         * Should be overriden by a child. Processes items in the queue.
         */
        virtual void process() = 0;
    private:
        void work();

        bool isWorking;
        std::thread worker;
    };
}
