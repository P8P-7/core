#include <utility>

#include <goliath/controller/commands/obstacle_course_command.h>
#include <goliath/motor-controller/motor_controller.h>
#include <goliath/servo/wings/commandbuilder/wing_command_builder.h>

using namespace goliath::handles;
using namespace goliath;

const std::vector<size_t> leftMotors{HANDLE_LEFT_FRONT_MOTOR,
                                     HANDLE_LEFT_BACK_MOTOR};
const std::vector<size_t> rightMotors{HANDLE_RIGHT_FRONT_MOTOR,
                                      HANDLE_RIGHT_BACK_MOTOR};

commands::ObstacleCourseCommand::ObstacleCourseCommand(const size_t &id,
                                                       std::shared_ptr<repositories::WingStateRepository> repository)
        : BasicCommand(id, {HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR,
                            HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO,
                            HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER, HANDLE_CAM}), repository(std::move(repository)) {}

void commands::ObstacleCourseCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    handleMap = handles;

    i2c::I2cSlave slave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                        *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(slave);
    servo::WingController wingController(repository);

    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
    vision::FollowLineDetector followLineDetector(webcam.getFrame(), 4, 40, 10, 20, 10, 10000);

    servo::WingCommandBuilder builder;
    builder.setSpeed(1023).setAngle(20);
    wingController.execute({
                                   builder.setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_MOTOR))
                                           .setShortestDirection(repository->getState(HANDLE_LEFT_FRONT_MOTOR), 270)
                                           .build(),
                                   builder.setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_MOTOR))
                                           .setShortestDirection(repository->getState(HANDLE_RIGHT_FRONT_MOTOR), 270)
                                           .build()
                           });

    builder.setAngle(190);
    wingController.execute({
                                   builder.setHandle(handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_MOTOR))
                                           .setShortestDirection(repository->getState(HANDLE_LEFT_BACK_MOTOR), 270)
                                           .flipDirection()
                                           .build(),
                                   builder.setHandle(handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_MOTOR))
                                           .setShortestDirection(repository->getState(HANDLE_RIGHT_BACK_MOTOR), 270)
                                           .flipDirection()
                                           .build(),
                           });

    follow_line(followLineDetector, webcam, motorController);

    BOOST_LOG_TRIVIAL(trace) << "setting speed to 0";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    BOOST_LOG_TRIVIAL(trace) << "setting speed to 0";
    move(0, 0, motorController);
}

void commands::ObstacleCourseCommand::follow_line(vision::FollowLineDetector &followLineDetector, vision::Webcam &camera,
                                              motor_controller::MotorController &motorController) {
    std::vector<cv::Vec4d> lines = followLineDetector.detect();
    int noLinesCount = 0;
    double lastDirection = 0;

    while (noLinesCount < 30) {
        if (isInterrupted()) {
            move(0, 0, motorController);
            return;
        }

        if (lines[0][0] == vision::FollowLineDirection::NO_LINE) {
            noLinesCount++;
            BOOST_LOG_TRIVIAL(trace) << "no line detected";
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
}

void
commands::ObstacleCourseCommand::move(double direction, int speed, motor_controller::MotorController &motorController) {
    std::vector<motor_controller::MotorStatus> commands{};

    double leftSpeed = speed;
    double rightSpeed = speed;

    if (std::abs(direction) > 1) {
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

