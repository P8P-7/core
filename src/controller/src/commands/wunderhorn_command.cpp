#include <goliath/controller/commands/wunderhorn_command.h>

#include <vector>
#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

const std::vector<proto::commands::MotorCommand_Motor> leftMotors{proto::commands::MotorCommand_Motor_LEFT_FRONT,
                                                                  proto::commands::MotorCommand_Motor_LEFT_BACK};
const std::vector<proto::commands::MotorCommand_Motor> rightMotors{proto::commands::MotorCommand_Motor_RIGHT_FRONT,
                                                                   proto::commands::MotorCommand_Motor_RIGHT_BACK};

commands::WunderhornCommand::WunderhornCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_CAM, HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                            HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR}) {
}

void commands::WunderhornCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    handleMap = handles;
    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
    vision::FollowLineDetector followLineDetector(webcam.getFrame(), 4, 80, 20, 20, 10, 10000);

    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    BOOST_LOG_TRIVIAL(trace) << "following line...";
    follow_line(followLineDetector, webcam, motorController);
    BOOST_LOG_TRIVIAL(trace) << "stopped following line";

    vision::ColorRegionDetector colorRegionDetector(webcam.getFrame(), 0, 0, 0, 0);

    BOOST_LOG_TRIVIAL(trace) << "driving into red zone";
    move(0, 128, motorController);

    while (true) {
        if (isInterrupted()) {
            move(0, 0, motorController);
            return;
        }

        int detected;

        do {
            detected = static_cast<int>(colorRegionDetector.detect()[0][0]);
        } while (detected == -1);

        if (detected == 1) {
            BOOST_LOG_TRIVIAL(trace) << "red zone detected, stopping in 500ms";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            move(0, 0, motorController);
            BOOST_LOG_TRIVIAL(trace) << "stopped";
            break;
        }

        cv::Mat new_frame = webcam.getFrame();
        followLineDetector.update(new_frame);
    }

    BOOST_LOG_TRIVIAL(trace) << "waiting 30 seconds";
    std::this_thread::sleep_for(std::chrono::seconds(30));

    BOOST_LOG_TRIVIAL(trace) << "following line...";
    follow_line(followLineDetector, webcam, motorController);
    BOOST_LOG_TRIVIAL(trace) << "stopped following line";

    handles[HANDLE_CAM]->unlock();
}

void commands::WunderhornCommand::follow_line(vision::FollowLineDetector &followLineDetector, vision::Webcam &camera,
                                              motor_controller::MotorController &motorController) {
    std::vector<cv::Vec4d> lines = followLineDetector.detect();
    int noLinesCount = 0;
    double lastDirection = 0;

    while (noLinesCount < 20) {
        if (isInterrupted()) {
            move(0, 0, motorController);
            return;
        }

        if (lines[0][0] == vision::FollowLineDirection::NO_LINE) {
            noLinesCount++;
        }

        double direction = 0;
        if (lines[0][0] == vision::FollowLineDirection::LEFT) {
            direction = -lines[0][1];
        } else if (lines[0][0] == vision::FollowLineDirection::RIGHT) {
            direction = lines[0][1];
        }
        BOOST_LOG_TRIVIAL(trace) << "direction set to " << direction;

        if (direction != lastDirection) {
            move(direction, 128, motorController);
            lastDirection = direction;
        }

        cv::Mat new_frame = camera.getFrame();
        followLineDetector.update(new_frame);
    }

    BOOST_LOG_TRIVIAL(trace) << "setting speed to 0";
    move(0, 0, motorController);
}

void
commands::WunderhornCommand::move(double direction, int speed, motor_controller::MotorController &motorController) {
    std::vector<motor_controller::MotorStatus> commands{};

    double leftSpeed = speed;
    double rightSpeed = speed;

    if (direction < 0) {
        leftSpeed -= leftSpeed * (1 + direction);
    } else if (direction > 0) {
        rightSpeed -= rightSpeed * (1 - direction);
    }

    motor_controller::MotorDirection gear = motor_controller::MotorDirection::FORWARDS;
    if (speed == 0) {
        gear = motor_controller::MotorDirection::LOCKED;
    }

    for (size_t motor : leftMotors) {
        auto handle = handleMap.get<handles::MotorHandle>(motor);
        motor_controller::MotorStatus motorCommand{
                .id = handle->getMotorId(),
                .direction = gear,
                .speed = static_cast<motor_controller::MotorSpeed>(leftSpeed)
        };

        commands.emplace_back(motorCommand);
    }

    for (size_t motor : rightMotors) {
        auto handle = handleMap.get<handles::MotorHandle>(motor);
        motor_controller::MotorStatus motorCommand{
                .id = handle->getMotorId(),
                .direction = gear,
                .speed = static_cast<motor_controller::MotorSpeed>(rightSpeed)
        };

        commands.emplace_back(motorCommand);
    }

    if (!commands.empty()) {
        motorController.sendCommands(commands.begin(), commands.end());
    }
}
