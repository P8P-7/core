#include "i2c_slave_handle.h"

using namespace goliath::handles;
using namespace goliath::i2c;

I2cSlaveHandle::I2cSlaveHandle(const i2cAddress& address)
    : address(address) {
}

const i2cAddress& I2cSlaveHandle::getSlaveAddress() const {
    return address;
}
