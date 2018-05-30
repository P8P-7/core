#pragma once

#include <goliath/i2c.h>

#include <cstdint>
#include <vector>

namespace goliath::controller {
    using LedId = std::uint8_t;
    using Hue = std::uint8_t;
    using Saturation = std::uint8_t;
    using Value = std::uint8_t;

    enum class LightingType : std::uint8_t {
        SPECIFIC = 0,
        ALL = 1,
        CIRCLE = 2,
    };

    enum class ColorType : std::uint8_t {
        HSV = 0,
        RAINBOW = 1,
    };

    struct LedStatus {
        LightingType lightingType;
        ColorType colorType;
    };

    struct SpecificColour {
        LedId ledId;
        Hue hue;
        Saturation saturation;
        Value value;
    };

    struct AllLeds {
        Hue hue;
        Saturation saturation;
        Value value;
        bool rainbow;
    };

    struct SpecificRainbow {
        LedId ledId;
        Hue startHue;
    };
    struct Circle {
        LedId startId;
        LedId endId;
        bool cw;
        Hue hue;
        Saturation saturation;
    };

    struct SpecColMessage {
        LedStatus ledStatus;
        SpecificColour specificColour;
    };

    struct AllLedsMessage {
        LedStatus ledStatus;
        AllLeds allLeds;
    };

    struct SpecRainMessage {
        LedStatus ledStatus;
        SpecificRainbow specificRainbow;
    };

    struct CircleMessage {
        LedStatus ledStatus;
        Circle circle;
    };

    class LedStripController {
    public:
        LedStripController(i2c::I2cSlave &slave);

        /**
         * Sends the @param message to the physical controller to process.
         * @param message new led status to process
         */
        void sendCommand(const SpecColMessage &message);
        void sendCommand(const AllLedsMessage &message);
        void sendCommand(const SpecRainMessage &message);
        void sendCommand(const CircleMessage &message);

        const static size_t numberOfPixels = 5;

    private:
        i2c::I2cSlave &slave;
    };
}
