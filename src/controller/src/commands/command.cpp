#include <goliath/controller/commands/command.h>

#include <boost/log/trivial.hpp>

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

void Command::run(const goliath::handles::HandleMap &handles, const CommandMessage& message) {
    running = true;
    BOOST_LOG_TRIVIAL(debug) << "Command " << std::to_string(getId()) << " is being executed";
    execute(handles, message);
    BOOST_LOG_TRIVIAL(debug) << "Command " << std::to_string(getId()) << " has finished";
    running = false;
}

size_t Command::getId() const {
    return id;
}
