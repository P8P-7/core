#include <iostream>
#include <goliath/caterpillar_movement.h>
#include <goliath/io.h>

#include "commands/move_command.h"

using namespace goliath::core;

int main() {
    std::cout << "Started controller!" << std::endl;
    goliath::io::zmq_subscriber subscriber("tcp://*:5555", "test");
    return 0;
}