#include <goliath/controller/commands/move_command.h>

#include <thread>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <goliath/motor-controller.h>
#include <boost/range/adaptor/map.hpp>

using namespace goliath;
using namespace goliath::handles;
using namespace goliath::commands;

MoveCommand::MoveCommand(const size_t &id)
        : QueueCommand(id, {HANDLE_I2C_BUS, HANDLE_MOTOR_CONTROLLER,
                            HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR}) { }

void MoveCommand::execute(const std::vector<proto::commands::MotorCommand> &commandMessages) {
    i2c::I2cSlave controllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                  *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(controllerSlave);

    std::vector<motor_controller::MotorStatus> commands{};
    for (auto &command : commandMessages) {
        if (commandMotorToHandleMap.find(command.motor()) == commandMotorToHandleMap.end()) {
            BOOST_LOG_TRIVIAL(fatal) << "Unknown motor " << std::to_string(command.motor());
            continue;
        }

        if (command.speed() < 0 || command.speed() > 255) {
            BOOST_LOG_TRIVIAL(fatal) << "Speed is out of range [0, 255] " << std::to_string(command.speed());
            continue;
        }

        auto handle = handles.get<handles::MotorHandle>(commandMotorToHandleMap.at(command.motor()));
        motor_controller::MotorStatus motorCommand{
                .id = handle->getMotorId(),
                .direction = static_cast<motor_controller::MotorDirection>(command.gear()),
                .speed = static_cast<motor_controller::MotorSpeed>(command.speed())
        };

        commands.emplace_back(motorCommand);
    }

    if (!commands.empty()) {
        motorController.sendCommands(commands.begin(), commands.end());
    }
}

void MoveCommand::process() {
    std::unique_lock<std::mutex> lock(mutex);

    std::map<proto::commands::MotorCommand_Motor, proto::commands::MotorCommand> map;

    while (!queue.empty()) {
        auto message = queue.front().movecommand();
        queue.pop();

        for (const auto &command : message.commands()) {
            map[command.motor()] = command;
        }
    }

    lock.unlock();

    std::vector<proto::commands::MotorCommand> commands;
    for (auto &command : map | boost::adaptors::map_values) {
        commands.emplace_back(command);
    }

    try {
        execute(commands);
    } catch (std::exception &ex) {
        BOOST_LOG_TRIVIAL(error) << "Error executing command \"" << getId() << "\" what(): " << ex.what();
    }
}
