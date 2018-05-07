#include "commands/command_map.h"
#include "commands/move_command.h"
#include "commands/command_executor.h"

using namespace goliath;

int main(int argc, char *argv[]) {
    const size_t test_id = 42;

    commands::command_map command_map;
    command_map.add(test_id, std::make_shared<commands::move_command>(commands::move_command()));
    command_map.add(test_id + 1, std::make_shared<commands::move_command>(commands::move_command()));

    commands::command_executor runner(command_map);
    runner.run(test_id);
    runner.run(test_id);
    runner.run(test_id + 1);

    runner.wait_for_completion();

    return 0;
}
