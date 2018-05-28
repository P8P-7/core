#include <goliath/led_controller.h>

using namespace goliath::controller;

LedController::LedController(i2c::I2cSlave &slave) : slave(slave) {}

void LedController::sendCommand(const SpecColMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedController::sendCommand(const AllLedsMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedController::sendCommand(const SpecRainMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

void LedController::sendCommand(const CircleMessage &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}
