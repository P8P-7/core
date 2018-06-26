#include <goliath/controller/commands/grip_command.h>

using namespace goliath::commands;
using namespace goliath;

GripCommand::GripCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_GRIPPER_ARM}) { }

void GripCommand::execute(goliath::handles::HandleMap &handles,
                          const goliath::proto::CommandMessage &message) {
    const proto::commands::GripCommand &gripCommand = message.gripcommand();

    auto servoGripper = handles.get<handles::ServoHandle>(HANDLE_GRIPPER_ARM)->getDevice();

    servoGripper->setGoalPosition(static_cast<short>(gripCommand.gripping() ? 735 : 447));
}
