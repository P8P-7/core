#include <goliath/controller/commands/move_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <goliath/motor-controller.h>

using namespace goliath::handles;
using namespace goliath;

commands::MoveCommand::MoveCommand(const size_t &id)
        : Command(id, {HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER, HANDLE_LEFT_FRONT_MOTOR}) {
}

void commands::MoveCommand::execute(const HandleMap &handles, const CommandMessage &message) {
    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    const ::MoveCommand &moveCommand = message.movecommand();
    std::vector<motor_controller::MotorStatus> commands{};
    for (auto &command : moveCommand.commands()) {
        if (commandMotorToHandleMap.find(command.motor()) == commandMotorToHandleMap.end()) {
            throw std::runtime_error("Unknown motor id");
        }

        auto handle = handles.get<handles::MotorHandle>(commandMotorToHandleMap.at(command.motor()));
        motor_controller::MotorStatus motorCommand{};
        motorCommand.id = handle->getMotorId();
        motorCommand.direction = static_cast<motor_controller::MotorDirection>(command.gear());
        motorCommand.speed = static_cast<motor_controller::MotorSpeed>(command.speed());
        if (command.speed() < 0 || command.speed() > 255) {
            BOOST_LOG_TRIVIAL(fatal) << "Speed is out of range [0, 255] " << std::to_string(command.speed());
            continue;
        }

        commands.emplace_back(motorCommand);
    }
    motorController.sendCommands(commands.begin(), commands.end());

    for (auto const &lockedHandle: getRequiredHandles()) {
        handles[lockedHandle]->unlock();
    }
}
