#include <iostream>
#include <goliath/caterpillar_movement.h>
#include <goliath/io.h>
#include <thread>

#include "commands/move_command.h"

using namespace goliath::core;
using namespace goliath::io;

int main() {
    std::cout << "Started controller!" << std::endl;
    zmq_subscriber sub("localhost", 5555, "test");
    return 0;
}