#include "move_command.h"

using namespace goliath::commands;
using namespace goliath::core;

move_command::move_command(const std::shared_ptr<movement_service> &movement_interface)
    : _movement_interface(movement_interface) {
    this->_movement_interface = movement_interface;
}

void move_command::execute() {
    _movement_interface->move();
}
