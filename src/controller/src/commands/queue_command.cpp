#include "goliath/controller/commands/queue_command.h"

#include <thread>

using namespace goliath::commands;

QueueCommand::QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles)
        : Command(id, requiredHandles), queue{}, isWorking(true), worker(&QueueCommand::work, this) {}

void QueueCommand::run(goliath::handles::HandleMap &handles, const CommandMessage &message) {
    std::lock_guard<std::mutex> lock(mutex);

    if (!isWorking) {
        BOOST_LOG_TRIVIAL(warning) << "Worker isn't working, please restart";
        return;
    }

    this->handles = handles;
    CommandMessage copy = message;
    queue.emplace(copy);

    cv.notify_one();
}

void QueueCommand::work() {
    std::unique_lock<std::mutex> lock(mutex);

    while (isWorking) {
        BOOST_LOG_TRIVIAL(debug) << "Worker has started working... waiting for messages";
        cv.wait(lock, [&]() {
            return !isWorking || isInterrupted() || !queue.empty();
        });

        BOOST_LOG_TRIVIAL(debug) << "Worker has been awakened";

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
                BOOST_LOG_TRIVIAL(error) << "Error executing command \"" << getId() << "\" what(): "
                                         << ex.what();
            }

            lock.lock();
        }
        handles.unlockAll();
        running = false;

        BOOST_LOG_TRIVIAL(debug) << "Worker has finished working";
    }
    BOOST_LOG_TRIVIAL(debug) << "Worker has exited";
}

QueueCommand::~QueueCommand() {
    std::unique_lock<std::mutex> lock(mutex);

    if (!isWorking) {
        return;
    }

    isWorking = false;
    cv.notify_one();
    worker.join();
}

void QueueCommand::onInterrupt() {
    cv.notify_one();
}

bool QueueCommand::canRunParallel() const {
    return true;
}
