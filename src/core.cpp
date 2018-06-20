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
#include <goliath/controller/repositories/system_status_repository.h>
#include <CommandMessage.pb.h>
#include <repositories/SystemStatusRepository.pb.h>

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
    auto configRepository = std::make_shared<repositories::ConfigRepository>(configFile);
    std::shared_ptr<proto::repositories::ConfigRepository> config = configRepository->getConfig();

    util::Console console(&util::colorConsoleFormatter,
                          argv[0],
                          "core-text.txt",
                          static_cast<boost::log::trivial::severity_level>(config->logging().severity_level()));
    BOOST_LOG_TRIVIAL(info) << "Core is starting";

    BOOST_LOG_TRIVIAL(info) << "Setting up repositories";
    auto commandStatusRepository = std::make_shared<repositories::CommandStatusRepository>();
    auto batteryRepository = std::make_shared<repositories::BatteryRepository>();
    auto emotionRepository = std::make_shared<repositories::EmotionRepository>();
    auto loggingRepository = std::make_shared<repositories::LogRepository>(config->logging().history_size());
    auto systemStatusRepository = std::make_shared<repositories::SystemStatusRepository>();

    auto ioService = std::make_shared<boost::asio::io_service>();

    boost::asio::signal_set signals(*ioService, SIGINT, SIGTERM);
    signals.async_wait([&ioService](const boost::system::error_code &errorCode, int signalNumber) {
        BOOST_LOG_TRIVIAL(info) << "Got signal " << signalNumber << " stopping io_service.";
        ioService->stop();
    });

    BOOST_LOG_TRIVIAL(info) << "Setting up subscriber";
    zmq::context_t context(1);
    messaging::ZmqSubscriber subscriber(context, "localhost", config->zmq().subscriber_port());
    BOOST_LOG_TRIVIAL(info) << "Setting up publisher";
    messaging::ZmqPublisher publisher(context, "localhost", config->zmq().publisher_port());

    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    auto watcher = std::make_shared<repositories::Watcher>(config->watcher().polling_rate(), publisher);

    BOOST_LOG_TRIVIAL(info) << "Watching repositories";
    watcher->watch(batteryRepository);
    auto batteryRepository = std::make_shared<repositories::BatteryRepository>();
    watcher->watch(batteryRepository);
    watcher->watch(commandStatusRepository);
    watcher->watch(emotionRepository);
    watcher->watch(loggingRepository);
    watcher->watch(configRepository);
    watcher->watch(systemStatusRepository);

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

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = config->serial().port();
    ulong baudRate = config->serial().baudrate();
    auto port = std::make_shared<dynamixel::SerialPort>();
    bool connectSuccess = port->connect(portName, static_cast<uint>(baudRate));

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handles;

    handles.add<handles::WebcamHandle>(HANDLE_CAM, 0);

    if (connectSuccess) {
        BOOST_LOG_TRIVIAL(info) << "Setting up Dynamixel servo handles";

        for (const proto::repositories::Wing &wing : config->servos().wings()) {
            auto dynamixel = std::make_shared<dynamixel::Dynamixel>(wing.id(), port);

            size_t handle;
            switch (wing.position()) {
                case proto::repositories::Position::LEFT_FRONT:
                    handle = HANDLE_LEFT_FRONT_WING_SERVO;
                    break;
                case proto::repositories::Position::LEFT_BACK:
                    handle = HANDLE_LEFT_BACK_WING_SERVO;
                    break;
                case proto::repositories::Position::RIGHT_FRONT:
                    handle = HANDLE_RIGHT_FRONT_WING_SERVO;
                    break;
                case proto::repositories::Position::RIGHT_BACK:
                    handle = HANDLE_RIGHT_BACK_WING_SERVO;
                    break;
                default:
                    throw std::runtime_error("Servo position could not be handled");
            }

            handles.add<handles::ServoHandle>(handle, dynamixel, callback);
        }
    }

    handles.add<handles::I2cBusHandle>(HANDLE_I2C_BUS, "/dev/i2c-1");
    handles.add<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER, 0x30);
    const auto &motors = config->motor_controller().motors();
    for (const auto &motor : motors) {
        size_t handle;
        switch (motor.position()) {
            case proto::repositories::Position::LEFT_FRONT:
                handle = HANDLE_LEFT_FRONT_MOTOR;
                break;
            case proto::repositories::Position::LEFT_BACK:
                handle = HANDLE_LEFT_BACK_MOTOR;
                break;
            case proto::repositories::Position::RIGHT_FRONT:
                handle = HANDLE_RIGHT_FRONT_MOTOR;
                break;
            case proto::repositories::Position::RIGHT_BACK:
                handle = HANDLE_RIGHT_BACK_MOTOR;
                break;
            default:
                throw std::runtime_error("Servo position could not be handled");
        }
        handles.add<handles::MotorHandle>(handle, motor.id());
    }

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commands(commandStatusRepository);
    commands.add<commands::InvalidateAllCommand>(proto::CommandMessage::kInvalidateAllCommand, watcher);
    commands.add<commands::InterruptCommandCommand>(
        proto::CommandMessage::kInterruptCommandCommand,
        std::make_shared<commands::CommandMap>(commands));
    commands.add<commands::ShutdownCommand>(proto::CommandMessage::kShutdownCommand, ioService);
    commands.add<commands::SynchronizeCommandsCommand>(
        proto::CommandMessage::kSynchronizeCommandsCommand,
        commandStatusRepository);
    commands.add<commands::MoveCommand>(proto::CommandMessage::kMoveCommand);
    commands.add<commands::MoveWingCommand>(proto::CommandMessage::kMoveWingCommand);
    commands.add<commands::SynchronizeSystemStatusCommand>(
        proto::CommandMessage::kSynchronizeSystemStatusCommand,
        systemStatusRepository);
    commands.add<commands::SynchronizeSystemStatusCommand>(proto::CommandMessage::kSynchronizeSystemStatusCommand,
                                                       systemStatusRepository);
    commands.add<commands::SynchronizeBatteryVoltageCommand>(proto::CommandMessage::kSynchronizeBatteryVoltageCommand,
                                                       batteryRepository);

    // Part 1: Entering the Arena
    commands.add<commands::EnterCommand>(proto::CommandMessage::kEnterCommand);

    // Part 2: So you think you can Dance!?
    commands.add<commands::DanceCommand>(proto::CommandMessage::kDanceCommand);

    // Part 3: "Line" Dance
    commands.add<commands::LineDanceCommand>(proto::CommandMessage::kLineDanceCommand);

    // Part 4: Obstacle Course
    commands.add<commands::ObstacleCourseCommand>(proto::CommandMessage::kObstacleCourseCommand);

    // Part 5: Des Knaben Wunderhorn
    commands.add<commands::WunderhornCommand>(proto::CommandMessage::kWunderhornCommand);

    // Part 6: Transport and Rebuild
    commands.add<commands::TransportRebuildCommand>(proto::CommandMessage::kTransportRebuildCommand);

    commands::CommandExecutor runner(config->command_executor().number_of_executors(), commands, handles);

    BOOST_LOG_TRIVIAL(info) << "Starting default commands";
    runner.run(proto::CommandMessage::kSynchronizeSystemStatusCommand);

    runner.run(proto::CommandMessage::kSynchronizeBatteryVoltageCommand, proto::CommandMessage());

    BOOST_LOG_TRIVIAL(info) << "Launching subscriber";
    subscriber.bind(proto::MessageCarrier::MessageCase::kCommandMessage,
                    [&runner](const proto::MessageCarrier &carrier) {
                        const proto::CommandMessage &message = carrier.commandmessage();
                        runner.run(message.command_case(), message);
                    });
    subscriber.start();

    BOOST_LOG_TRIVIAL(info) << "Starting watcher";
    watcher->start();

    BOOST_LOG_TRIVIAL(info) << "Press CTR+C to stop the controller";

    ioService->run();

    BOOST_LOG_TRIVIAL(warning) << "Core is shutting down...";

    BOOST_LOG_TRIVIAL(info) << "Stopping watcher";
    watcher->stop();

    BOOST_LOG_TRIVIAL(info) << "Stopping subscriber";
    subscriber.stop();

    BOOST_LOG_TRIVIAL(fatal) << "Core has been shut down";

    return 0;
}
