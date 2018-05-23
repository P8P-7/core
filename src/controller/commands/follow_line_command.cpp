#include "follow_line_command.h"

#include <goliath/vision.h>

#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::FollowLineCommand::FollowLineCommand(const size_t &id)
    : Command(id, { HANDLE_CAM }) {
}

void commands::FollowLineCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    if(isInterrupted()) {
        return;
    }

    ::FollowLineCommand followLineCommand = message.followlinecommand();

    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
}
