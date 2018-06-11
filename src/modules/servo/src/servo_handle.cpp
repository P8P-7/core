#include <goliath/servo/servo_handle.h>

using namespace goliath::handles;
using namespace goliath::dynamixel;

ServoHandle::ServoHandle(const size_t &id, std::shared_ptr<Dynamixel> device, std::function<void(bool)> txCallback)
        : Handle(id), device(device) {
    device->setDirectionCallback(txCallback);
    device->setCWAngleLimit(0);
    device->setCCWAngleLimit(0);
}

std::shared_ptr<Dynamixel> ServoHandle::getDevice() {
    return device;
}
