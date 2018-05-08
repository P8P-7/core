#pragma once

#include <goliath/i2c.h>

namespace goliath::i2c {
    class i2c_slave {
    public:
        /**
         * Initializes an instance with a handle to a bus and a handle to a slave on that bus.
         * @param bus_handle
         * @param slave_handle
         */
        i2c_slave(const handles::i2c_bus_handle &bus_handle, const handles::i2c_slave_handle &slave_handle);
        /**
         * Initializes an instance with a bus handle and an address corresponding to a slave on that bus.
         * @param bus_handle
         * @param slave_address
         */
        i2c_slave(const handles::i2c_bus_handle &bus_handle, const i2c_address &slave_address);

        /**
         * \warning Length of iterator should not be larger than the max value of \see i2c_address.
         *
         * @tparam iterator_type type of iterator
         * @param begin begin of iterator
         * @param end end of iterator
         */
        template<typename iterator_type>
        void write(iterator_type begin, iterator_type end);

        /**
         * Write single byte to slave in one message.
         * @param value byte to write
         * @return the same instance of i2c_slave
         */
        i2c_slave& operator<<(const char &value);

    private:
        std::string device;
        std::uint8_t address;

        ssize_t write(const char *buffer, size_t length);
    };
}

#include "i2c_slave.tpp"