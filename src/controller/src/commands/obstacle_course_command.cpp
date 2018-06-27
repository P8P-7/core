#include <utility>

#include <goliath/controller/commands/obstacle_course_command.h>
#include <goliath/motor-controller/motor_controller.h>
#include <goliath/servo/wings/commandbuilder/wing_command_builder.h>

using namespace goliath;

commands::ObstacleCourseCommand::ObstacleCourseCommand(const size_t &id,
                                                       std::shared_ptr<repositories::WingStateRepository> repository)
        : BasicCommand(id, {HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR,
                            HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO,
                            HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER}), repository(std::move(repository)) {}

void commands::ObstacleCourseCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    i2c::I2cSlave slave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                        *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(slave);
    servo::WingController wingController(repository);

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
}
