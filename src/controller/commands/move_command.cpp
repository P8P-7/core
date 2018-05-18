#include "move_command.h"

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::MoveCommand::MoveCommand(const size_t &id)
        : Command(id, {}) {
}

void commands::MoveCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has finished";
}
