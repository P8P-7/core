#include "command_runner.h"

using namespace goliath::commands;

void command_runner::run(const unsigned command_id, Message &message) {
    auto instance = commands->get_instance(command_id);
    if (instance == nullptr) {
        throw std::runtime_error("Command does not exist"); // TODO: Own exception class?
    }

    std::thread(&command_runner::execute, this, std::ref(command_id), instance, std::ref(message));
}

void command_runner::execute(const unsigned &command_id, std::shared_ptr<command> instance, Message &message) {
    thread_count++;

    while(true) {
        if(can_start(*instance)) {
            commands->set_command_status(command_id, command_status::RUNNING);
            instance->execute(message);
            commands->set_command_status(command_id, command_status::STALE);
            break;
        }

        for(size_t handle_id : instance->get_handles()) {
            size_t locker = handles[handle_id]->get_locker();
            commands->get_instance(locker)->interrupt();
        }

    }

    for(unsigned handle : instance->get_handles()) {
        handles[handle]->wait();
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
