#include "command_runner.h"

using namespace goliath::commands;

void command_runner::run(const unsigned command_id) {
    std::shared_ptr<Command> instance;

    auto instance_ptr = cm.lock()->get_instance(command_id);

    if(auto command_ptr = instance_ptr.lock())
        instance = command_ptr;
    else
        return;

    std::thread(&command_runner::execute, this, std::ref(command_id), instance);
}

void command_runner::execute(const unsigned &command_id, std::shared_ptr<Command> instance) {
    thread_count++;

    while(true) {
        if(can_start(*instance)) {
            cm.lock()->set_command_status(command_id, command_status::RUNNING);
            instance->execute();
            cm.lock()->set_command_status(command_id, command_status::STALE);
            break;
        }
        else {
            for(unsigned handle : instance->get_handles()) {
                unsigned locker = hm.get(handle)->get_locker();
                cm.lock()->get_instance(locker).lock()->interrupt();
            }
        }

        std::unique_lock<std::mutex> lock(mutex);
        var->wait(lock);
    }

    thread_count--;
}

bool command_runner::can_start(const Command& command) const {
    bool start = true;

    for(const unsigned handle : command.get_handles())
        if(hm.get(handle)->get_status())
            start = false;

    return start;
}
