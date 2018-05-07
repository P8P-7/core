#include "commands/command_map.h"
#include "commands/move_command.h"
#include "command_runner.h"

using namespace goliath;

/**
 * @file controller.cpp
 * @brief Application entry point
 * @author Group 7 - Informatica
 */

/**
 * @namespace goliath
 * @brief Main project namespace
 */

int main(int argc, char *argv[]) {
    const size_t test_id = 42;

    commands::command_map command_map;
    command_map.add(test_id, std::make_shared<commands::move_command>(commands::move_command()));

    commands::command_runner runner(std::make_shared<commands::command_map>(command_map));

    return 0;
}
