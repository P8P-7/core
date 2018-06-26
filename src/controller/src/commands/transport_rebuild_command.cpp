#include <goliath/controller/commands/transport_rebuild_command.h>

#include <goliath/led-strip-controller.h>
#include <goliath/emotions.h>

using namespace goliath;

commands::TransportRebuildCommand::TransportRebuildCommand(const size_t &id)
    : BasicCommand(id, {HANDLE_I2C_BUS, HANDLE_LED_CONTROLLER, HANDLE_EMOTIONS}) { }

void commands::TransportRebuildCommand::execute(handles::HandleMap &handles, const proto::CommandMessage &message) {
    led_controller::CircleMessage helmetMessage{
        .ledStatus = {
            .lightingType = led_controller::LightingType::CIRCLE,
            .colorType = led_controller::ColorType::HSV
        },
        .circle = {
            .startId = 0,
            .endId = 0, // TODO: Set ID's correctly
            .cw = true,
            .hue = 40,
            .saturation = 100
        }
    };

    i2c::I2cSlave ledControllerSlave(*handles.get<handles::I2cBusHandle>(HANDLE_I2C_BUS),
                                     *handles.get<handles::I2cSlaveHandle>(HANDLE_LED_CONTROLLER));
    led_controller::LedStripController ledController(ledControllerSlave);

    ledController.sendCommand(helmetMessage);

    waitForInterrupt();

    ledController.sendCommand(led_controller::AllLedsMessage{
        .ledStatus = {
            .lightingType = led_controller::LightingType::ALL,
            .colorType = led_controller::ColorType::HSV
        },
        .allLeds = {
            .hue = 0,
            .saturation = 0,
            .value = 0,
            .rainbow = false
        }
    });
}
