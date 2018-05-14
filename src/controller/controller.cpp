#include <goliath/core.h>
#include <goliath/vision.h>
#include <goliath/io.h>
#include <boost/log/trivial.hpp>

#include "command_map.h"
#include "commands/move_command.h"
#include "command_executor.h"
#include "commands/follow_line_command.h"
#include "commands/move_tower_command.h"
#include "handles.h"

/**
 * @file controller.cpp
 * @brief Application entry point
 * @author Group 7 - Informatica
 */

/**
* @namespace goliath
* @brief Main project namespace
*/
using namespace goliath;

int main(int argc, char *argv[]) {
    BOOST_LOG_TRIVIAL(info) << "Controller is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    io::zmq_subscriber subscriber(context, "localhost", 5555);
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    io::zmq_publisher publisher(context, "localhost", 5556);

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    repositories::watcher watcher(500, publisher);
    auto battery_repo = std::make_shared<repositories::battery_repository>();
    watcher.watch(battery_repo);

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::handle_map handle_map;
    handle_map.add(HANDLE_LEFT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle(0)));
    handle_map.add(HANDLE_RIGHT_EYE_CAM, std::make_shared<handles::webcam_handle>(handles::webcam_handle(0)));

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::command_map command_map;
    command_map.add(CommandMessage::kMoveCommand, std::make_shared<commands::move_command>(commands::move_command()));
    command_map.add(
            CommandMessage::kFollowLineCommand,
            std::make_shared<commands::follow_line_command>(commands::follow_line_command())
    );
    command_map.add(
            CommandMessage::kMoveTowerCommand,
            std::make_shared<commands::move_tower_command>(commands::move_tower_command())
    );

    commands::command_executor runner(command_map, handle_map);

    subscriber.bind(MessageCarrier::MessageCase::kCommandMessage,
                     [&runner](const MessageCarrier &carrier) {
                         CommandMessage message = carrier.commandmessage();
                         runner.run(message.command_case(), message);
                     });

    BOOST_LOG_TRIVIAL(info) << "Launching subscriber";
    subscriber.start();
    BOOST_LOG_TRIVIAL(info) << "Starting watcher";
    watcher.start();

    BOOST_LOG_TRIVIAL(info) << "Press any key to stop the controller";
    getchar();

    BOOST_LOG_TRIVIAL(info) << "Stopping watcher";
    watcher.stop();

    BOOST_LOG_TRIVIAL(info) << "Stopping subscriber";
    subscriber.stop();

    BOOST_LOG_TRIVIAL(info) << "Controller has been stopped";

    return 0;
}
