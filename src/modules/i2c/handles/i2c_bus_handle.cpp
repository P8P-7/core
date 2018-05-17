#include "i2c_bus_handle.h"

using namespace goliath::handles;

I2cBusHandle::I2cBusHandle(const std::string &device)
        : device(device) {
}

const std::string &I2cBusHandle::getDevicePath() const {
    return device;
}
