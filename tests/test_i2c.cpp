#define BOOST_TEST_MODULE test_i2c

#include <boost/test/included/unit_test.hpp>
#include "../src/modules/i2c/handles/i2c_slave_handle.h"
#include "../src/modules/i2c/handles/i2c_bus_handle.h"
#include "../src/modules/i2c/i2c_slave.h"

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_write_bytes) {
        const std::string device = "/dev/i2c-1";
        const std::array<std::uint8_t, 2> addresses = {{ 0x30, 0x40 }};
        const std::array<char, 2> buffer {{ 0x69, 0x42 }};

        handles::i2c_bus_handle bus_handle(device);
        bus_handle.lock(999);

        for (auto address : addresses) {
            i2c::i2c_slave slave(bus_handle, address);

            slave << buffer[0] << buffer[1];
            slave.write(buffer.begin(), buffer.end());
        }

        bus_handle.unlock();

        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END()
