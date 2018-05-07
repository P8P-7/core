#include <boost/log/trivial.hpp>
#include <thread>
#include "move_tower_command.h"
#include "../handles.h"

using namespace goliath::commands;
using namespace goliath::handles;

move_tower_command::move_tower_command()
        : command({ HANDLE_RIGHT_EYE_CAM, HANDLE_LEFT_EYE_CAM }) {
}

void move_tower_command::execute(const handle_map &handle, const Message &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move tower command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move tower command has finished";
}
