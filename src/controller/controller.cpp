#include <goliath/webcam_handle.h>
#include <goliath/io.h>

#include "command_map.h"
#include "commands/move_command.h"
#include "command_executor.h"
#include "handles.h"
#include "commands/follow_line_command.h"
#include "commands/move_tower_command.h"

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
    handles::handle_map handle_map;
    handle_map.add(HANDLE_LEFT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle("/dev/null")));
    handle_map.add(HANDLE_RIGHT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle("/dev/null")));

    commands::command_map command_map;
    command_map.add(Message::kMoveCommand, std::make_shared<commands::move_command>(commands::move_command()));
    command_map.add(
            Message::kFollowLineCommand,
            std::make_shared<commands::follow_line_command>(commands::follow_line_command())
    );
    command_map.add(
            Message::kMoveTowerCommand,
            std::make_shared<commands::move_tower_command>(commands::move_tower_command())
    );

    commands::command_executor runner(command_map, handle_map);

    zmq::context_t context(1);
    io::zmq_subscriber subscriber(context, "localhost", 5555, "commands", [&runner](size_t command_id, const Message &message){
        runner.run(command_id, message);
    });

    subscriber.receive();

    return 0;
}
