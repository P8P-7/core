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
        BOOST_LOG_TRIVIAL(fatal) << "Wings not calibrated";
        return;
    }

    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    led_controller::LedStripController ledController(ledControllerSlave);

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
    clock::time_point realStart = clock::now(), start = clock::now();

    BOOST_LOG_TRIVIAL(info) << "Do doggy style chainsaw action";
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

    // Start sawing section
    BOOST_LOG_TRIVIAL(info) << "Dance: Sawing section 1";
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
    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    std::this_thread::sleep_until(start + 13s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 65 * 4, 256, false);
    std::this_thread::sleep_until(start + 15s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), 0);
    emotionRepository->setCurrentEmotion(Emotion::NEUTRAL);

    BOOST_LOG_TRIVIAL(info) << "Dance: doing pushups";
    start = clock::now();
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
    // 2
    leftFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    leftFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    // 3
    leftFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(345).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    leftFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(300).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    // 4
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
    emotionRepository->setCurrentEmotion(Emotion::SAD);

    BOOST_LOG_TRIVIAL(info) << "Start driving forwards and backwards";
    start = clock::now();

    // Drive forwards a bit and then back again
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_until(start + 2s);

    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
    };
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());

    emotionRepository->setCurrentEmotion(Emotion::WINK_LEFT);
    std::this_thread::sleep_until(start + 4s);
    emotionRepository->setCurrentEmotion(Emotion::HAPPY);

    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
    };
    // Drive backwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_until(start + 6s);

    leftFront.setAngle(0).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setAngle(0).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    leftBack.setAngle(0).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(0).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});

    std::this_thread::sleep_for(2s);
    BOOST_LOG_TRIVIAL(info) << "Start synchroon zwemmen";
    start = clock::now();

    // Stukje synchroon zwemmen
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::FORWARDS,
                    .speed = 32
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 1s);

    wingController.setAngle(*leftFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 0 * 4, 512, false);
    std::this_thread::sleep_until(start + 2s);

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 3s);

    wingController.setAngle(*leftFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 0 * 4, 512, false);
    std::this_thread::sleep_until(start + 4s);

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 5s);

    wingController.setAngle(*leftFront.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 75 * 4, 512, false);
    std::this_thread::sleep_until(start + 6s);
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 32
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 7s);

    wingController.setAngle(*leftFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 0 * 4, 512, false);
    std::this_thread::sleep_until(start + 8s);

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 9s);

    wingController.setAngle(*leftFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 0 * 4, 512, false);
    std::this_thread::sleep_until(start + 10s);

    wingController.setAngle(*leftFront.getHandle(), 20 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 0 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 20 * 4, 512, false);
    std::this_thread::sleep_until(start + 11s);

    wingController.setAngle(*leftFront.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*leftBack.getHandle(), 75 * 4, 512, false);
    wingController.setAngle(*rightBack.getHandle(), 75 * 4, 512, false);
    std::this_thread::sleep_until(start + 12s);
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
    };
    // Stop motors
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_until(start + 15s);

    BOOST_LOG_TRIVIAL(info) << "Doing a sick burn out";
    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    start = clock::now();

    leftBack.setAngle(0).setSpeed(256).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(0).setSpeed(256).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});
    std::this_thread::sleep_until(start + 2s);

    // First do the right front burnout
    motorController.sendCommand(motor_controller::MotorStatus{
            .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = 255
    });
    std::this_thread::sleep_until(start + 8s);
    // Do left front burnout
    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 255
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
    };
    // Stop motors
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_until(start + 12s);
    // Stop left front motor
    motorController.sendCommand(motor_controller::MotorStatus{
            .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
            .direction = motor_controller::MotorDirection::LOCKED,
            .speed = 0
    });
    // Reset to base position
    leftBack.setAngle(75).setSpeed(256).setDirection(servo::Direction::CLOCKWISE);
    rightBack.setAngle(75).setSpeed(256).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    BOOST_LOG_TRIVIAL(info) << "Do a 360";
    start = clock::now();
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
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::BACKWARDS,
                    .speed = 128
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());
    std::this_thread::sleep_until(start + 5s);

    motorCommands = {
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_LEFT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
            motor_controller::MotorStatus{
                    .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
                    .direction = motor_controller::MotorDirection::LOCKED,
                    .speed = 0
            },
    };
    // Drive forwards
    motorController.sendCommands(motorCommands.begin(), motorCommands.end());

    BOOST_LOG_TRIVIAL(trace) << "Setting level";
    leftFront.setAngle(0).setSpeed(128).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setAngle(0).setSpeed(128).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    leftBack.setAngle(0).setSpeed(128).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(0).setSpeed(128).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});
    emotionRepository->setCurrentEmotion(Emotion::NEUTRAL);

    // Fill time
    leftFront.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    leftBack.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    rightBack.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});
    leftFront.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    leftBack.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});
    leftFront.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    leftBack.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    rightBack.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});
    leftFront.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    leftBack.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(0).setSpeed(512).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build(), leftBack.build(), rightBack.build()}});
    
    leftBack.setSpeed(1023).setAngle(190).setDirection(servo::Direction::CLOCKWISE);
    rightBack.setSpeed(1023).setAngle(190).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    leftFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({leftFront.build(), rightFront.build()});

    leftBack.setSpeed(1023).setAngle(220).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setSpeed(1023).setAngle(220).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    leftFront.setSpeed(512).setAngle(65).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build()}});

    emotionRepository->setCurrentEmotion(Emotion::NEUTRAL);
    std::this_thread::sleep_until(realStart + 1min + 23s);
    start = clock::now();
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 75 * 4, 756, false);
    std::this_thread::sleep_until(realStart + 1min + 24s); // 1.24

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), 0);
    wingController.setAngle(*leftFront.getHandle(), 15 * 4, 756, false);
    std::this_thread::sleep_until(realStart + 1min + 25s); // 1.25

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 35 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 29s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*leftFront.getHandle(), 15 * 4, 756, false);
    std::this_thread::sleep_until(realStart + 1min + 30s); //
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 60 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 32s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*leftFront.getHandle(), 40 * 4, 756, false);
    std::this_thread::sleep_until(realStart + 1min + 33s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
    wingController.setAngle(*leftFront.getHandle(), 10 * 4, 756, false);
    std::this_thread::sleep_until(realStart + 1min + 35s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 75 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 36s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedFast);
    wingController.setAngle(*leftFront.getHandle(), 50 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 38s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*leftFront.getHandle(), 65 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 39s);
    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), 0);
    wingController.setAngle(*leftFront.getHandle(), 0, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 40s);

    leftFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftFront.build()}});
    std::this_thread::sleep_until(realStart + 1min + 42s);
    rightFront.setSpeed(1023).setAngle(65).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{rightFront.build()}});
    std::this_thread::sleep_until(realStart + 1min + 44s);

    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 46s);
    wingController.setAngle(*rightFront.getHandle(), 50 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 48s);
    wingController.setAngle(*rightFront.getHandle(), 65 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 49s);
    wingController.setAngle(*rightFront.getHandle(), 40 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 50s);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 51s);
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), 0);
    wingController.setAngle(*rightFront.getHandle(), 0 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 51s + 500ms);

    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 52s + 500ms);
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedMedium);
    wingController.setAngle(*rightFront.getHandle(), 50 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 53s);
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedExtraFast);
    wingController.setAngle(*rightFront.getHandle(), 75 * 4, 1023, false);
    std::this_thread::sleep_until(realStart + 1min + 54s);
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), 0);

    rightFront.setSpeed(1023).setAngle(290).setDirection(servo::Direction::COUNTER_CLOCKWISE);

    leftBack.setAngle(190).setSpeed(1023).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    rightBack.setAngle(190).setSpeed(1023).setDirection(servo::Direction::COUNTER_CLOCKWISE);
    wingController.execute({{leftBack.build(), rightBack.build()}});

    leftFront.setAngle(0).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    rightFront.setAngle(0).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    wingController.execute({{leftFront.build(), rightFront.build()}});

    leftFront.setAngle(75).setSpeed(1023).setDirection(servo::Direction::CLOCKWISE);
    emotionRepository->setCurrentEmotion(Emotion::WINK_LEFT);
    wingController.execute({{leftFront.build()}});
    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
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