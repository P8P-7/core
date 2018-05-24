#pragma once

#include <cstdint>
#include <goliath/i2c.h>
#include <vector>

namespace goliath::controller {
    using LedId = std::uint8_t;
    using Hue = std::uint8_t;
    using Saturation = std::uint8_t;
    using Value = std::uint8_t;

    struct LedStatus {
        LedId led_id;
        Hue hue;
        Saturation saturation;
        Value value;
    };

    class LedController {
    public:
        LedController(i2c::I2cSlave &slave);

        /**
         * Sends the \param message to the physical controller to process.
         * @param message new led status to process
         */
        void sendCommand(const LedStatus &message);

    private:
        const static size_t numberOfPixels = 5;
        i2c::I2cSlave &slave;
    };
}
