#include <goliath/controller/commands/move_wing_command.h>

using namespace goliath;
using namespace goliath::handles;

commands::MoveWingCommand::MoveWingCommand(const size_t &id, std::shared_ptr<repositories::WingStateRepository> repository)
        : BasicCommand(id, {HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}), repository(repository) {
}

void commands::MoveWingCommand::execute(HandleMap &handles, const proto::CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of move wing command has started";
    const proto::commands::MoveWingCommand wingCommand = message.movewingcommand();

    for (auto &servoCommand : wingCommand.commands()) {
        std::size_t handle = enumToHandle(servoCommand.motor());
        if (handle == std::numeric_limits<std::size_t>::max()) {
            // All handles case
            for (auto const &servoHandle : getRequiredHandles()) {
                // Execute servo command
                executeServoCommand(handles.get<handles::WingHandle>(servoHandle), servoCommand);
            }
        } else {
            // Execute servo command
            executeServoCommand(handles.get<handles::WingHandle>(handle), servoCommand);
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Execution of move wing command has finished";
}

void commands::MoveWingCommand::executeServoCommand(std::shared_ptr<handles::WingHandle> handle,
                                                    const proto::commands::ServoCommand &command) {
    servo::WingController controller(repository);
    auto &state = repository->getState(handle->getId());
    if (state.getMode() == servo::ServoMode::JOINT) {
        controller.setMode(*handle, servo::ServoMode::WHEEL);
    }

    switch (command.direction()) {
        case proto::commands::ServoCommand_Direction_UP:
            controller.setSpeed(*handle, command.speed(), servo::Direction::CLOCKWISE);
            break;
        case proto::commands::ServoCommand_Direction_DOWN:
            controller.setSpeed(*handle, command.speed(), servo::Direction::COUNTER_CLOCKWISE);
            break;
        default:
            throw std::runtime_error("Direction could not be handled");
    }
}

std::size_t commands::MoveWingCommand::enumToHandle(proto::commands::ServoCommand_Motor motor) {
    switch (motor) {
        case proto::commands::ServoCommand_Motor_LEFT_FRONT:
            return HANDLE_LEFT_FRONT_WING_SERVO;
        case proto::commands::ServoCommand_Motor_LEFT_BACK:
            return HANDLE_LEFT_BACK_WING_SERVO;
        case proto::commands::ServoCommand_Motor_RIGHT_FRONT:
            return HANDLE_RIGHT_FRONT_WING_SERVO;
        case proto::commands::ServoCommand_Motor_RIGHT_BACK:
            return HANDLE_RIGHT_BACK_WING_SERVO;
        case proto::commands::ServoCommand_Motor_ALL:
        default:
            return std::numeric_limits<std::size_t>::max();
    }
}
