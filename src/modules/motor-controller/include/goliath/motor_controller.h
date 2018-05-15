#pragma once

#include <cstdint>
#include <goliath/i2c.h>
#include <vector>

namespace goliath::motor_controller {
    using motor_id = std::uint8_t;
    using motor_speed = std::uint8_t;

    /**
     * The physical motor controller has two pins for each motor to set in which direction it goes.
     * Each direction in this enum represents a state of a set of these pins.
     *
     * \warning Before the motor can change direction it has to be stalled. The motor controller does this for you, but this takes some time.
     */
    enum class motor_direction: std::uint8_t {
        /**
         * One of the pins is high, the other is low.
         */
        FORWARDS = 0,
        /**
         * Counterpart of FORWARDS
         */
        BACKWARDS = 1,
        /**
         * Both pins are low. Thus the motor is free to move.
         */
        FREE = 2,
        /**
         * Both pins are high, so the motor is locked at its current position.
         */
        LOCKED = 3
    };

    /**
     * Struct representing a current, or new motor status.
     */
    struct motor_status {
        motor_id id;
        motor_direction direction;
        motor_speed speed;
    };

    /**
     * Handles all interaction with the physical motor controller
     */
    class motor_controller {
    public:
        /**
         * @param slave the slave on the i2c bus representing the motor controller
         */
        motor_controller(i2c::i2c_slave &slave);

        /**
         * Sends the \param message to the physical controller to process.
         * @param message new motor status to process
         */
        void send_command(const motor_status &message);

        /**
         * Retrieves all motors from the motor controller, but only uses the first byte to determine the number of motors.
         * @return number of motors connected to the motor controller
         */
        size_t get_number_of_motors();

        /**
         * Retrieves all motors from the motor controller.
         * @return vector of all motors connected
         */
        std::vector<motor_status> get_motors();
    private:
        /**
         * Maximum supported of connected motors.
         */
        const static size_t max_number_of_motors = 4;
        /**
         * Size of allocated buffer before receiving the command.
         */
        const static size_t motor_buffer_size = 1 + sizeof(motor_status) * max_number_of_motors;

        i2c::i2c_slave& slave;
    };
}

