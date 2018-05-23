#pragma once

#include <goliath/foundation.h>
#include "motor_controller.h"

namespace goliath::handles {
    class MotorHandle : public Handle {
    public:
        MotorHandle(const size_t &handleId, const controller::MotorId motorId) : Handle(handleId) {}

        controller::MotorId getMotorId() const;
    private:
        controller::MotorId id;
    };
}