#include <goliath/controller/commands/enter_command.h>

using namespace goliath::commands;
using namespace goliath;

EnterCommand::EnterCommand(const size_t &id, const motor_controller::MotorDirection &direction,
                           const motor_controller::MotorSpeed &speed)
    : BasicCommand(id, {HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                        HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                        HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR}), direction(direction), speed(speed) {
}

void EnterCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    std::vector<uint8_t> motorHandles = {HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                                         HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR};

    std::vector<motor_controller::MotorStatus> commands;

    for (uint8_t handle : motorHandles) {
        motor_controller::MotorStatus motorCommand {
            .id = handle,
            .direction = static_cast<motor_controller::MotorDirection>(direction),
            .speed = static_cast<motor_controller::MotorSpeed>(speed)
        };

        commands.emplace_back(motorCommand);
    }

    motorController.sendCommands(commands.begin(), commands.end());
}
