#define BOOST_TEST_MODULE test_motor_controller

#include <boost/test/included/unit_test.hpp>
#include <goliath/i2c.h>
#include <goliath/led_controller.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

    BOOST_AUTO_TEST_CASE(test_led_control) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x40;
        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);
        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::LedController controller(slave);
        controller::LedStatus message = {
                1,
                0,
                255,
                255,
        };
        for (controller::LedId j = 0; j < 5; ++j) {
            message.value = 255;
            message.led_id = j;
            for (controller::Hue i = 0;; i++) {
                message.hue = i;
                controller.sendCommand(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(4));

                if (i == 255) {
                    break;
                }
            }
            for (controller::Hue i = 254;; i--) {
                message.hue = i;
                controller.sendCommand(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(4));

                if (i == 0) {
                    break;
                }
            }

            message.value = 0;

            controller.sendCommand(message);
        }

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);

    }

BOOST_AUTO_TEST_SUITE_END()
