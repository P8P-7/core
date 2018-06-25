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
                            /*HANDLE_LEFT_FRONT_WING_SERVO, HANDLE_LEFT_BACK_WING_SERVO,
                            HANDLE_RIGHT_FRONT_WING_SERVO, HANDLE_RIGHT_BACK_WING_SERVO,*/
                            // I²C bus for led
                            /*HANDLE_I2C_BUS,*/
                            // Handle for the led controller
                            /*HANDLE_LED_CONTROLLER*/}) {
}

void commands::LineDanceCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has started";

    // Get GPIO device
    std::shared_ptr<gpio::GPIO> gpioDevice = handles.get<GPIOHandle>(HANDLE_GPIO_PIN_5)->getDevice();

    // Get led strip controller
    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    led_controller::LedStripController ledController(ledControllerSlave);

    led_controller::AllLedsMessage allLedsMessage{
            {led_controller::LightingType::ALL, led_controller::ColorType::HSV},
            {90, 255, 0}
    };

    t0 = std::chrono::high_resolution_clock::now();

    while (!isInterrupted()) {
        bool pulse = gpioDevice->get() != 1;

        if (pulse) {
            processPulse();

            /*allLedsMessage.allLeds.value = static_cast<led_controller::Value>(pulse ? 255 : 0);
            ledController.sendCommand(allLedsMessage);*/
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(pollingRate + (pulse ? 100 : 0)));
    }

    BOOST_LOG_TRIVIAL(info) << "Execution of line dance command has finished";
}


void commands::LineDanceCommand::processPulse() {
    // Beat amplitude trigger has been detected
    t1 = std::chrono::high_resolution_clock::now();
    history.push_back(60.0 / (std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() / 1000.0));

    double bpm = std::accumulate(history.begin(), history.end(), 0.0) / history.size();

    BOOST_LOG_TRIVIAL(debug) << "Pulse" << std::endl;

    t0 = t1;

    // Check to see that this tempo is within the limits allowed
    if (bpm >= minimumAllowedBpm && bpm <= maximumAllowedBpm) {
        runningBpm = bpm;
        BOOST_LOG_TRIVIAL(debug) << "Current BPM " << runningBpm << std::endl;
    } else {
        // Outside of bpm threshold, ignore
        history.pop_back();
    }

    if (history.size() >= 4) {
        history.erase(history.begin());
    }
}