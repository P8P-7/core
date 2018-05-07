#include <boost/log/trivial.hpp>
#include "command_executor.h"

using namespace goliath::commands;

void command_executor::run(const size_t command_id) {
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
    threads.emplace_back(std::thread(&command_executor::try_execute, this, command_id));
}

void command_executor::try_execute(const size_t &command_id) {
    std::unique_lock<std::mutex> lock(mutex);

    command_item& item = commands[command_id];
    if(can_start(*(item.instance))) {
        lock.unlock();
        item.instance->execute();
        lock.lock();
        item.status = command_status::STALE;
        return;
    }

    lock.unlock();
    for(size_t handle_id : item.instance->get_handles()) {
        size_t locker_id = handles[handle_id]->get_locker();
        commands[locker_id].instance->interrupt();
    }

    for(size_t handle_id : item.instance->get_handles()) {
        handles[handle_id]->wait();
        handles[handle_id]->lock();
    }

    try_execute(command_id);
}

bool command_executor::can_start(const command& command) const {
    for(const size_t handle_id : command.get_handles()) {
        if (handles.get(handle_id)->is_locked()) {
            return false;
        }
    }

    return true;
}

void command_executor::wait_for_completion() {
    for (auto &thread : threads) {
        thread.join();
    }
}
