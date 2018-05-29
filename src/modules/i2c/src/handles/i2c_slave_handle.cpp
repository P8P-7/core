#include <goliath/i2c/handles/i2c_slave_handle.h>

using namespace goliath::handles;
using namespace goliath::i2c;

I2cSlaveHandle::I2cSlaveHandle(const size_t &id, const I2cAddress& address)
    : Handle(id), address(address) {
}

const I2cAddress& I2cSlaveHandle::getSlaveAddress() const {
    return address;
}
