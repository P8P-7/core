#include "command_runner.h"

using namespace goliath::commands;

void command_runner::run(const unsigned command_id) {
    auto instance = commands->get_instance(command_id);
    if (instance == nullptr) {
        throw std::runtime_error("Command does not exist"); // TODO: Eigen exception classes?
    }

    std::thread(&command_runner::execute, this, std::ref(command_id), instance);
}

void command_runner::execute(const unsigned &command_id, std::shared_ptr<command> instance) {
    thread_count++;

    while(true) {
        if(can_start(*instance)) {
            commands->set_command_status(command_id, command_status::RUNNING);
            instance->execute();
            commands->set_command_status(command_id, command_status::STALE);
            break;
        }

        for(size_t handle_id : instance->get_handles()) {
            size_t locker = handles[handle_id]->get_locker();
            commands->get_instance(locker)->interrupt();
        }

        std::unique_lock<std::mutex> lock(mutex);
        var->wait(lock);
    }

    thread_count--;
}

bool command_runner::can_start(const command& command) const {
    for(const size_t handle_id : command.get_handles()) {
        if (handles.get(handle_id)->is_locked()) {
            return false;
        }
    }

    return true;
}
