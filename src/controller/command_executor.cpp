#include "command_executor.h"
#include <boost/log/trivial.hpp>

using namespace goliath::commands;

CommandExecutor::CommandExecutor(CommandMap &commands, HandleMap &handles)
        : commands(commands), handles(handles) {
}

CommandExecutor::~CommandExecutor() {
    for (auto &thread : threads) {
        thread.join();
    }
}

void CommandExecutor::run(const size_t commandId, const CommandMessage &message) {
    if (!commands.commandExists(commandId)) {
        throw std::runtime_error("Command does not exist"); // TODO: Own exception class?
    }

    auto &item = commands[commandId];
    if (item.status != CommandStatus::STALE) {
        BOOST_LOG_TRIVIAL(warning) << "Command " << commandId << " was dropped because it was already running";
        return;
    }
    item.status = CommandStatus::STARTING;

    threads.emplace_back(&CommandExecutor::tryExecute, this, commandId, message);
}

void CommandExecutor::tryExecute(const size_t &commandId, const CommandMessage &message) {
    std::unique_lock<std::mutex> lock(mutex);

    CommandItem &item = commands[commandId];
    auto requiredHandles = handles.getHandles(item.instance->getRequiredHandles());
    if (canStart(*(item.instance))) {
        lock.unlock();

        requiredHandles.lockAll(commandId);
        item.status = CommandStatus::STARTED;

        BOOST_LOG_TRIVIAL(info) << "Execution of \"Command " << commandId << "\" has started";
        item.instance->run(requiredHandles, message);
        BOOST_LOG_TRIVIAL(info) << "Execution of \"Command " << commandId << "\" has finished";

        requiredHandles.unlockAll();

        lock.lock();
        item.status = CommandStatus::STALE;
        return;
    }

    lock.unlock();
    for (size_t handleId : item.instance->getRequiredHandles()) {
        size_t lockerId = handles[handleId]->getOwnerId();
        commands[lockerId].instance->interrupt();
        requiredHandles[handleId]->waitAndLock(commandId);
    }

    item.status = CommandStatus::STARTED;
    item.instance->run(requiredHandles, message);
    lock.lock();
    item.status = CommandStatus::STALE;
}

bool CommandExecutor::canStart(const Command &command) const {
    for (const size_t handleId : command.getRequiredHandles()) {
        if (handles[handleId]->isLocked()) {
            return false;
        }
    }

    return true;
}
