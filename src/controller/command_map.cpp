#include "command_map.h"

using namespace goliath::commands;

command_item::command_item()
        : status(command_status::STALE) { }
command_item::command_item(std::shared_ptr<command> command_instance, command_status status)
        : instance(command_instance), status(status) { }

command_map::command_map() {}
command_map::command_map(const std::map<size_t, command_item> commands)
        : map(commands) {
}

void command_map::add(const size_t command_id, std::shared_ptr<command> command) {
    map[command_id] = { command, command_status::STALE };
}

const command_item& command_map::operator[](const size_t id) const {
    return map.at(id);
}

command_item& command_map::operator[](const size_t id) {
    return map.at(id);
}

bool command_map::command_exists(const size_t id) const {
    return map.find(id) != map.end();
}

