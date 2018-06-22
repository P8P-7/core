#pragma once

#include <goliath/servo/wing_handle.h>
#include <goliath/servo/servo_mode.h>

namespace goliath::servo {
    struct WingCommand {
        handles::WingHandle &handle;
        std::uint16_t speed;
        double angle;
        servo::Direction direction;
    };
}