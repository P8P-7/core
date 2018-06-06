#include <boost/log/trivial.hpp>
#include <goliath/controller/command_map.h>

using namespace goliath::commands;
using namespace goliath;

CommandItem::CommandItem()
    : status(CommandStatus::STALE) {}

CommandItem::CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status)
    : instance(commandInstance), status(status) {}

CommandMap::CommandMap(std::shared_ptr<repositories::CommandStatusRepository> statusRepository)
    : statusRepository(statusRepository) {}

CommandMap::CommandMap(std::shared_ptr<repositories::CommandStatusRepository> statusRepository,
                       const std::map<size_t, CommandItem> commands)
    : statusRepository(statusRepository), map(commands) {}

CommandItem &CommandMap::add(std::shared_ptr<Command> command) {
    BOOST_LOG_TRIVIAL(debug) << "Added an instance of "
                             << std::string(boost::core::demangled_name(BOOST_CORE_TYPEID(*command))) << " with id "
                             << std::to_string(command->getId());

    statusRepository->addItem(command->getId());

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

std::shared_ptr<repositories::CommandStatusRepository> &CommandMap::getStatusRepository() {
    return statusRepository;
}

CommandMap::iterator CommandMap::begin() {
    return map.begin();
}

CommandMap::iterator CommandMap::end() {
    return map.end();
}

CommandMap::const_iterator CommandMap::begin() const {
    return map.cbegin();
}

CommandMap::const_iterator CommandMap::end() const {
    return map.cend();
}
