#include <goliath/led_controller.h>

using namespace goliath::controller;

LedController::LedController(i2c::I2cSlave &slave)
        : slave(slave) {
}

void LedController::sendCommand(const LedStatus &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}
