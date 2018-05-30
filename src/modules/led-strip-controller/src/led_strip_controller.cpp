#include <goliath/led-strip-controller/led_strip_controller.h>

using namespace goliath::controller;

LedStripController::LedStripController(i2c::I2cSlave &slave)
        : slave(slave) {
}

void LedStripController::sendCommand(const SpecColMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedStripController::sendCommand(const AllLedsMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedStripController::sendCommand(const SpecRainMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedStripController::sendCommand(const CircleMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}
