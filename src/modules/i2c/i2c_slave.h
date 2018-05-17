#pragma once

#include <string>

#include "handles/i2c_bus_handle.h"
#include "handles/i2c_slave_handle.h"

/**
 * @file i2c_slave.h
 * @author Group 7 - Informatica
 */


namespace goliath::i2c {
    class I2cSlave {
    public:
        /**
         * @brief Initializes an instance with a handle to a bus and a handle to a slave on that bus.
         * @param busHandle
         * @param slaveHandle
         */
        I2cSlave(const handles::I2cBusHandle &busHandle, const handles::I2cSlaveHandle &slaveHandle);
        /**
         * @brief Initializes an instance with a bus handle and an address corresponding to a slave on that bus.
         * @param busHandle
         * @param slaveAddress
         */
        I2cSlave(const handles::I2cBusHandle &busHandle, const i2cAddress &slaveAddress);

        /**
         * @warning Length of iterator should not be larger than the max value of @see I2cAddress.
         *
         * @tparam IteratorType type of iterator
         * @param begin begin of iterator
         * @param end end of iterator
         */
        template<typename IteratorType>
        void write(IteratorType begin, IteratorType end);

        /**
         * @brief Write single byte to slave in one message.
         * @param value byte to write
         * @return the same instance of @see I2cSlave
         */
        I2cSlave& operator<<(const char &value);

    private:
        std::string device;
        i2c::i2cAddress address;

        ssize_t write(const char *buffer, size_t length);
    };
}

#include "i2c_slave.tpp"
