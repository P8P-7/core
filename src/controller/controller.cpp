#include <goliath/webcam_handle.h>
#include "command_map.h"
#include "commands/move_command.h"
#include "command_executor.h"
#include "handles.h"
#include "commands/follow_line_command.h"
#include "commands.h"
#include "commands/move_tower_command.h"

using namespace goliath;

int main(int argc, char *argv[]) {
    handles::handle_map handle_map;
    handle_map.add(HANDLE_LEFT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle("/dev/null")));
    handle_map.add(HANDLE_RIGHT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle("/dev/null")));

    commands::command_map command_map;
    command_map.add(COMMAND_MOVE, std::make_shared<commands::move_command>(commands::move_command()));
    command_map.add(
            COMMAND_FOLLOW_LINE,
            std::make_shared<commands::follow_line_command>(commands::follow_line_command())
    );
    command_map.add(
            COMMAND_MOVE_TOWER,
            std::make_shared<commands::move_tower_command>(commands::move_tower_command())
    );

    commands::command_executor runner(command_map, handle_map);
    runner.run(COMMAND_MOVE);
    runner.run(COMMAND_MOVE);

    runner.run(COMMAND_FOLLOW_LINE);
    runner.run(COMMAND_MOVE_TOWER);

    return 0;
}
