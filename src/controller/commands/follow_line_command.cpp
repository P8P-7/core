#include "follow_line_command.h"

#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::FollowLineCommand::FollowLineCommand()
    : Command({ HANDLE_RIGHT_EYE_CAM, HANDLE_LEFT_EYE_CAM }) {
}

void commands::FollowLineCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of follow line command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if (isInterrupted()) {
        BOOST_LOG_TRIVIAL(warning) << "Follow line command was interrupted";
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    BOOST_LOG_TRIVIAL(info) << "Execution of follow line command has finished";
}
