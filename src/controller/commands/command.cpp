#include "command.h"

using namespace goliath::commands;

command::command(const std::vector<unsigned> &occupying_handles)
        : occupying_handles(occupying_handles) {}

void command::interrupt() {
    interrupted = true;
}

const std::vector<unsigned>& command::get_handles() const {
    return occupying_handles;
}