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

struct GPIOTranslator {
    typedef std::string internal_type;
    typedef gpio::GPIO::MapPin external_type;

    boost::optional<external_type> get_value(internal_type const &v) {
        int gpioVal = std::stoi(v);
        if (gpioVal < 2 || gpioVal > 27) {
            return boost::none;
        }

        return static_cast<external_type>(gpioVal);
    }

    boost::optional<internal_type> put_value(external_type const &v) {
        return std::to_string(static_cast<int>(v));
    }
};

static GPIOTranslator gpioTrans;

/**
 * @fn main(int argc, char *argv[])
 * @brief Application entry point
 */
int main(int argc, char *argv[]) {
    util::Console console(&util::colorConsoleFormatter, argv[0], "core-text.txt");

    std::string configFile = util::FoundationUtilities::executableToFile(argv[0], "config/core-config.json");
    repositories::ConfigRepository configRepository(configFile);

    std::shared_ptr<::ConfigRepository> config = configRepository.getConfig();

    boost::asio::io_service ioService;

    boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
    signals.async_wait([&ioService](const boost::system::error_code &errorCode, int signalNumber) {
        BOOST_LOG_TRIVIAL(info) << "Got signal " << signalNumber << " stopping io_service.";
        ioService.stop();
    });

    BOOST_LOG_TRIVIAL(info) << "Controller is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    messaging::ZmqSubscriber subscriber(context, "localhost", config->zmq().subscriber_port());
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    messaging::ZmqPublisher publisher(context, "localhost", config->zmq().publisher_port());

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    repositories::Watcher watcher(500, publisher);
    auto battery_repo = std::make_shared<repositories::BatteryRepository>();
    watcher.watch(battery_repo);

    BOOST_LOG_TRIVIAL(info) << "Setting up GPIO";
    gpio::GPIO gpio(static_cast<gpio::GPIO::MapPin>(config->gpio().pin()), gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
    std::function<void(bool)> callback = [&gpio](bool isTx) {
        if (isTx) {
            gpio.set(gpio::GPIO::State::High);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(20));
            gpio.set(gpio::GPIO::State::Low);
        }
    };

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = config->serial().port();
    unsigned int baudRate = config->serial().baudrate();

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

        std::map<std::string, int> servos;

        for(Wing wing : config->servos().wings()) {
            std::shared_ptr<Dynamixel> dynamixel = std::make_shared<Dynamixel>(wing.id(), port);

            int handle;

            if (wing.position() == Position::LEFT_FRONT) {
                handle = HANDLE_LEFT_FRONT_WING_SERVO;
            } else if (wing.position() == Position::LEFT_BACK) {
                handle = HANDLE_LEFT_BACK_WING_SERVO;
            } else if (wing.position() == Position::RIGHT_FRONT) {
                handle = HANDLE_RIGHT_FRONT_WING_SERVO;
            } else if (wing.position() == Position::RIGHT_BACK) {
                handle = HANDLE_RIGHT_BACK_WING_SERVO;
            } else {
                throw std::runtime_error("Servo position could not be handled");
            }

            handles.add<handles::ServoHandle>(handle, dynamixel, callback);
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commands;
    commands.add<commands::MoveCommand>(CommandMessage::kMoveCommand);
    commands.add<commands::MoveWingCommand>(CommandMessage::kMoveWingCommand);
    commands.add<commands::WunderhornCommand>(CommandMessage::kWunderhornCommand);
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
