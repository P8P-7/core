#include <goliath/controller/command_item.h>

using namespace goliath::commands;

CommandItem::CommandItem()
    : status(CommandStatus::STALE) {}

CommandItem::CommandItem(std::shared_ptr<Command> commandInstance, CommandStatus status)
    : instance(std::move(commandInstance)), status(status) {}
