#include <goliath/controller/commands/command.h>

#include <boost/range/adaptor/map.hpp>
#include <boost/log/trivial.hpp>

using namespace goliath::commands;

Command::Command(const size_t &id, const std::vector<size_t> &requiredHandles)
        : running(false), id(id), interrupted(false), requiredHandles(requiredHandles) {}

void Command::interrupt() {
    interrupted = true;
    onInterrupt();
}

void Command::onInterrupt() {}

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
