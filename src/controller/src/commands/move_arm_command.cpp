#include <goliath/controller/commands/move_arm_command.h>

using namespace goliath::commands;
using namespace goliath;

MoveArmCommand::MoveArmCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_BASE_ARM, HANDLE_FIRST_JOINT_ARM, HANDLE_SECOND_JOINT_ARM}) { }

void MoveArmCommand::execute(goliath::handles::HandleMap &handles,
                             const goliath::proto::CommandMessage &message) {
    const proto::commands::MoveArmCommand &moveArmCommand = message.movearmcommand();

    auto servoBase = handles.get<handles::ServoHandle>(HANDLE_BASE_ARM)->getDevice();
    auto servoFirstJoint = handles.get<handles::ServoHandle>(HANDLE_FIRST_JOINT_ARM)->getDevice();
    auto servoSecondJoint = handles.get<handles::ServoHandle>(HANDLE_SECOND_JOINT_ARM)->getDevice();

    switch (moveArmCommand.position()) {
        case proto::commands::MoveArmCommand::TUCKED:
            servoBase->setGoalPosition(197);
            servoFirstJoint->setGoalPosition(768);
            servoSecondJoint->setGoalPosition(755);
            break;
        case proto::commands::MoveArmCommand::HIGH:
            servoBase->setGoalPosition(500);
            servoFirstJoint->setGoalPosition(465);
            servoSecondJoint->setGoalPosition(448);
            break;
        case proto::commands::MoveArmCommand::MED:
            servoBase->setGoalPosition(500);
            servoFirstJoint->setGoalPosition(203);
            servoSecondJoint->setGoalPosition(229);
            break;
        case proto::commands::MoveArmCommand::LOW:
            servoBase->setGoalPosition(500);
            servoFirstJoint->setGoalPosition(239);
            servoSecondJoint->setGoalPosition(426);
            break;
        default:
            servoBase->setGoalPosition(197);
            servoFirstJoint->setGoalPosition(768);
            servoSecondJoint->setGoalPosition(755);
            break;
    }
}
