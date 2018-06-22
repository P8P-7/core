#include <goliath/servo/wing_handle.h>
#include <goliath/servo/servo_handle.h>
#include <goliath/servo/servo_mode.h>
#include <goliath/servo/utilities.h>

using namespace goliath;
using namespace goliath::handles;

WingHandle::WingHandle(const size_t &id,
                        const std::shared_ptr<dynamixel::Dynamixel> &device,
                        size_t numberOfSectors,
                        bool mirrorAngle,
                        bool flipDirection,
                        double offsetAngle)
                        : ServoHandle(id, device),
                          numberOfSectors(numberOfSectors),
                          mirrorAngle(mirrorAngle),
                          flipDirection(flipDirection),
                          offsetAngle(offsetAngle) { }

double WingHandle::getTranslatedWorldAngle(double angle) const {
    return mirrorAngle ? 360.0 - angle : angle;
}

double WingHandle::getTranslatedAngle(double angle) const {
    return mirrorAngle ? 300.0 - angle : angle;
}

std::size_t WingHandle::getTranslatedSector(std::size_t sector) const {
    return mirrorAngle && sector != 0 ? numberOfSectors - sector : sector;
}

servo::Direction WingHandle::getTranslatedDirection(servo::Direction direction) const {
    return flipDirection ? direction : servo::Utilities::flipDirection(direction);
}