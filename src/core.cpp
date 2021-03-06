#include <boost/log/trivial.hpp>

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
    auto configRepository = std::make_shared<repositories::ConfigRepository>(configFile);
    auto config = configRepository->getConfig();

    util::Console console(&util::colorConsoleFormatter,
                          argv[0],
                          "core-text.txt",
                          static_cast<boost::log::trivial::severity_level>(config->logging().severity_level()));

    if (geteuid() != 0) {
        BOOST_LOG_TRIVIAL(error) << "Root privileges needed.";
        return 1;
    }

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

    BOOST_LOG_TRIVIAL(info) << "Setting up GPIO";
    gpio::GPIO gpio(static_cast<gpio::GPIO::MapPin>(config->gpio().pin()), gpio::GPIO::Direction::Out,
                    gpio::GPIO::State::Low);

    BOOST_LOG_TRIVIAL(info) << "Setting up serial port";
    std::string portName = config->serial().port();
    ulong baudRate = config->serial().baudrate();
    auto port = std::make_shared<dynamixel::SerialPort>();
    bool connectSuccess = port->connect(portName, static_cast<uint32_t>(baudRate));

    BOOST_LOG_TRIVIAL(info) << "Setting up handles";
    handles::HandleMap handles;

    handles.add<handles::WebcamHandle>(HANDLE_CAM, 0);
    std::shared_ptr<repositories::WingStateRepository> wingStateRepository = nullptr;

    auto callback = [&gpio](bool isTx) {
        gpio.set(isTx ? gpio::GPIO::State::High : gpio::GPIO::State::Low);
    };
    if (connectSuccess) {
        BOOST_LOG_TRIVIAL(info) << "Setting up Dynamixel servo handles";

        std::vector<size_t> wingHandleIds;
        for (const proto::repositories::Wing &wing : config->servos().wings()) {
            auto dynamixel = std::make_shared<dynamixel::Dynamixel>(wing.id(), port);
            dynamixel->setDirectionCallback(callback);

            size_t handleId;
            switch (wing.position()) {
                case proto::repositories::Position::LEFT_FRONT:
                    handleId = HANDLE_LEFT_FRONT_WING_SERVO;
                    break;
                case proto::repositories::Position::LEFT_BACK:
                    handleId = HANDLE_LEFT_BACK_WING_SERVO;
                    break;
                case proto::repositories::Position::RIGHT_FRONT:
                    handleId = HANDLE_RIGHT_FRONT_WING_SERVO;
                    break;
                case proto::repositories::Position::RIGHT_BACK:
                    handleId = HANDLE_RIGHT_BACK_WING_SERVO;
                    break;
                default:
                    throw std::runtime_error("Wing position could not be handled");
            }

            auto handle = handles.add<handles::WingHandle>(handleId,
                                                           dynamixel,
                                                           wing.number_of_sectors(),
                                                           wing.mirror(),
                                                           wing.mirror(),
                                                           0.0
            );
            wingHandleIds.emplace_back(handle->getId());
        }

        wingStateRepository = std::make_shared<repositories::WingStateRepository>(wingHandleIds);
    }

    auto servo_arm = std::make_shared<dynamixel::Dynamixel>(5, port);
    servo_arm->setDirectionCallback(callback);
    handles.add<handles::ServoHandle>(HANDLE_BASE_ARM, servo_arm);
    auto servo_first_joint = std::make_shared<dynamixel::Dynamixel>(6, port);
    servo_first_joint->setDirectionCallback(callback);
    handles.add<handles::ServoHandle>(HANDLE_FIRST_JOINT_ARM, servo_first_joint);
    auto servo_second_joint = std::make_shared<dynamixel::Dynamixel>(7, port);
    servo_second_joint->setDirectionCallback(callback);
    handles.add<handles::ServoHandle>(HANDLE_SECOND_JOINT_ARM, servo_second_joint);
    auto servo_gripper = std::make_shared<dynamixel::Dynamixel>(8, port);
    servo_gripper->setDirectionCallback(callback);
    handles.add<handles::ServoHandle>(HANDLE_GRIPPER_ARM, servo_gripper);

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
                throw std::runtime_error("Unknown motor position");
        }
        handles.add<handles::MotorHandle>(handle, motor.id());
    }

    handles.add<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER, 0x40);
    handles.add<handles::EmotionHandle>(HANDLE_EMOTIONS, emotionRepository);

    gpio::GPIO fanGpio(static_cast<gpio::GPIO::MapPin>(config->gpio().pin()), gpio::GPIO::Direction::Out, gpio::GPIO::State::High);
    handles.add<handles::GPIOHandle>(HANDLE_GPIO_PIN_FAN, std::make_shared<gpio::GPIO>(fanGpio));

    BOOST_LOG_TRIVIAL(info) << "Setting up commands";
    commands::CommandMap commands(commandStatusRepository);

    auto runner = std::make_shared<commands::CommandExecutor>(config->command_executor().number_of_executors(),
                                                              commands, handles);
    BOOST_LOG_TRIVIAL(info) << "Setting up watcher";
    auto watcher = std::make_shared<repositories::Watcher>(config->watcher().polling_rate(), publisher, runner);

    BOOST_LOG_TRIVIAL(info) << "Watching repositories";
    watcher->watch(batteryRepository);
    watcher->watch(commandStatusRepository);
    watcher->watch(emotionRepository);
    watcher->watch(loggingRepository);
    watcher->watch(configRepository);
    watcher->watch(systemStatusRepository);

    subscriber.bind(proto::MessageCarrier::MessageCase::kCommandMessage,
                    [&runner](const proto::MessageCarrier &carrier) {
                        const proto::CommandMessage &message = carrier.commandmessage();
                        runner->run(message.command_case(), message);
                    });

    commands.add<commands::InvalidateAllCommand>(proto::CommandMessage::kInvalidateAllCommand, watcher);
    commands.add<commands::InterruptCommandCommand>(
            proto::CommandMessage::kInterruptCommandCommand,
            std::make_shared<commands::CommandMap>(commands));
    commands.add<commands::ShutdownCommand>(proto::CommandMessage::kShutdownCommand, ioService);
    commands.add<commands::SynchronizeCommandsCommand>(
            proto::CommandMessage::kSynchronizeCommandsCommand,
            commandStatusRepository);
    commands.add<commands::MoveCommand>(proto::CommandMessage::kMoveCommand);
    commands.add<commands::MoveWingCommand>(proto::CommandMessage::kMoveWingCommand, wingStateRepository);
    commands.add<commands::SetWingPositionCommand>(proto::CommandMessage::kSetWingPositionCommand, wingStateRepository);
    commands.add<commands::SynchronizeSystemStatusCommand>(proto::CommandMessage::kSynchronizeSystemStatusCommand,
                                                           systemStatusRepository,
                                                           config->fan().start_threshold(),
                                                           config->fan().stop_threshold());
    commands.add<commands::SynchronizeBatteryVoltageCommand>(proto::CommandMessage::kSynchronizeBatteryVoltageCommand,
                                                             batteryRepository);
    commands.add<commands::MoveArmCommand>(proto::CommandMessage::kMoveArmCommand);
    commands.add<commands::GripCommand>(proto::CommandMessage::kGripCommand);

    // Part 1: Entering the Arena
    commands.add<commands::EnterCommand>(proto::CommandMessage::kEnterCommand);

    // Part 2: So you think you can Dance!?
    commands.add<commands::DanceCommand>(proto::CommandMessage::kDanceCommand, wingStateRepository,
                                         config->dance().chainsaw_speed_low(),
                                         config->dance().chainsaw_speed_medium(),
                                         config->dance().chainsaw_speed_high(),
                                         config->dance().chainsaw_speed_extra_high());

    // Part 3: "Line" Dance
    gpio::GPIO gpio5(gpio::GPIO::MapPin::GPIO5, gpio::GPIO::Direction::In);
    // TODO: Fix hardcoded GPIO pin.
    handles.add<handles::GPIOHandle>(HANDLE_GPIO_PIN_5, std::make_shared<gpio::GPIO>(gpio5));
    commands.add<commands::LineDanceCommand>(proto::CommandMessage::kLineDanceCommand, wingStateRepository);

    // Part 4: Obstacle Course
    commands.add<commands::ObstacleCourseCommand>(proto::CommandMessage::kObstacleCourseCommand, wingStateRepository);

    // Part 5: Des Knaben Wunderhorn
    commands.add<commands::WunderhornCommand>(proto::CommandMessage::kWunderhornCommand);

    // Part 6: Transport and Rebuild
    commands.add<commands::TransportRebuildCommand>(proto::CommandMessage::kTransportRebuildCommand);

    commands.add<commands::InterruptCommandCommand>(
        proto::CommandMessage::kInterruptCommandCommand,
        std::make_shared<commands::CommandMap>(commands));

    BOOST_LOG_TRIVIAL(info) << "Launching subscriber";
    subscriber.start();

    BOOST_LOG_TRIVIAL(info) << "Starting watcher";
    watcher->start();

    BOOST_LOG_TRIVIAL(info) << "Press CTR+C to stop the core";

    ioService->run();

    BOOST_LOG_TRIVIAL(warning) << "Core is shutting down...";
    for (auto &command : commands) {
        command.second.instance->interrupt();
    }

    BOOST_LOG_TRIVIAL(info) << "Stopping watcher";
    watcher->stop();

    BOOST_LOG_TRIVIAL(info) << "Stopping subscriber";
    subscriber.stop();

    BOOST_LOG_TRIVIAL(fatal) << "Core has been shut down";

    return 0;
}
