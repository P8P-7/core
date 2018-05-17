#pragma once

#include <goliath/core.h>

/**
 * @file i2c_slave_handle.h
 * @author Group 7 - Informatica
 */


namespace goliath::i2c {
    using i2cAddress = std::uint8_t;
}

namespace goliath::handles {
    class I2cSlaveHandle : public Handle {
    public:
        I2cSlaveHandle(const i2c::i2cAddress& address);

        /**
         * @return slave address
         */
        const i2c::i2cAddress& getSlaveAddress() const;
    private:
        std::string device;
        i2c::i2cAddress address;
    };
}
