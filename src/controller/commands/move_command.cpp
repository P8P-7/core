#include <boost/log/trivial.hpp>
#include <thread>
#include <chrono>

#include "move_command.h"

goliath::commands::move_command::move_command()
        : command({}) {
}

void goliath::commands::move_command::execute(const handles::handle_map &handles, const Message &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has started";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    BOOST_LOG_TRIVIAL(info) << "Execution of move command has finished";
}
