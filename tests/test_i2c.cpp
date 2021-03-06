#define BOOST_TEST_MODULE test_i2c

#include <boost/test/included/unit_test.hpp>
#include <goliath/i2c.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_write_bytes) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;
        const int number_of_messages = 10;

        handles::I2cBusHandle busHandle(1, device);
        busHandle.lock(999);

        i2c::I2cSlave slave(busHandle, address);
        for (int i = 0; i < number_of_messages; i++) {
            slave << i;
        }

        busHandle.unlock();

        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE(test_read_bytes) {
        const std::string device = "/dev/i2c-1";
        const std::uint8_t address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        bus_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, address);
        char buffer[2];
        ssize_t result = slave.read(buffer, 2);
        BOOST_CHECK_EQUAL(result, 2);
        BOOST_CHECK_EQUAL(buffer[0], 'H');
        BOOST_CHECK_EQUAL(buffer[1], 'A');

        bus_handle.unlock();
    }
BOOST_AUTO_TEST_SUITE_END()
