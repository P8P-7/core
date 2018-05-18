#include <goliath/foundation.h>
#include <goliath/vision.h>
#include <goliath/zmq_messaging.h>
#include <boost/log/trivial.hpp>

#include "command_map.h"
#include "commands/move_command.h"
#include "command_executor.h"
#include "commands/follow_line_command.h"
#include "commands/move_tower_command.h"
#include "util/colored_console.h"
#include "handles.h"

/**
 * @file controller.cpp
 * @author Group 7 - Informatica
 */

/**
* @namespace goliath
* @brief Main project namespace
*/
using namespace goliath;

/**
 * @fn main(int argc, char *argv[])
 * @brief Application entry point
 */
int main(int argc, char *argv[]) {
    goliath::util::init();

    BOOST_LOG_TRIVIAL(info) << "Controller is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    messaging::ZmqSubscriber subscriber(context, "localhost", 5555);
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    messaging::ZmqPublisher publisher(context, "localhost", 5556);

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    repositories::Watcher watcher(500, publisher);
    auto battery_repo = std::make_shared<repositories::BatteryRepository>();
    watcher.watch(battery_repo);

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handleMap;
    handleMap.add(HANDLE_LEFT_EYE_CAM, std::make_shared<handles::WebcamHandle>(handles::WebcamHandle(0)));
    handleMap.add(HANDLE_RIGHT_EYE_CAM, std::make_shared<handles::WebcamHandle>(handles::WebcamHandle(0)));

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commandMap;
    commandMap.add(CommandMessage::kMoveCommand, std::make_shared<commands::MoveCommand>(commands::MoveCommand()));
    commandMap.add(
            CommandMessage::kFollowLineCommand,
            std::make_shared<commands::FollowLineCommand>(commands::FollowLineCommand())
    );
    commandMap.add(
            CommandMessage::kMoveTowerCommand,
            std::make_shared<commands::MoveTowerCommand>(commands::MoveTowerCommand())
    );

    commands::CommandExecutor runner(commandMap, handleMap);

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
