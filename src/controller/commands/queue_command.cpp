#include "queue_command.h"

#include <thread>

using namespace goliath::commands;

QueueCommand::QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles) : Command(id,
                                                                                                   requiredHandles),
                                                                                           queue{},
                                                                                           isWorking(true),
                                                                                           worker(&QueueCommand::work,
                                                                                                  this) {}

void QueueCommand::run(goliath::handles::HandleMap &handles, const CommandMessage &message) {
    std::lock_guard<std::mutex> lock(mutex);

    this->handles = handles;
    CommandMessage copy = message;
    queue.emplace(copy);

    cv.notify_all();
}

void QueueCommand::work() {
    std::unique_lock<std::mutex> lock(mutex);

    while (isWorking) {
        cv.wait(lock, [&]() {
            return !isWorking || !queue.empty() || isInterrupted();
        });

        BOOST_LOG_TRIVIAL(info) << "Worker has been awakened";

        if (!isWorking || isInterrupted()) {
            break;
        }

        running = true;
        while (!queue.empty() && !isInterrupted()) {
            handles::HandleMap handles = this->handles;
            CommandMessage message = queue.front();
            queue.pop();
            lock.unlock();

            try {
                execute(handles, message);
            } catch (std::exception &ex) {
                BOOST_LOG_TRIVIAL(fatal) << "Error executing command \"" << getId() << "\" what(): "
                                         << ex.what();
            }
            lock.lock();
        }
        lock.unlock();
        handles.unlockAll();
        running = false;

        BOOST_LOG_TRIVIAL(info) << "Worker has finished working";
    }
}

QueueCommand::~QueueCommand() {
    std::unique_lock<std::mutex> lock(mutex);

    isWorking = false;
    cv.notify_one();
    worker.join();
}
