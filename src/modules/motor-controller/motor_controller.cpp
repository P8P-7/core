#include <goliath/motor_controller.h>
#include <cstring>
#include <vector>
#include <boost/log/trivial.hpp>

using namespace goliath::controller;

MotorController::MotorController(i2c::I2cSlave &slave) : slave(slave) {}

void MotorController::sendCommand(const MotorStatus &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

size_t MotorController::getNumberOfMotors() {
    char buffer[1];
    ssize_t length = slave.read(buffer, 1);
    if (length < 1) {
        throw std::runtime_error("Failed to retrieve the number of motors");
    }

    return static_cast<size_t>(buffer[0]);
}

std::vector<MotorStatus> MotorController::getMotors() {
    char buffer[motorBufferSize];
    ssize_t length = slave.read(buffer, motorBufferSize);
    if (length < 1) {
        throw std::runtime_error("Failed to retrieve motors");
    }
    size_t number_of_motors = static_cast<size_t>(buffer[0]);
    BOOST_LOG_TRIVIAL(debug) << "Received " << std::to_string(length) << " bytes from motor controller. "
                             << "Controller has " << std::to_string(number_of_motors) << " motors.";

    std::vector<MotorStatus> result;
    for (int i = 0; i < number_of_motors; i++) {
        MotorStatus motor{};
        std::memcpy(&motor, buffer + 1 + i * sizeof(MotorStatus), sizeof(MotorStatus));
        result.emplace_back(motor);
    }

    return result;
}
