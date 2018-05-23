#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <goliath/motor_handle.h>

#include "move_command.h"

using namespace goliath::handles;
using namespace goliath;

commands::MoveCommand::MoveCommand(const size_t &id)
        : Command(id, {HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER, HANDLE_LEFT_FRONT_MOTOR}) {
}

size_t mapCommandMotorToHandle(const size_t commandMotorId) {
    switch (commandMotorId) {
        case ::MotorCommand::Motor::MotorCommand_Motor_LEFT_FRONT:
            return HANDLE_LEFT_FRONT_MOTOR;
        default:
            throw std::runtime_error("Unknown motor id");
    }
}

void commands::MoveCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    controller::MotorController motorController(controllerSlave);

    const ::MoveCommand &moveCommand = message.movecommand();

    for (auto &command : moveCommand.commands()) {
        controller::MotorStatus motorCommand{};
        motorCommand.id = handles.get<handles::MotorHandle>(mapCommandMotorToHandle(command.motor()))->getMotorId();
        motorCommand.direction = static_cast<controller::MotorDirection>(command.gear());
        motorCommand.speed = command.speed();

        motorController.sendCommand(motorCommand);
    }

    for (auto const &lockedHandle: getRequiredHandles()) {
        handles[lockedHandle]->unlock();
    }
}
