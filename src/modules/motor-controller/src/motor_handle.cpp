#include <goliath/motor-controller/motor_handle.h>

using namespace goliath;

motor_controller::MotorId handles::MotorHandle::getMotorId() const {
    return id;
}
