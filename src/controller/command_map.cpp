#include "command_map.h"

using namespace goliath::commands;

CommandItem::CommandItem()
        : status(CommandStatus::STALE) {
}
CommandItem::CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status)
        : instance(commandInstance), status(status) {
}

CommandMap::CommandMap() {}
CommandMap::CommandMap(const std::map<size_t, CommandItem> commands)
        : map(commands) {
}

void CommandMap::add(const size_t commandId, std::shared_ptr<Command> command) {
    map[commandId] = { command, CommandStatus::STALE };
}

const CommandItem& CommandMap::operator[](const size_t id) const {
    return map.at(id);
}

CommandItem& CommandMap::operator[](const size_t id) {
    return map.at(id);
}

bool CommandMap::commandExists(const size_t id) const {
    return map.find(id) != map.end();
}

