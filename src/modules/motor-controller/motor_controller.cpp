#include <goliath/motor_controller.h>
#include <cstring>
#include <vector>
#include <boost/log/trivial.hpp>

using namespace goliath::motor_controller;

motor_controller::motor_controller(i2c::i2c_slave &slave) : slave(slave) {}

void motor_controller::send_command(const motor_status &message) {
    slave.write(reinterpret_cast<const char *>(&message), sizeof(message));
}

size_t motor_controller::get_number_of_motors() {
    char buffer[1];
    ssize_t length = slave.read(buffer, 1);
    if (length < 1) {
        throw std::runtime_error("Failed to retrieve the number of motors");
    }

    return static_cast<size_t>(buffer[0]);
}

std::vector<motor_status> motor_controller::get_motors() {
    char buffer[motor_buffer_size];
    ssize_t length = slave.read(buffer, motor_buffer_size);
    if (length < 1) {
        throw std::runtime_error("Failed to retrieve motors");
    }
    size_t number_of_motors = static_cast<size_t>(buffer[0]);
    BOOST_LOG_TRIVIAL(debug) << "Received " << std::to_string(length) << " bytes from motor controller. "
                             << "Controller has " << std::to_string(number_of_motors) << " motors.";

    std::vector<motor_status> result;
    for (int i = 0; i < number_of_motors; i++) {
        motor_status motor{};
        std::memcpy(&motor, buffer + 1 + i * sizeof(motor_status), sizeof(motor_status));
        result.emplace_back(motor);
    }

    return result;
}
