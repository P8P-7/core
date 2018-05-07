#include <boost/log/trivial.hpp>
#include <chrono>
#include "follow_line_command.h"
#include "../handles.h"
#include <thread>

using namespace goliath::commands;

follow_line_command::follow_line_command() : command({ HANDLE_RIGHT_EYE_CAM, HANDLE_LEFT_EYE_CAM }) { }

void follow_line_command::execute(const handles::handle_map &handles, const Message &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of follow line command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    if (is_interrupted()) {
        BOOST_LOG_TRIVIAL(warning) << "Follow line command was interrupted";
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    BOOST_LOG_TRIVIAL(info) << "Execution of follow line command has finished";
}
