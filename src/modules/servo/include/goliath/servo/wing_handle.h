#pragma once

#include <goliath/servo/servo_handle.h>
#include <goliath/servo/servo_mode.h>

namespace goliath::handles {
    class WingHandle : public ServoHandle {
    public:
        WingHandle(const size_t &id,
                    const std::shared_ptr<dynamixel::Dynamixel> &device,
                    std::size_t numberOfSectors,
                    bool mirrorAngle,
                    bool flipDirection,
                    double offsetAngle);

        double getTranslatedWorldAngle(double angle) const;
        double getTranslatedAngle(double angle) const;
        std::size_t getTranslatedSector(std::size_t sector) const;
        servo::Direction getTranslatedDirection(servo::Direction direction) const;
    private:
        std::size_t numberOfSectors;
        bool mirrorAngle;
        bool flipDirection;
        double offsetAngle;
    };
}