#include "servo_handle.h"

using namespace goliath::handles;

ServoHandle::ServoHandle(const size_t &id, Dynamixel &device, std::function<void(bool)> txCallback)
        : Handle(id), device(device) {
    device.setDirectionCallback(txCallback);
    device.setCWAngleLimit(0);
    device.setCCWAngleLimit(0);
}

Dynamixel &ServoHandle::getDevice() {
    return device;
}
