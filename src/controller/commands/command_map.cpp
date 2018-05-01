#include "command_map.h"

using namespace goliath::commands;

void command_map::add(const unsigned command_id, std::shared_ptr<Command> command) {
    map[command_id] = {command, command_status::STALE};
}

command_status command_map::get_command_status(const unsigned command_id) const {
    return map[command_id].status;
}

void command_map::set_command_status(const unsigned command_id, const command_status status) const {
    map[command_id].status = status;
}


std::weak_ptr<Command> command_map::get_instance(const unsigned command_id) {
    if(get_command_status(command_id) == command_status::STALE)
        return map[command_id].command;
    return std::shared_ptr<Command>(nullptr);
}