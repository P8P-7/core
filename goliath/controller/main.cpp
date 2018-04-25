#include <iostream>
#include <vector>

#include <goliath/caterpillar_movement.h>
#include <goliath/io.h>

#include "commands/move_command.h"
#include "handles/handle_map.h"

using namespace goliath::core;
using namespace goliath::handles;
using namespace goliath::caterpillar_movement;

int main() {
    std::condition_variable var;
    handle_map map(var);
    return 0;
}