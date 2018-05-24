#pragma once

#include <goliath/foundation.h>
#include "motor_controller.h"

namespace goliath::handles {
    class MotorHandle : public Handle {
    public:
        MotorHandle(const size_t &handleId, const motor_controller::MotorId motorId) : Handle(handleId) {}

        motor_controller::MotorId getMotorId() const;
    private:
        motor_controller::MotorId id;
    };
}