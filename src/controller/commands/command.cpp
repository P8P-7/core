#include "command.h"

using namespace goliath::commands;

command::command(const std::vector<size_t> &required_handles)
        : required_handles(required_handles) {}

void command::interrupt() {
    interrupted = true;
}

bool command::is_interrupted() const {
    return interrupted;
}

const std::vector<size_t>& command::get_required_handles() const {
    return required_handles;
}

void command::run(const goliath::handles::handle_map &handles) {
    running = true;
    execute(handles);
    running = false;
}
