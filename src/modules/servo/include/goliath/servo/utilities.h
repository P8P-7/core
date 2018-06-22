#pragma once

#include "servo_mode.h"

namespace goliath::servo {
    class Utilities {
    public:
        static Direction flipDirection(Direction direction);
    };
}