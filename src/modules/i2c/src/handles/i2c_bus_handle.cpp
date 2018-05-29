#include <goliath/i2c/handles/i2c_bus_handle.h>

using namespace goliath::handles;

I2cBusHandle::I2cBusHandle(const size_t &id, const std::string &device)
        : Handle(id), device(device) {
}

const std::string& I2cBusHandle::getDevicePath() const {
    return device;
}
