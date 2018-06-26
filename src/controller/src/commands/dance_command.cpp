#include <goliath/controller/commands/dance_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <commands/MoveCommand.pb.h>

#include <goliath/controller/commands/set_wing_position_command.h>
#include <goliath/controller/commands/move_command.h>
#include <goliath/emotions.h>
#include <goliath/led-strip-controller.h>
#include <goliath/transitions.h>
#include <goliath/servo.h>

using namespace std::chrono_literals;
using namespace goliath::handles;
using namespace goliath;

commands::DanceCommand::DanceCommand(const size_t &id,
                                     const std::shared_ptr<repositories::WingStateRepository> &repository,
                                     const std::uint8_t chainSawSpeedLow, const std::uint8_t chainSawSpeedMedium,
                                     const std::uint8_t chainSawSpeedFast, const std::uint8_t chainSawSpeedExtraFast)
        : BasicCommand(id, {HANDLE_EMOTIONS,
                            HANDLE_LED_CONTROLLER,
                            HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                            HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR,
                            HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}),
          repository(repository),
          chainSawSpeedLow(chainSawSpeedLow), chainSawSpeedMedium(chainSawSpeedMedium),
          chainSawSpeedFast(chainSawSpeedFast), chainSawSpeedExtraFast(chainSawSpeedExtraFast) {}

void commands::DanceCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    using Emotion = proto::repositories::EmotionRepository;

    led_controller::LedStatus allColor{
            .lightingType = led_controller::LightingType::ALL,
            .colorType = led_controller::ColorType::HSV
    };

    led_controller::AllLedsMessage allGreen{
            allColor,
            .allLeds = {
                    .hue = 125,
                    .saturation = 100,
                    .value = 100,
                    .rainbow = false
            }
    };

    led_controller::AllLedsMessage allRed{
            allColor,
            .allLeds = {
                    .hue = 0,
                    .saturation = 100,
                    .value = 100,
                    .rainbow = false
            }
    };

    led_controller::AllLedsMessage allBlue{
            allColor,
            .allLeds = {
                    .hue = 236,
                    .saturation = 100,
                    .value = 100,
                    .rainbow = false
            }
    };

    led_controller::AllLedsMessage allYellow{
            allColor,
            .allLeds = {
                    .hue = 59,
                    .saturation = 100,
                    .value = 100,
                    .rainbow = false
            }
    };

    if (repository->hasLostTracking()) {
        BOOST_LOG_TRIVIAL(error) << "Wings not calibrated";
        return;
    }

    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    //led_controller::LedStripController ledController(ledControllerSlave);

    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    std::shared_ptr<repositories::EmotionRepository> emotionRepository = handles.get<handles::EmotionHandle>(
            HANDLE_EMOTIONS)->getEmotionRepository();

    servo::WingController wingController(repository);

    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
    //ledController.sendCommand(allGreen);

    servo::WingCommandBuilder leftFront(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    servo::WingCommandBuilder rightFront(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO));
    servo::WingCommandBuilder leftBack(handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO));
    servo::WingCommandBuilder rightBack(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO));


    using clock = std::chrono::high_resolution_clock;
    std::vector<motor_controller::MotorStatus> motorCommands;
    clock::time_point start = clock::now();

    // Start: Doggy Style
    leftFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({leftFront.build(), rightFront.build()});

    leftBack.setSpeed(1023).setAngle(220).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setSpeed(1023).setAngle(220).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    leftFront.setSpeed(512).setAngle(45).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build()}});
    std::this_thread::sleep_until(start + 7s);

    // Start sawing section 1
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*leftFront.getHandle(), 15 * 4, 256, false);
    std::this_thread::sleep_until(start + 9s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedFast);
    wingController.setAngle(*leftFront.getHandle(), 35 * 4, 756, false);
    std::this_thread::sleep_until(start + 9s + 500ms);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*leftFront.getHandle(), 25 * 4, 512, false);
    std::this_thread::sleep_until(start + 10s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*leftFront.getHandle(), 45 * 4, 1023, false);
    std::this_thread::sleep_until(start + 10s + 250ms);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
    std::this_thread::sleep_until(start + 11s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    std::this_thread::sleep_until(start + 11s + 500ms);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
    std::this_thread::sleep_until(start + 12s);
    wingController.setAngle(*leftFront.getHandle(), 15 * 4, 256, false);
    motorController.sendCommand(motor_controller::MotorStatus{
            .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
            .direction = motor_controller::MotorDirection::FREE,
            .speed = 0
    });
    std::this_thread::sleep_until(start + 13s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 65 * 4, 256, false);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), 0);
    std::this_thread::sleep_until(start + 15s);

    // Reset to base position
    leftFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build()}});

    // Set back wings along body
    leftBack.setSpeed(1023).setAngle(180).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setSpeed(1023).setAngle(180).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    // Set front wings to lower push up position
    leftFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    // Set front wings to upper push up position
    leftFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    // Repeat
    leftFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    leftFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});
    // End repeat

    // Set front wings up
    leftFront.setSpeed(1023).setAngle(75).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(75).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    // Set back wings up
    leftBack.setSpeed(1023).setAngle(75).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setSpeed(1023).setAngle(75).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});
    // All wings are up

    // Drive forwards a bit and then back again
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 128
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_for(3s);

    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
    };
    // Drive backwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_for(6s);

    
}

void commands::DanceCommand::saw(motor_controller::MotorController &motorController, motor_controller::MotorId motor,
                                 std::uint8_t speed) {
    motorController.sendCommand(motor_controller::MotorStatus{
            .id = motor,
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = speed
    });
}

void commands::DanceCommand::moveWingOverTime(const std::chrono::milliseconds &duration,
                                              servo::WingController &wingController,
                                              std::vector<servo::WingCommand> commands) {
    auto start = std::chrono::high_resolution_clock::now();
    wingController.execute(std::move(commands));

    if (std::chrono::high_resolution_clock::now() - start > 0ms) {
        std::this_thread::sleep_for(duration - (std::chrono::high_resolution_clock::now() - start));
    } else {
        throw std::runtime_error("Servo duration too short");
    }
}

motor_controller::MotorId commands::DanceCommand::motorHandleToId(handles::HandleMap &handles, size_t handle) {
    return handles.get<handles::MotorHandle>(handle)->getMotorId();
}