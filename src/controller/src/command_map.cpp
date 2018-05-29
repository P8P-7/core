#include <boost/log/trivial.hpp>
#include <goliath/controller/command_map.h>

using namespace goliath::commands;

CommandItem::CommandItem()
        : status(CommandStatus::STALE) {}

CommandItem::CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status)
        : instance(commandInstance), status(status) {}

CommandMap::CommandMap() {}

CommandMap::CommandMap(const std::map<size_t, CommandItem> commands)
        : map(commands) {}

CommandItem &CommandMap::add(std::shared_ptr<Command> command) {
    BOOST_LOG_TRIVIAL(debug) << "Added an instance of "
                             << std::string(boost::core::demangled_name(BOOST_CORE_TYPEID(*command))) << " with id "
                             << std::to_string(command->getId());

    return (map[command->getId()] = {command, CommandStatus::STALE});
}

const CommandItem &CommandMap::operator[](const size_t id) const {
    return map.at(id);
}

CommandItem &CommandMap::operator[](const size_t id) {
    return map.at(id);
}

bool CommandMap::commandExists(const size_t id) const {
    return map.find(id) != map.end();
}

std::map<size_t, CommandItem>::iterator CommandMap::begin() {
    return map.begin();
}

std::map<size_t, CommandItem>::iterator CommandMap::end() {
    return map.end();
}
