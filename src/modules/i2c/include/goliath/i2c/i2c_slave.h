#pragma once

#include <string>

#include "handles/i2c_bus_handle.h"
#include "handles/i2c_slave_handle.h"
#include "i2c_error.h"

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
        I2cSlave(const handles::I2cBusHandle &busHandle, const I2cAddress &slaveAddress);

        ~I2cSlave();

        /**
         * @warning Length of iterator should not be larger than the max value of @see I2cAddress.
         *
         * @tparam IteratorType type of iterator
         * @param begin begin of iterator
         * @param end end of iterator
         */
        template<typename IteratorType>
        void write(IteratorType begin, IteratorType end) {
            auto tempLength = std::distance(begin, end) * sizeof(typename std::iterator_traits<IteratorType>::value_type);
            if (tempLength < std::numeric_limits<size_t>::min()
                || tempLength > std::numeric_limits<size_t>::max()) {
                throw goliath::exceptions::I2cError(device, "Buffer too large");
            }

            auto length = static_cast<size_t>(tempLength);
            write(reinterpret_cast<const char *>(&*begin), length);
        }

        /**
         * @brief Write single byte to slave in one message.
         * @param value byte to write
         * @return the same instance of @see I2cSlave
         */
        I2cSlave &operator<<(const char &value);

        /**
         * Writes anything at buffer with size length to the slave.
         * @param buffer pointer to start
         * @param length length of message
         * @return number of written bytes
         */
        ssize_t write(const char *buffer, size_t length);

        /**
         * Request the slave to send something.
         * @param buffer pointer to start writing
         * @param length size of buffer
         * @return total number of bytes read
         */
        ssize_t read(char *buffer, size_t length);

    private:
        std::string device;
        i2c::I2cAddress address;

        int file_descriptor;

        void setActiveSlaveOnBus(const i2c::I2cAddress &address);
    };
}
