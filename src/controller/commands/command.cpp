#include "command.h"

using namespace goliath::commands;

void Command::interrupt() {
    interrupted = true;
}

const std::vector<unsigned>& Command::get_handles() const {
    return occupying_handles;
}