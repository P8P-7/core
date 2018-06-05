#include <boost/log/trivial.hpp>
#include <memory>

#include <goliath/foundation.h>
#include <goliath/gpio.h>
#include <goliath/servo.h>
#include <goliath/vision.h>
#include <goliath/zmq-messaging.h>
#include <goliath/emotions.h>
#include <goliath/i2c.h>
#include <goliath/motor-controller.h>
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
    std::string configFile = util::FoundationUtilities::executableToFile(argv[0], "config/core-config.json");
    repositories::ConfigRepository configRepository(configFile);
    std::shared_ptr<proto::repositories::ConfigRepository> config = configRepository.getConfig();

    util::Console console(&util::colorConsoleFormatter,
                          argv[0],
                          "core-text.txt",
                          static_cast<boost::log::trivial::severity_level>(config->logging().severity_level()));

    auto emotionRepository = std::make_shared<repositories::EmotionRepository>();
    auto loggingRepository = std::make_shared<repositories::LogRepository>(config->logging().history_size());

    boost::asio::io_service ioService;

    boost::asio::signal_set signals(ioService, SIGINT, SIGTERM);
    signals.async_wait([&ioService](const boost::system::error_code &errorCode, int signalNumber) {
        BOOST_LOG_TRIVIAL(info) << "Got signal " << signalNumber << " stopping io_service.";
        ioService.stop();
    });

    BOOST_LOG_TRIVIAL(info) << "Core is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    messaging::ZmqSubscriber subscriber(context, "localhost", config->zmq().subscriber_port());
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    messaging::ZmqPublisher publisher(context, "localhost", config->zmq().publisher_port());
    BOOST_LOG_TRIVIAL(info) << "Setting up emotion publisher";
    emotions::EmotionPublisher emotionPublisher(context, config->emotions().host(), config->emotions().port(),
                                                emotionRepository);

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    auto watcher = std::make_shared<repositories::Watcher>(config->watcher().polling_rate(), publisher);
    auto batteryRepo = std::make_shared<repositories::BatteryRepository>();
    watcher->watch(batteryRepo);
    watcher->watch(emotionRepository);
    watcher->watch(loggingRepository);

    BOOST_LOG_TRIVIAL(info) << "Setting up GPIO";
    gpio::GPIO gpio(static_cast<gpio::GPIO::MapPin>(config->gpio().pin()), gpio::GPIO::Direction::Out,
                    gpio::GPIO::State::Low);
    std::function<void(bool)> callback = [&gpio](bool isTx) {
        if (isTx) {
            gpio.set(gpio::GPIO::State::High);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(20));
            gpio.set(gpio::GPIO::State::Low);
        }
    };

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handles;

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = config->serial().port();
    unsigned int baudRate = config->serial().baudrate();

    SerialPort port;
    bool connectSuccess = port.connect(portName, baudRate) != 0;

    if (!connectSuccess) {
        BOOST_LOG_TRIVIAL(warning) << "Couldn't connect to serial port";
    }

    handles.add<handles::WebcamHandle>(HANDLE_CAM, 0);
    handles.add<handles::EmotionHandle>(HANDLE_EMOTIONS, emotionPublisher);

    if (connectSuccess) {
        BOOST_LOG_TRIVIAL(info) << "Setting up Dynamixel servo handles";

        std::map<std::string, int> servos;

        for (proto::repositories::Wing wing : config->servos().wings()) {
            std::shared_ptr<Dynamixel> dynamixel = std::make_shared<Dynamixel>(wing.id(), port);

            size_t handle;

            if (wing.position() == proto::repositories::Position::LEFT_FRONT) {
                handle = HANDLE_LEFT_FRONT_WING_SERVO;
            } else if (wing.position() == proto::repositories::Position::LEFT_BACK) {
                handle = HANDLE_LEFT_BACK_WING_SERVO;
            } else if (wing.position() == proto::repositories::Position::RIGHT_FRONT) {
                handle = HANDLE_RIGHT_FRONT_WING_SERVO;
            } else if (wing.position() == proto::repositories::Position::RIGHT_BACK) {
                handle = HANDLE_RIGHT_BACK_WING_SERVO;
            } else {
                throw std::runtime_error("Servo position could not be handled");
            }

            handles.add<handles::ServoHandle>(handle, dynamixel, callback);
        }
    }

    handles.add<handles::I2cBusHandle>(HANDLE_I2C_BUS, "/dev/i2c-1");
    handles.add<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER, 0x30);
    handles.add<handles::MotorHandle>(HANDLE_LEFT_FRONT_MOTOR, 0);

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commands;
    commands.add<commands::MoveCommand>(proto::CommandMessage::kMoveCommand);
    commands.add<commands::MoveWingCommand>(proto::CommandMessage::kMoveWingCommand);
    commands.add<commands::WunderhornCommand>(proto::CommandMessage::kWunderhornCommand);
    commands.add<commands::MoveTowerCommand>(proto::CommandMessage::kMoveTowerCommand);
    commands.add<commands::InvalidateAllCommand>(proto::CommandMessage::kInvalidateAllCommand, watcher);

    commands::CommandExecutor runner(config->command_executor().number_of_executors(), commands, handles);

    subscriber.bind(proto::MessageCarrier::MessageCase::kCommandMessage, [&runner](const proto::MessageCarrier &carrier) {
        proto::CommandMessage message = carrier.commandmessage();
        runner.run(message.command_case(), message);
    });

    BOOST_LOG_TRIVIAL(info) << "Launching subscriber";
    subscriber.start();
    BOOST_LOG_TRIVIAL(info) << "Starting watcher";
    watcher->start();

    BOOST_LOG_TRIVIAL(info) << "Press CTR+C to stop the controller";

    ioService.run();

    BOOST_LOG_TRIVIAL(warning) << "Core is shutting down...";

    BOOST_LOG_TRIVIAL(info) << "Stopping watcher";
    watcher->stop();

    BOOST_LOG_TRIVIAL(info) << "Stopping subscriber";
    subscriber.stop();

    BOOST_LOG_TRIVIAL(fatal) << "Core has been shut down";

    return 0;
}
