#include "command.h"

using namespace goliath::commands;

Command::Command(const size_t &id, const std::vector<size_t> &requiredHandles)
        : id(id), requiredHandles(requiredHandles) {
}

Command::Command(const size_t& id, const std::vector<size_t>& requiredHandles, const boost::property_tree::ptree &config)
        : id(id), requiredHandles(requiredHandles), config(config) {
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

void Command::run(const goliath::handles::HandleMap &handles, const CommandMessage& message) {
    running = true;
    execute(handles, message);
    running = false;
}

size_t Command::getId() const {
    return id;
}
