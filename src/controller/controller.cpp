#include <goliath/foundation.h>
#include <goliath/gpio.h>
#include <goliath/servo.h>
#include <goliath/vision.h>
#include <goliath/zmq_messaging.h>
#include <boost/log/trivial.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_service.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "command_map.h"
#include "command_executor.h"
#include "commands/move_command.h"
#include "commands/move_wing_command.h"
#include "commands/follow_line_command.h"
#include "commands/move_tower_command.h"

#include "handles.h"
#include "dynamixel/Dynamixel.h"
#include "util/colored_console.h"

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
 * @fn displayLogo(std::string textFile)
 * @brief Display ASCII logo
 */
void displayLogo(std::string textFile) {
    std::string line;
    std::ifstream logo("logo.txt");
    std::string line2;
    std::ifstream text;

    bool hasText = !textFile.empty();
    if (hasText) {
        text.open(textFile);
        hasText = text.is_open();
    }

    if (logo.is_open()) {
        while (getline(logo, line)) {
            if (hasText && getline(text, line2)) {
                std::cout << line << line2 << std::endl;
            } else {
                std::cout << line << std::endl;
            }
        }
        logo.close();
        if (hasText) {
            text.close();
        }
    }
}

/**
 * @fn main(int argc, char *argv[])
 * @brief Application entry point
 */
int main(int argc, char *argv[]) {
    displayLogo("core-text.txt");

    goliath::util::init();

    boost::asio::io_service ioService;

    boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
    signals.async_wait([&ioService](const boost::system::error_code &errorCode, int signalNumber) {
        BOOST_LOG_TRIVIAL(info) << "Got signal " << signalNumber << " stopping io_service.";
        ioService.stop();
    });

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

    BOOST_LOG_TRIVIAL(info) << "Setting up GPIO";
    gpio::GPIO gpio;
    gpio.setup(GPIO18, OUT, LOW);
    std::function<void(bool)> callback = [&gpio](bool isTx) {
        if (isTx) {
            gpio.set(HIGH);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(20));
            gpio.set(LOW);
        }
    };

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = "/dev/serial0";
    unsigned int baudRate = 1000000;

    SerialPort port;
    bool connectSuccess = port.connect(portName, baudRate) != 0;

    if (!connectSuccess) {
        BOOST_LOG_TRIVIAL(warning) << "Couldn't connect to serial port";
    }

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handles;
    handles.add<handles::WebcamHandle>(HANDLE_CAM, 0);
    if (connectSuccess) {
        BOOST_LOG_TRIVIAL(info) << "Setting up Dynamixel servo handles";
        Dynamixel motor1(1, &port);
        Dynamixel motor2(2, &port);
        Dynamixel motor3(3, &port);
        Dynamixel motor4(4, &port);

        handles.add<handles::ServoHandle>(HANDLE_LEFT_FRONT_WING_SERVO, motor1, callback);
        handles.add<handles::ServoHandle>(HANDLE_LEFT_BACK_WING_SERVO, motor2, callback);
        handles.add<handles::ServoHandle>(HANDLE_RIGHT_FRONT_WING_SERVO, motor3, callback);
        handles.add<handles::ServoHandle>(HANDLE_RIGHT_BACK_WING_SERVO, motor4, callback);
    }

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commands;
    commands.add<commands::MoveCommand>(CommandMessage::kMoveCommand);
    commands.add<commands::MoveWingCommand>(CommandMessage::kMoveWingCommand);
    commands.add<commands::FollowLineCommand>(CommandMessage::kFollowLineCommand);
    commands.add<commands::MoveTowerCommand>(CommandMessage::kMoveTowerCommand);

    commands::CommandExecutor runner(commands, handles);

    subscriber.bind(MessageCarrier::MessageCase::kCommandMessage, [&runner](const MessageCarrier &carrier) {
        CommandMessage message = carrier.commandmessage();
        runner.run(message.command_case(), message);
    });

    BOOST_LOG_TRIVIAL(info) << "Launching subscriber";
    subscriber.start();
    BOOST_LOG_TRIVIAL(info) << "Starting watcher";
    watcher.start();

    BOOST_LOG_TRIVIAL(info) << "Press CTR+C to stop the controller";

    ioService.run();

    BOOST_LOG_TRIVIAL(warning) << "Controller is shutting down...";

    BOOST_LOG_TRIVIAL(info) << "Stopping watcher";
    watcher.stop();

    BOOST_LOG_TRIVIAL(info) << "Stopping subscriber";
    subscriber.stop();

    BOOST_LOG_TRIVIAL(fatal) << "Controller has been shut down";

    return 0;
}
