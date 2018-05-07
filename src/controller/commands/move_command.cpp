#include "move_command.h"
#include <boost/log/trivial.hpp>
#include <thread>
#include <chrono>

using namespace goliath::commands;

move_command::move_command() : command({}) {}

void move_command::execute(const handles::handle_map &handles) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has finished";
}
