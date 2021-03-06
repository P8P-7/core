#include <goliath/controller/command_executor.h>
#include <goliath/controller/commands/queue_command.h>

#include <boost/range/adaptor/map.hpp>
#include <boost/log/trivial.hpp>
#include <cmath>

using namespace goliath::commands;

CommandExecutor::CommandExecutor(size_t numberOfThreads, CommandMap &commands, HandleMap &handles)
        : numberOfThreads(numberOfThreads), numberOfActiveCommands(0), commands(commands), handles(handles),
          pool(numberOfThreads) {}

CommandExecutor::~CommandExecutor() {
    pool.join();
}

void CommandExecutor::run(const size_t commandId, const proto::CommandMessage &message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!commands.commandExists(commandId)) {
        throw std::runtime_error("Command does not exist"); // TODO: Own exception class?
    }

    auto &item = commands[commandId];
    if (item.status != CommandStatus::STALE) {
        if (!item.instance->canRunParallel()) {
            BOOST_LOG_TRIVIAL(warning) << "Command " << commandId << " was dropped because it was already running: "
                                       << (item.status == CommandStatus::STARTING ? "STARTING" : "STARTED");
            return;
        }

        BOOST_LOG_TRIVIAL(warning) << "Command " << commandId << " was already running, running parallel.";
    }
    item.status = CommandStatus::STARTING;
    commands.getStatusRepository()->updateItem(commandId, item.status);

    if (numberOfActiveCommands + 1 >= std::ceil(numberOfThreads * 0.75)) {
        BOOST_LOG_TRIVIAL(warning) << "Number of active commands is almost exceeding the number of command threads!"
                                   << " Execution of commands may be postponed.";
    }

    std::string commandName = boost::core::demangled_name(BOOST_CORE_TYPEID(*item.instance));
    auto requiredHandles = handles.getHandles(item.instance->getRequiredHandles());
    for (auto &handle : requiredHandles | boost::adaptors::map_values) {
        if (handle->isLocked(commandId)) {
            continue;
        } else if (!handle->isLocked()) {
            handle->lock(commandId);
        } else {
            BOOST_LOG_TRIVIAL(warning) << "Execution of \"Command " << commandName << "\" is delayed by handle " << std::to_string(handle->getId());

            size_t lockerId = handle->getOwnerId();
            commands[lockerId].instance->interrupt();
            handle->waitAndLock(commandId);
        }
    }

    boost::asio::post(pool, std::bind(&CommandExecutor::start, this, commandId, message));
}

void CommandExecutor::run(size_t commandId) {
    run(commandId, proto::CommandMessage());
}

void CommandExecutor::start(const size_t &commandId, const proto::CommandMessage &message) {
    std::unique_lock<std::mutex> lock(mutex);
    numberOfActiveCommands++;
    CommandItem &item = commands[commandId];
    item.status = CommandStatus::STARTED;
    commands.getStatusRepository()->updateItem(commandId, item.status);

    std::string commandName = boost::core::demangled_name(BOOST_CORE_TYPEID(*item.instance));
    lock.unlock();

    BOOST_LOG_TRIVIAL(info) << "Execution of \"Command " << commandName << "\" has started";
    try {
        auto requiredHandles = handles.getHandles(item.instance->getRequiredHandles());
        item.instance->execute(requiredHandles, message);
    } catch (std::exception &ex) {
        BOOST_LOG_TRIVIAL(error) << "Error executing command \"" << commandId << "\" what(): " << ex.what();
    }
    BOOST_LOG_TRIVIAL(info) << "Execution of \"Command " << commandName << "\" has finished";

    lock.lock();
    item.status = CommandStatus::STALE;
    commands.getStatusRepository()->updateItem(commandId, item.status);

    numberOfActiveCommands--;
}
