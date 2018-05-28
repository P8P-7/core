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

        controller::SpecColMessage specColMessage{
                {
                        controller::LightingType::SPECIFIC, controller::ColourType::HSV
                },
                {
                        0,                                  0, 255, 255
                }
        };
        controller::AllLedsMessage allLedsMessage{
                {
                        controller::LightingType::ALL, controller::ColourType::HSV
                },
                {
                        90,                            255, 255
                }
        };
        controller::SpecRainMessage specRainMessage{
                {
                        controller::LightingType::SPECIFIC, controller::ColourType::RAINBOW
                },
                {
                        0,                                  0
                }
        };
        controller::CircleMessage circleMessage{
                {
                        controller::LightingType::CIRCLE, controller::ColourType::HSV
                },
                {
                        0,                                4, true, 180, 255
                }
        };

        for (controller::LedId j = 0; j < 5; ++j) {
            specColMessage.specificColour.value = 255;
            specColMessage.specificColour.led_id = j;
            for (controller::Hue i = 0;; i++) {
                specColMessage.specificColour.hue = static_cast<controller::Hue>(i * 25);
                controller.sendCommand(specColMessage);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));

                if (i == 10) {
                    break;
                }
            }
            specRainMessage.specificRainbow.led_id = j;
            controller.sendCommand(specRainMessage);
            std::this_thread::sleep_for(std::chrono::seconds(2));

            specColMessage.specificColour.value = 0;
            controller.sendCommand(specColMessage);
        }

        controller.sendCommand(circleMessage);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        controller.sendCommand(allLedsMessage);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        allLedsMessage.allLeds.value = 0;
        controller.sendCommand(allLedsMessage);


        circleMessage.circle.end_id = 0;
        controller.sendCommand(circleMessage);

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);

    }

BOOST_AUTO_TEST_SUITE_END()
