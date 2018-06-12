#include "goliath/controller/commands/queue_command.h"

#include <thread>

using namespace goliath;
using namespace goliath::commands;

QueueCommand::QueueCommand(const size_t &id, const std::vector<size_t> &requiredHandles)
        : Command(id, requiredHandles), queue{}, isWorking(true), worker(&QueueCommand::work, this) { }

QueueCommand::~QueueCommand() {
    std::unique_lock<std::mutex> lock(mutex);

    if (!isWorking) {
        return;
    }

    isWorking = false;
    lock.unlock();
    cv.notify_all();
    worker.join();
}

void QueueCommand::run(goliath::handles::HandleMap &handles, const proto::CommandMessage &message) {
    std::lock_guard<std::mutex> lock(mutex);

    if (!isWorking) {
        BOOST_LOG_TRIVIAL(warning) << "Worker isn't working, please restart";
        return;
    }

    this->handles = handles;
    proto::CommandMessage copy = message;
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
            running = false;
            handles.unlockAll();
            while (!queue.empty()) {
                queue.pop();
            }
            break;
        }

        running = true;
        while (!queue.empty() && !isInterrupted()) {
            lock.unlock();

            process();

            lock.lock();
        }
        handles.unlockAll();
        running = false;

        BOOST_LOG_TRIVIAL(debug) << "Worker has finished working";
    }
    BOOST_LOG_TRIVIAL(debug) << "Worker has exited";
}

void QueueCommand::interrupt() {
    Command::interrupt();

    cv.notify_one();
}

bool QueueCommand::canRunParallel() const {
    return true;
}
