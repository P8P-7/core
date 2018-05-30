#define BOOST_TEST_MODULE test_motor_controller

#include <boost/test/included/unit_test.hpp>
#include <goliath/led-strip-controller.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

    BOOST_AUTO_TEST_CASE(test_led_control) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x40;
        handles::I2cBusHandle busHandle(1, device);
        handles::I2cSlaveHandle slaveHandle(2, address);
        busHandle.lock(999);
        slaveHandle.lock(999);
        i2c::I2cSlave slave(busHandle, slaveHandle);
        controller::LedStripController controller(slave);

        controller::SpecColMessage specColMessage{
            {controller::LightingType::SPECIFIC, controller::ColorType::HSV},
            {0, 0, 255, 0}
        };
        controller::AllLedsMessage allLedsMessage{
            {controller::LightingType::ALL, controller::ColorType::HSV},
            { 90, 255, 255}
        };
        controller::SpecRainMessage specRainMessage{
            {controller::LightingType::SPECIFIC, controller::ColorType::RAINBOW},
            {0, 0}
        };
        controller::CircleMessage circleMessage{
            {controller::LightingType::CIRCLE, controller::ColorType::HSV},
            {0, 4, false, 180, 255}
        };

        for (controller::LedId j = 0; j < controller::LedStripController::numberOfPixels; ++j) {
            specRainMessage.specificRainbow.ledId = j;
            specRainMessage.specificRainbow.startHue = static_cast<controller::Hue>(
                255 / controller::LedStripController::numberOfPixels * j);
            controller.sendCommand(specRainMessage);
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        controller.sendCommand(circleMessage);
        std::this_thread::sleep_for(std::chrono::seconds(4));

        for (controller::LedId j = 0; j < controller::LedStripController::numberOfPixels; ++j) {
            specRainMessage.specificRainbow.ledId = j;
            specRainMessage.specificRainbow.startHue = static_cast<controller::Hue>(0);
            controller.sendCommand(specRainMessage);
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
        for (controller::LedId j = 0; j < controller::LedStripController::numberOfPixels; ++j) {
            specColMessage.specificColour.ledId = j;
            controller.sendCommand(specColMessage);
        }

        for (controller::LedId j = 0; j < controller::LedStripController::numberOfPixels; ++j) {
            specColMessage.specificColour.value = 255;
            specColMessage.specificColour.ledId = j;
            for (controller::Hue i = 0;; i++) {
                specColMessage.specificColour.hue = static_cast<controller::Hue>(i * 25);
                controller.sendCommand(specColMessage);
                std::this_thread::sleep_for(std::chrono::milliseconds(250));

                if (i == 10) {
                    break;
                }
            }
            specRainMessage.specificRainbow.ledId = j;
            controller.sendCommand(specRainMessage);
            std::this_thread::sleep_for(std::chrono::seconds(3));

            specColMessage.specificColour.value = 0;
            controller.sendCommand(specColMessage);
        }

        controller.sendCommand(allLedsMessage);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        allLedsMessage.allLeds.value = 0;
        controller.sendCommand(allLedsMessage);


        circleMessage.circle.endId = 0;
        controller.sendCommand(circleMessage);

        slaveHandle.unlock();
        busHandle.unlock();

        BOOST_CHECK(true);

    }

BOOST_AUTO_TEST_SUITE_END()
