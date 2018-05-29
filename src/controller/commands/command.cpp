#include <boost/log/trivial.hpp>
#include "command.h"

using namespace goliath::commands;

Command::Command(const size_t &id, const std::vector<size_t> &requiredHandles)
        : id(id), requiredHandles(requiredHandles) {
}

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
