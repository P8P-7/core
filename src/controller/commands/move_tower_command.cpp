#include "move_tower_command.h"

#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::MoveTowerCommand::MoveTowerCommand()
        : Command({ HANDLE_CAM }) {
}

void commands::MoveTowerCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move tower command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if (isInterrupted()) {
        BOOST_LOG_TRIVIAL(warning) << "Move tower command was interrupted";
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move tower command has finished";
}
