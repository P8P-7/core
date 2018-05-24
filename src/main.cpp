#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

#include <goliath/foundation.h>
#include <goliath/gpio.h>
#include <goliath/servo.h>
#include <goliath/vision.h>
#include <goliath/zmq_messaging.h>
#include <goliath/controller.h>

/**
 * @file main.cpp
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
    util::Console console(&util::colorConsoleFormatter, "core-text.txt");

    boost::property_tree::ptree root;
    boost::property_tree::read_json("config/core-config.json", root);

    boost::asio::io_service ioService;

    boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
    signals.async_wait([&ioService](const boost::system::error_code &errorCode, int signalNumber) {
        BOOST_LOG_TRIVIAL(info) << "Got signal " << signalNumber << " stopping io_service.";
        ioService.stop();
    });

    BOOST_LOG_TRIVIAL(info) << "Controller is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    messaging::ZmqSubscriber subscriber(context, "localhost", root.get<int>("zmq.subscribe_port"));
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    messaging::ZmqPublisher publisher(context, "localhost", root.get<int>("zmq.publish_port"));

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    repositories::Watcher watcher(500, publisher);
    auto battery_repo = std::make_shared<repositories::BatteryRepository>();
    watcher.watch(battery_repo);

    BOOST_LOG_TRIVIAL(info) << "Setting up GPIO";
    gpio::GPIO gpio;
    gpio.setup(root.get<int>("gpio.pin"), OUT, LOW);
    std::function<void(bool)> callback = [&gpio](bool isTx) {
        if (isTx) {
            gpio.set(HIGH);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(20));
            gpio.set(LOW);
        }
    };

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = root.get<std::string>("serial.port");
    unsigned int baudRate = root.get<int>("serial.baud_rate");

    SerialPort port;
    bool connectSuccess = port.connect(portName, baudRate) != 0;

    if (!connectSuccess) {
        BOOST_LOG_TRIVIAL(warning) << "Couldn't connect to serial port";
    }

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handles;
    handles.add<handles::WebcamHandle>(HANDLE_CAM, root.get<int>("vision.camera"));

    if (true) {
        BOOST_LOG_TRIVIAL(info) << "Setting up Dynamixel servo handles";

        std::map<std::string, int> servos;

        for(boost::property_tree::ptree::value_type &servo : root.get_child("servos")) {
            servos[servo.second.get<std::string>("position")] = servo.second.get<int>("id");
        }

        for(auto const &servo : servos) {
            std::shared_ptr<Dynamixel> dynamixel = std::make_shared<Dynamixel>(servo.second, port);

            std::cout << servo.second << " " << servo.first << std::endl;

            int handle;

            if(servo.first == "left_front") {
                handle = HANDLE_LEFT_FRONT_WING_SERVO;
            } else if(servo.first == "left_back") {
                handle = HANDLE_LEFT_BACK_WING_SERVO;
            } else if(servo.first == "right_front") {
                handle = HANDLE_RIGHT_FRONT_WING_SERVO;
            } else if(servo.first == "right_back") {
                handle = HANDLE_RIGHT_BACK_WING_SERVO;
            }

            handles.add<handles::ServoHandle>(handle, dynamixel, callback);
        }
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

    return 0;
}
