#pragma once

namespace goliath::servo {
    enum class ServoMode {
        WHEEL,
        JOINT
    };
    enum class Direction {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };
}