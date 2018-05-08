#pragma once

#include "handles/i2c_slave_handle.h"
#include "handles/i2c_bus_handle.h"

namespace goliath::i2c {
    class i2c_slave {
    public:
        i2c_slave(const handles::i2c_bus_handle &bus_handle, const handles::i2c_slave_handle &slave_handle);
        i2c_slave(const handles::i2c_bus_handle &bus_handle, const std::uint8_t &slave_address);

        template<typename iterator_type>
        void write(iterator_type begin, iterator_type end);

        /**
         * Write single byte to slave in one message
         * @param value byte to write
         * @return the same instance of i2c_slave
         */
        i2c_slave& operator<<(const char &value);

    private:
        std::string device;
        std::uint8_t address;

        /**
         * Writes buffer of length to slave in one message
         *
         * Use this function to write multiple bytes at once to a slave.
         *
         * @param buffer buffer to write
         * @param length length of buffer
         */
        ssize_t write(const char *buffer, size_t length);
    };
}

#include "i2c_slave.tpp"
