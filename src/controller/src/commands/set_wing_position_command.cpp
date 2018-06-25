#include <utility>

#include <algorithm>
#include <goliath/controller/commands/set_wing_position_command.h>

namespace ProtoCommands = goliath::proto::commands;
using namespace goliath;
using namespace goliath::commands;

SetWingPositionCommand::SetWingPositionCommand(const size_t &id,
                                               std::shared_ptr<repositories::WingStateRepository> repository)
    : BasicCommand(id, {HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                        HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO}), repository(
    std::move(repository)) {}

void SetWingPositionCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    auto &command = message.setwingpositioncommand();
    auto speed = static_cast<std::uint16_t>(std::floor(1023 * command.speed()));

    if (repository->hasLostTracking()) {
        calibrate(handles);
    }

    switch (command.wingposition()) {
        case ProtoCommands::SetWingPositionCommand_Position_DEFAULT:
            setFlat(handles, speed);
            break;
        case ProtoCommands::SetWingPositionCommand_Position_ALL_UP:
            setUp(handles, speed);
            break;
        case ProtoCommands::SetWingPositionCommand_Position_ALL_DOWN:
            setDown(handles, speed);
            break;
        case ProtoCommands::SetWingPositionCommand_Position_RAMP:
            setRamp(handles, speed);
            break;
        default:
            BOOST_LOG_TRIVIAL(fatal) << "Unknown position";
    }
}

void SetWingPositionCommand::calibrate(handles::HandleMap &handles) {
    servo::WingController controller(repository);
    controller.setMode(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO), servo::ServoMode::JOINT);
    controller.calibrate(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO));
    controller.setMode(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO), servo::ServoMode::JOINT);
    controller.calibrate(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO));
    controller.setMode(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO), servo::ServoMode::JOINT);
    controller.calibrate(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO));
    controller.setMode(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO), servo::ServoMode::JOINT);
    controller.calibrate(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO));
}

void SetWingPositionCommand::setFlat(handles::HandleMap &handles, std::uint16_t speed) {
    servo::WingController controller(repository);
    servo::WingCommand leftFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO), speed,
                                                0);
    servo::WingCommand rightFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO),
                                                 speed, 0);
    servo::WingCommand leftBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO), speed,
                                               0);
    servo::WingCommand rightBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO), speed,
                                                0);
    std::vector<servo::WingCommand> backCommands = {leftFront, rightFront, leftBack, rightBack};
    controller.execute(backCommands);
}

void SetWingPositionCommand::setUp(handles::HandleMap &handles, std::uint16_t speed) {
    servo::WingController controller(repository);
    servo::WingCommand leftFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO), speed,
                                                90, servo::Direction::CLOCKWISE);
    servo::WingCommand rightFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO),
                                                 speed, 90, servo::Direction::CLOCKWISE);
    std::vector<servo::WingCommand> frontCommands = {leftFront, rightFront};
    controller.execute(frontCommands);

    servo::WingCommand leftBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO), speed,
                                               90, servo::Direction::CLOCKWISE);
    servo::WingCommand rightBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO), speed,
                                                90, servo::Direction::CLOCKWISE);
    std::vector<servo::WingCommand> backCommands = {leftBack, rightBack};
    controller.execute(backCommands);
}

void SetWingPositionCommand::setRamp(handles::HandleMap &handles, std::uint16_t speed) {
    servo::WingController controller(repository);
    servo::WingCommand leftFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO), speed,
                                                20);
    servo::WingCommand rightFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO),
                                                 speed, 20);
    std::vector<servo::WingCommand> frontCommands = {leftFront, rightFront};
    controller.execute(frontCommands);

    servo::WingCommand leftBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO), speed,
                                               190);
    servo::WingCommand rightBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO), speed,
                                                190);
    std::vector<servo::WingCommand> backCommands = {leftBack, rightBack};
    controller.execute(backCommands);
}

void SetWingPositionCommand::setDown(handles::HandleMap &handles, std::uint16_t speed) {
    servo::WingController controller(repository);
    servo::WingCommand leftFrontCenter = buildCommand(
        *handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO),
        speed,
        std::min(30.0, std::max(repository->getState(HANDLE_LEFT_BACK_WING_SERVO).getWorldAngle(), 90.0))
    );
    servo::WingCommand rightFrontCenter = buildCommand(
        *handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO),
        speed,
        std::min(30.0, std::max(repository->getState(HANDLE_RIGHT_BACK_WING_SERVO).getWorldAngle(), 90.0))
    );
    controller.execute({leftFrontCenter, rightFrontCenter});

    servo::WingCommand leftFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_FRONT_WING_SERVO), speed,
                                                280, servo::Direction::COUNTER_CLOCKWISE);
    servo::WingCommand rightFront = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_FRONT_WING_SERVO),
                                                 speed, 280, servo::Direction::COUNTER_CLOCKWISE);
    controller.execute({leftFront, rightFront});

    servo::WingCommand leftBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_LEFT_BACK_WING_SERVO), speed,
                                               280, servo::Direction::COUNTER_CLOCKWISE);
    servo::WingCommand rightBack = buildCommand(*handles.get<handles::WingHandle>(HANDLE_RIGHT_BACK_WING_SERVO), speed,
                                                280, servo::Direction::COUNTER_CLOCKWISE);
    controller.execute({leftBack, rightBack});
}

servo::WingCommand SetWingPositionCommand::buildCommand(handles::WingHandle &handle, std::uint16_t speed, double angle) {
    auto &state = repository->getState(handle.getId());
    return buildCommand(handle, speed, angle,
                        handle.getTranslatedDirection(state.getDirectionFromWorldAngle(angle, 190)));
}

servo::WingCommand SetWingPositionCommand::buildCommand(handles::WingHandle &handle, std::uint16_t speed, double angle,
                                                        servo::Direction direction) {
    return {
        .handle = handle,
        .speed = speed,
        .angle = angle,
        .direction = direction
    };
}
