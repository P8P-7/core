#include "basic_command.h"

using namespace goliath::commands;

BasicCommand::BasicCommand(const size_t &id, const std::vector<size_t> &requiredHandles) : Command(id,
                                                                                                   requiredHandles) {}

void BasicCommand::run(goliath::handles::HandleMap &handles, const CommandMessage &message) {
    running = true;
    BOOST_LOG_TRIVIAL(debug) << "Command " << std::to_string(getId()) << " is being executed";
    execute(handles, message);
    BOOST_LOG_TRIVIAL(debug) << "Command " << std::to_string(getId()) << " has finished";
    handles.unlockAll();
    running = false;
}