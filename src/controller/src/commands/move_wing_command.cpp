#include <goliath/controller/commands/move_wing_command.h>

#include <boost/log/trivial.hpp>

using namespace goliath;
using namespace goliath::handles;

commands::MoveWingCommand::MoveWingCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                       HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}) {
}

void commands::MoveWingCommand::execute(HandleMap &handles, const CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move wing command has started";
    ::MoveWingCommand wingCommand = message.movewingcommand();

    std::vector<size_t> requiredHandles = getRequiredHandles();

    for (auto &servoCommand : wingCommand.commands()) {
        int handle = enumToHandle(servoCommand.motor());
        if (handle == -1) {
            // All handles case
            for (auto const &servoHandle : requiredHandles) {
                // Execute servo command
                executeServoCommand(std::static_pointer_cast<ServoHandle>(handles[servoHandle]), servoCommand);
            }
        } else {
            // Execute servo command
            executeServoCommand(std::static_pointer_cast<ServoHandle>(handles[handle]), servoCommand);
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Execution of move wing command has finished";
}

void commands::MoveWingCommand::executeServoCommand(std::shared_ptr<handles::ServoHandle> servoHandle,
                                                    const ServoCommand &servoCommand) {
    auto servoDevice = servoHandle->getDevice();

    switch (servoCommand.direction()) {
        case ServoCommand_Direction_UP:
            servoDevice->setMovingSpeed(servoCommand.speed() + 1024);
            break;
        case ServoCommand_Direction_DOWN:
            servoDevice->setMovingSpeed(servoCommand.speed());
            break;
        default:
            throw std::runtime_error("Direction could not be handled");
    }
}

int commands::MoveWingCommand::enumToHandle(ServoCommand_Motor motor) {
    switch (motor) {
        case ServoCommand_Motor_LEFT_FRONT:
            return HANDLE_LEFT_FRONT_WING_SERVO;
        case ServoCommand_Motor_LEFT_BACK:
            return HANDLE_LEFT_BACK_WING_SERVO;
        case ServoCommand_Motor_RIGHT_FRONT:
            return HANDLE_RIGHT_FRONT_WING_SERVO;
        case ServoCommand_Motor_RIGHT_BACK:
            return HANDLE_RIGHT_BACK_WING_SERVO;
        case ServoCommand_Motor_ALL:
        default:
            return -1;
    }
}
