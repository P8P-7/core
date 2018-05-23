#include <goliath/motor_handle.h>

using namespace goliath;

controller::MotorId handles::MotorHandle::getMotorId() const {
    return id;
}
