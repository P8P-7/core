#include <goliath/gpio.h>
#include <goliath/motor-controller.h>
#include <goliath/controller/commands/line_dance_command.h>
#include <goliath/led-strip-controller/led_strip_controller.h>

using namespace goliath;
using namespace goliath::handles;
using namespace goliath::commands;

// In milliseconds
const int pollingRate = 15;

commands::LineDanceCommand::LineDanceCommand(const size_t &id)
        : BasicCommand(id, {HANDLE_GPIO_PIN_5, // GPIO Pin 5 (for VU-meter)
                            // ALl wings
                            HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO,
                            // I²C bus for led and motor
                            HANDLE_I2C_BUS,
                            // Handle for the motor controller
                            HANDLE_MOTOR_CONTROLLER,
                            // Handle for the led controller
                            HANDLE_LED_CONTROLLER,
                            // All motors
                            HANDLE_LEFT_FRONT_MOTOR, HANDLE_LEFT_BACK_MOTOR,
                            HANDLE_RIGHT_FRONT_MOTOR, HANDLE_RIGHT_BACK_MOTOR}) {
}

void commands::LineDanceCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has started";

    // Get GPIO device
    std::shared_ptr<gpio::GPIO> gpioDevice = std::static_pointer_cast<GPIOHandle>(
            handles[HANDLE_GPIO_PIN_5])->getDevice();

    // Get motor controller
    i2c::I2cSlave motorControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                       *handles.get<handles::I2cSlaveHandle>(HANDLE_MOTOR_CONTROLLER));
    motor_controller::MotorController motorController(motorControllerSlave);

    // Get led strip controller
    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    led_controller::LedStripController ledController(ledControllerSlave);

    led_controller::SpecColMessage specColMessage{
            {led_controller::LightingType::SPECIFIC, led_controller::ColorType::HSV},
            {0, 0, 255, 0}
    };

    bool lastPulse = false;

    while (!isInterrupted()) {
        bool pulse = gpioDevice->get() != 0;

        if (pulse != lastPulse) {
            // Turn lights on/off for a pulse.
            for (led_controller::LedId j = 0; j < led_controller::LedStripController::numberOfPixels; ++j) {
                specColMessage.specificColour.value = static_cast<led_controller::Value>(pulse ? 255 : 0);
                specColMessage.specificColour.ledId = j;
                ledController.sendCommand(specColMessage);
            }

            lastPulse = pulse;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(pollingRate));
    }

    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has finished";
}
