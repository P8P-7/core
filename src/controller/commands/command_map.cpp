#include "command_map.h"

using namespace goliath::commands;

bool command_map::get_command_status(const unsigned command_id) {
    return true;
}

void command_map::add_command(const unsigned command_id, Command *command) {
    map[command_id] = {command, command_status::STALE};
}

Command* command_map::get_instance(const unsigned command_id) {
    if(get_command_status(command_id))
        return map[command_id].command;
    else
        return nullptr;
}

