#pragma once

#include <cstddef>
#include <cstdint>
#include "../servo_mode.h"

namespace goliath::servo {
    class WingState {
    public:
        WingState() = default;
        explicit WingState(const size_t &numberOfSectors);
        WingState(const WingState &other);

        void setWorldAngle(double angle);
        void reset(double newAngle);

        ServoMode getMode() const;
        void setMode(ServoMode mode);

        bool hasLostTracking() const;

        double getWorldAngle() const;

        double getAngle() const;
        void setAngle(double angle);

        size_t getSector() const;
        void setSector(size_t sector);

        size_t getNumberOfSectors() const;
        double getRatio() const;

        std::uint16_t getSpeed() const;
        void setSpeed(std::uint16_t speed);

        double getAngleFromWorldAngle(double angle) const;
        size_t getSectorFromWorldAngle(double angle) const;
        servo::Direction getDirectionFromWorldAngle(double angle, double threshold = 180) const;

        size_t getAdjacentSector(size_t sector, servo::Direction direction) const;
        size_t getNextSector(servo::Direction direction) const;
    private:
        size_t numberOfSectors;

        double angle;
        size_t sector;

        std::uint16_t speed;
        ServoMode mode;
        bool isTrackingLost;
    };
}