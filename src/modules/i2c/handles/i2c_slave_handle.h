#pragma once

#include <goliath/core.h>

/**
 * @file i2c_slave_handle.h
 * @author Group 7 - Informatica
 */


namespace goliath::i2c {
    using I2cAddress = std::uint8_t;
}

namespace goliath::handles {
    class I2cSlaveHandle : public Handle {
    public:
        I2cSlaveHandle(const size_t &id, const i2c::I2cAddress &address);

        /**
         * @return slave address
         */
        const i2c::I2cAddress& getSlaveAddress() const;
    private:
        std::string device;
        i2c::I2cAddress address;
    };
}
