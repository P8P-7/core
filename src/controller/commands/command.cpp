#include <boost/log/trivial.hpp>
#include "command.h"

using namespace goliath::commands;

Command::Command(const size_t &id, const std::vector<size_t> &requiredHandles)
        : running(false), id(id), interrupted(false), requiredHandles(requiredHandles) { }

void Command::interrupt() {
    interrupted = true;
}

bool Command::isInterrupted() const {
    return interrupted;
}

const std::vector<size_t>& Command::getRequiredHandles() const {
    return requiredHandles;
}

size_t Command::getId() const {
    return id;
}

void Command::onInterrupt() { }

bool Command::canStart(const goliath::handles::HandleMap &handles) const {
    for (const size_t handleId : getRequiredHandles()) {
        if (handles[handleId]->isLocked() && handles[handleId]->getOwnerId() != getId()) {
            return false;
        }
    }

    return true;
}

bool Command::canRunParallel() const {
    return false;
}
