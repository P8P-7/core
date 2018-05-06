#include "move_command.h"
#include <boost/log/trivial.hpp>
#include <thread>
#include <chrono>

goliath::commands::move_command::move_command() : command({}) {}

void goliath::commands::move_command::execute() {
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has finished";
}
