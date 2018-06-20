#include <goliath/controller/commands/command.h>

#include <boost/range/adaptor/map.hpp>
#include <boost/log/trivial.hpp>

using namespace goliath::commands;

Command::Command(const size_t &id, const std::vector<size_t> &requiredHandles)
    : running(false), id(id), interrupted(false), requiredHandles(requiredHandles) {}

void Command::interrupt() {
    interrupted = true;
    interrupter.notify_all();
}

bool Command::isInterrupted() const {
    return interrupted;
}

const std::vector<size_t> &Command::getRequiredHandles() const {
    return requiredHandles;
}

size_t Command::getId() const {
    return id;
}

bool Command::canStart(const handles::HandleMap &handles) const {
    for (const auto &handle : handles | boost::adaptors::map_values) {
        if (handle->isLocked() && handle->getOwnerId() != getId()) {
            return false;
        }
    }

    return true;
}

bool Command::canRunParallel() const {
    return false;
}

void Command::execute(goliath::handles::HandleMap &handles, const goliath::proto::CommandMessage &message) {
    run(handles, message);
    if (isInterrupted()) {
        BOOST_LOG_TRIVIAL(info) << "Execution of command " << std::to_string(getId()) << " was interrupted";
        interrupted = false;
    }
}

void Command::waitForInterrupt() {
    std::unique_lock<std::mutex> lock(mutex);
    interrupter.wait(lock, [&]() {
        return static_cast<bool>(interrupted);
    });
    lock.unlock();
}
