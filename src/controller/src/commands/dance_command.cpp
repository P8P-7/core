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
    using namespace std::chrono_literals;
    using Emotion = proto::repositories::EmotionRepository;
    using Motor = proto::commands::MotorCommand;

    controller::LedStatus allColor{
        .lightingType = controller::LightingType::ALL,
        .colorType = controller::ColorType::HSV
    };

    controller::AllLedsMessage allGreen{
        allColor,
        .allLeds = {
            .hue = 125,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allRed{
        allColor,
        .allLeds = {
            .hue = 0,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allBlue{
        allColor,
        .allLeds = {
            .hue = 236,
            .saturation = 100,
            .value = 100,
            .rainbow = false
        }
    };

    controller::AllLedsMessage allYellow{
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
    controller::LedStripController ledController(ledControllerSlave);

    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    std::shared_ptr<repositories::EmotionRepository> emotionRepository = handles.get<handles::EmotionHandle>(
        HANDLE_EMOTIONS)->getEmotionRepository();

    servo::WingController wingController(repository);

    auto start = std::chrono::high_resolution_clock::now();

    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
    ledController.sendCommand(allGreen);

    servo::WingCommandBuilder upBuilder(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    upBuilder.setSpeed(511)
        .setAngle(74)
        .setDirection(servo::Direction::CLOCKWISE);

    servo::WingCommandBuilder standBuilder(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    standBuilder.setSpeed(511)
        .setAngle(340)
        .setDirection(servo::Direction::COUNTER_CLOCKWISE);

    servo::WingCommand leftBackStand = standBuilder.build();
    servo::WingCommand rightBackStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO))
        .build();

    wingController.execute({leftBackStand, rightBackStand});

    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    servo::WingCommand leftFrontStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::CLOCKWISE)
        .build();
    servo::WingCommand rightFrontStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::CLOCKWISE)
        .build();

    start = std::chrono::high_resolution_clock::now();

    wingController.execute({leftFrontStand});

    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    ledController.sendCommand(allRed);

    // Start chainsaw phase 1
    saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedLow);

    std::this_thread::sleep_for(500ms);

    for (int i = 0; i < 6; ++i) {
        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    std::this_thread::sleep_for(2s);

    saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedFast);

    std::this_thread::sleep_for(500ms);

    saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedMedium);

    for (int i = 0; i < 4; ++i) {
        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    for (int i = 0; i < 6; ++i) {
        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::RIGHT_FRONT, chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
    }

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = Motor::RIGHT_FRONT,
        .direction = motor_controller::MotorDirection::LOCKED,
        .speed = 0
    });

    servo::WingCommandBuilder allFlatBuilder = servo::WingCommandBuilder(
        handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO))
        .setDirection(servo::Direction::CLOCKWISE)
        .setSpeed(511)
        .setAngle(0);

    servo::WingCommand leftBackFlat = allFlatBuilder.build();
    servo::WingCommand rightBackFlat = allFlatBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO))
        .build();
    servo::WingCommand leftFrontFlat = allFlatBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::COUNTER_CLOCKWISE)
        .build();
    servo::WingCommand rightFrontFlat = allFlatBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::COUNTER_CLOCKWISE)
        .build();

    wingController.execute({leftBackFlat, rightBackFlat, leftFrontFlat, rightFrontFlat});

    start = std::chrono::high_resolution_clock::now();
    wingController.execute({leftBackFlat, rightBackFlat, leftFrontFlat, rightFrontFlat});
    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    // 15 Sec.
    emotionRepository->setCurrentEmotion(Emotion::SAD);
    ledController.sendCommand(allBlue);

    servo::WingCommand leftFrontUp = upBuilder.build();
    servo::WingCommand rightFrontUp = upBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO))
        .build();
    servo::WingCommand leftBackUp = upBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO))
        .build();
    servo::WingCommand rightBackUp = upBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO))
        .build();

    start = std::chrono::high_resolution_clock::now();
    wingController.execute({leftFrontUp, rightFrontUp, leftBackUp, rightBackUp});
    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    // TODO: Add dance when singing
    motorController.sendCommand(motor_controller::MotorStatus{
        .id = Motor::RIGHT_FRONT,
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 127
    });
    motorController.sendCommand(motor_controller::MotorStatus{
        .id = Motor::RIGHT_BACK,
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 127
    });
    std::this_thread::sleep_for(10s);

    emotionRepository->setCurrentEmotion(Emotion::SUPRISED);
    ledController.sendCommand(allYellow);

    for (auto motor : commands::MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = motor.first,
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = 127
        });
    }
    std::this_thread::sleep_for(3s);

    for (auto motor : commands::MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = motor.first,
            .direction = motor_controller::MotorDirection::BACKWARDS,
            .speed = 127
        });
    }
    std::this_thread::sleep_for(3s);

    std::this_thread::sleep_for(48s);

    // 1 Min. 23
    // Chainsaw phase 2

    start = std::chrono::high_resolution_clock::now();
    wingController.execute({leftBackStand, rightBackStand});
    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    start = std::chrono::high_resolution_clock::now();
    wingController.execute({rightFrontStand, leftFrontStand});
    std::this_thread::sleep_for(1s - (std::chrono::high_resolution_clock::now() - start));

    saw(motorController, Motor::LEFT_FRONT, chainSawSpeedExtraFast);

    std::this_thread::sleep_for(200ms);

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = Motor::LEFT_FRONT,
        .direction = motor_controller::MotorDirection::FREE,
        .speed = 0
    });

    std::this_thread::sleep_for(1800ms);

    for (int i = 0; i < 16; ++i) {
        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    saw(motorController, Motor::LEFT_FRONT, chainSawSpeedLow);
    std::this_thread::sleep_for(2s);

    for (int i = 0; i < 10; ++i) {
        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
    }

    for (int i = 0; i < 4; ++i) {
        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    for (int i = 0; i < 8; ++i) {
        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    saw(motorController, Motor::LEFT_FRONT, chainSawSpeedExtraFast);
    std::this_thread::sleep_for(5s);

    for (int i = 0; i < 6; ++i) {
        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);

        saw(motorController, Motor::LEFT_FRONT, chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
    }

    saw(motorController, Motor::LEFT_FRONT, chainSawSpeedLow);
    std::this_thread::sleep_for(3s);
}

void commands::DanceCommand::saw(motor_controller::MotorController &motorController, motor_controller::MotorId motor,
                                 std::uint8_t speed) {
    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motor,
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = speed
    });
}
