#include <goliath/controller/commands/wunderhorn_command.h>

#include <vector>
#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

const std::vector<size_t> leftMotors{HANDLE_LEFT_FRONT_MOTOR,
                                     HANDLE_LEFT_BACK_MOTOR};
const std::vector<size_t> rightMotors{HANDLE_RIGHT_FRONT_MOTOR,
                                      HANDLE_RIGHT_BACK_MOTOR};

commands::WunderhornCommand::WunderhornCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_CAM, HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                            HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR}),
          lineRoi(cv::Mat(480, 640, CV_8U), 0, 240, 639, 239), areaRoi(cv::Mat(480, 640, CV_8U), 220, 400, 200, 80) {
}

void commands::WunderhornCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    handleMap = handles;
    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
    vision::FollowLineDetector followLineDetector(webcam.getRoiFrame(lineRoi), 4, 40, 10, 20, 10, 10000);

    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    BOOST_LOG_TRIVIAL(trace) << "following line...";
    follow_line(followLineDetector, webcam, motorController);
    BOOST_LOG_TRIVIAL(trace) << "stopped following line";

    vision::ColorRegionDetector colorRegionDetector(webcam.getRoiFrame(areaRoi), 340, 20, 70, 100);

    BOOST_LOG_TRIVIAL(trace) << "driving into red zone";
    move(0, 50, motorController);

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

        cv::Mat new_frame = webcam.getRoiFrame(lineRoi);
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
            direction = -lines[0][1] * 4;
        } else if (lines[0][0] == vision::FollowLineDirection::RIGHT) {
            direction = lines[0][1] * 4;
        }
        BOOST_LOG_TRIVIAL(trace) << "direction set to " << direction;

        if (direction != lastDirection) {
            move(direction, 50, motorController);
            lastDirection = direction;
        }

        cv::Mat new_frame = camera.getFrame();
        followLineDetector.update(new_frame);
        lines = followLineDetector.detect();
    }

    BOOST_LOG_TRIVIAL(trace) << "setting speed to 0";
    move(0, 0, motorController);
}

void
commands::WunderhornCommand::move(double direction, int speed, motor_controller::MotorController &motorController) {
    std::vector<motor_controller::MotorStatus> commands{};

    double leftSpeed = speed;
    double rightSpeed = speed;

    if(std::abs(direction) > 1){
        direction = direction / std::abs(direction);
    }

    if (direction < 0) {
        leftSpeed -= leftSpeed * -direction;
        rightSpeed += rightSpeed * -direction;
    } else if (direction > 0) {
        leftSpeed += leftSpeed * direction;
        rightSpeed -= rightSpeed * direction;
    }

    BOOST_LOG_TRIVIAL(trace) << "speed left: " << leftSpeed << "\t right: " << rightSpeed;

    motor_controller::MotorDirection gear = motor_controller::MotorDirection::BACKWARDS;
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
