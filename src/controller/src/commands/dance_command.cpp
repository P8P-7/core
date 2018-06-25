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

    emotionRepository->setCurrentEmotion(Emotion::HAPPY);
    ledController.sendCommand(allGreen);

    servo::WingCommandBuilder upBuilder(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    upBuilder.setSpeed(511)
        .setAngle(74)
        .setDirection(servo::Direction::CLOCKWISE);

    servo::WingCommandBuilder standBuilder(handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO));
    standBuilder.setSpeed(511)
        .setAngle(260)
        .setDirection(servo::Direction::COUNTER_CLOCKWISE);

    servo::WingCommand leftBackStand = standBuilder.build();
    servo::WingCommand rightBackStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO))
        .build();
    servo::WingCommand leftFrontStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::CLOCKWISE)
        .build();
    servo::WingCommand rightFrontStand = standBuilder
        .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO))
        .setDirection(servo::Direction::CLOCKWISE)
        .build();

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

    moveWingOverTime(2000ms, wingController, {leftFrontStand, rightFrontStand});
    if (isInterrupted()) {
        return;
    }
    moveWingOverTime(1000ms, wingController, {allFlatBuilder
                                                  .setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO))
                                                  .build()});
    if (isInterrupted()) {
        return;
    }

    emotionRepository->setCurrentEmotion(Emotion::ANGRY);
    ledController.sendCommand(allRed);

    // Start chainsaw phase 1
    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedLow);

    std::this_thread::sleep_for(500ms);
    if (isInterrupted()) {
        return;
    }

    for (int i = 0; i < 6; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    std::this_thread::sleep_for(2s);
    if (isInterrupted()) {
        return;
    }

    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedFast);

    std::this_thread::sleep_for(500ms);
    if (isInterrupted()) {
        return;
    }

    saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedMedium);

    for (int i = 0; i < 4; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    for (int i = 0; i < 6; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController,motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR), chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
        .direction = motor_controller::MotorDirection::LOCKED,
        .speed = 0
    });

    moveWingOverTime(1000ms, wingController, {leftBackFlat, rightBackFlat, leftFrontFlat, rightFrontFlat});
    if (isInterrupted()) {
        return;
    }

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

    moveWingOverTime(1000ms, wingController, {leftFrontUp, rightFrontUp, leftBackUp, rightBackUp});
    if (isInterrupted()) {
        return;
    }

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 127
    });
    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 127
    });
    std::this_thread::sleep_for(10s);
    if (isInterrupted()) {
        return;
    }

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_RIGHT_FRONT_MOTOR),
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 0
    });
    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_RIGHT_BACK_MOTOR),
        .direction = motor_controller::MotorDirection::FORWARDS,
        .speed = 0
    });

    moveWingOverTime(1000ms, wingController, {leftFrontFlat, rightFrontFlat, leftBackFlat, rightBackFlat});
    if (isInterrupted()) {
        return;
    }

    emotionRepository->setCurrentEmotion(Emotion::SUPRISED);
    ledController.sendCommand(allYellow);

    for (auto motor : commands::MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = static_cast<motor_controller::MotorId>(motor.first),
            .direction = motor_controller::MotorDirection::FORWARDS,
            .speed = 127
        });
    }
    std::this_thread::sleep_for(3s);
    if (isInterrupted()) {
        return;
    }

    for (auto motor : commands::MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = static_cast<motor_controller::MotorId>(motor.first),
            .direction = motor_controller::MotorDirection::BACKWARDS,
            .speed = 127
        });
    }
    std::this_thread::sleep_for(3s);
    if (isInterrupted()) {
        return;
    }

    for (int i = 0; i < 10; ++i) {
        moveWingOverTime(1000ms, wingController, {leftFrontFlat, rightFrontFlat, leftBackFlat, rightBackFlat});
        if (isInterrupted()) {
            return;
        }

        moveWingOverTime(1000ms, wingController, {leftFrontUp, rightFrontUp, leftBackUp, rightBackUp});
        if (isInterrupted()) {
            return;
        }
    }

    std::this_thread::sleep_for(27s);

    // Chainsaw phase 2

    moveWingOverTime(1000ms, wingController, {leftBackStand, rightBackStand});
    if (isInterrupted()) {
        return;
    }
    moveWingOverTime(1000ms, wingController, {rightFrontStand, leftFrontStand});
    if (isInterrupted()) {
        return;
    }

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);

    std::this_thread::sleep_for(200ms);
    if (isInterrupted()) {
        return;
    }

    motorController.sendCommand(motor_controller::MotorStatus{
        .id = motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR),
        .direction = motor_controller::MotorDirection::LOCKED,
        .speed = 0
    });

    std::this_thread::sleep_for(1800ms);
    if (isInterrupted()) {
        return;
    }

    for (int i = 0; i < 16; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
    std::this_thread::sleep_for(2s);
    if (isInterrupted()) {
        return;
    }

    for (int i = 0; i < 10; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    for (int i = 0; i < 4; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    for (int i = 0; i < 8; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
    std::this_thread::sleep_for(5s);
    if (isInterrupted()) {
        return;
    }

    for (int i = 0; i < 6; ++i) {
        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedExtraFast);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }

        saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedMedium);
        std::this_thread::sleep_for(500ms);
        if (isInterrupted()) {
            return;
        }
    }

    saw(motorController, motorHandleToId(handles, HANDLE_LEFT_FRONT_MOTOR), chainSawSpeedLow);
    std::this_thread::sleep_for(3s);

    for (auto motor : commands::MoveCommand::COMMAND_MOTOR_TO_HANDLE_MAP) {
        motorController.sendCommand(motor_controller::MotorStatus{
            .id = static_cast<motor_controller::MotorId>(motor.first),
            .direction = motor_controller::MotorDirection::LOCKED,
            .speed = 0
        });
    }
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
