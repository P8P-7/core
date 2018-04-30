#include "command_runner.h"

using namespace goliath::commands;

void command_runner::run(const unsigned command_id) const {
    std::shared_ptr<Command> instance;

    auto instance_ptr = cm.lock()->get_instance(command_id);

    if(auto command_ptr = instance_ptr.lock())
        instance = command_ptr;
    else
        return;

    while(true) {
        if(can_start(*instance)) {
            cm.lock()->set_command_status(command_id, command_status::RUNNING);
            instance->execute();
            cm.lock()->set_command_status(command_id, command_status::STALE);
        }
    }
}

bool command_runner::can_start(const Command& command) const {
    bool start = true;

    for(const unsigned handle : command.get_handles())
        if(hm.lock()->get(handle)->get_status())
            start = false;

    return start;
}
