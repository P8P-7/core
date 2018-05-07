#include <boost/log/trivial.hpp>
#include "command_executor.h"

using namespace goliath::commands;

command_executor::command_executor(command_map& commands, handle_map &handles)
    : commands(commands), handles(handles) {}

command_executor::~command_executor() {
    for (auto &thread : threads) {
        thread.join();
    }
}

void command_executor::run(const size_t command_id, const Message &message) {
    std::lock_guard<std::mutex> lock_guard(mutex);

    if (!commands.command_exists(command_id)) {
        throw std::runtime_error("Command does not exist"); // TODO: Own exception class?
    }

    auto& item = commands[command_id];
    if (item.status != command_status::STALE) {
        BOOST_LOG_TRIVIAL(warning) << "Command "
                                   << command_id
                                   << " has been dropped because was it was already running";
        return;
    }
    item.status = command_status::RUNNING;

    threads.emplace_back(std::thread(&command_executor::try_execute, this, command_id, std::ref(message)));
}

void command_executor::try_execute(const size_t &command_id, const Message &message) {
    std::unique_lock<std::mutex> lock(mutex);

    command_item& item = commands[command_id];
    auto required_handles = handles.get_handles(item.instance->get_required_handles());
    if(can_start(*(item.instance))) {
        lock.unlock();

        for(size_t handle_id : item.instance->get_required_handles()) {
            required_handles[handle_id]->lock(command_id);
        }
        item.instance->execute(required_handles, message);

        lock.lock();
        item.status = command_status::STALE;
        return;
    }

    lock.unlock();
    for(size_t handle_id : item.instance->get_required_handles()) {
        size_t locker_id = handles[handle_id]->get_owner_id();
        commands[locker_id].instance->interrupt();
    }

    for(size_t handle_id : item.instance->get_required_handles()) {
        required_handles[handle_id]->wait();
        required_handles[handle_id]->lock(command_id);
    }

    item.instance->execute(required_handles, message);
    item.status = command_status::STALE;
}

bool command_executor::can_start(const command& command) const {
    for(const size_t handle_id : command.get_required_handles()) {
        if (handles[handle_id]->is_locked()) {
            return false;
        }
    }

    return true;
}
