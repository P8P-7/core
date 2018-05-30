#pragma once

#include <cstdint>
#include <goliath/i2c.h>
#include <vector>

namespace goliath::motor_controller {
    using MotorId = std::uint8_t;
    using MotorSpeed = std::uint8_t;

    /**
     * The physical motor controller has two pins for each motor to set in which direction it goes.
     * Each direction in this enum represents a state of a set of these pins.
     *
     * \warning Before the motor can change direction it has to be stalled. The motor controller does this for you, but this takes some time.
     */
    enum class MotorDirection: std::uint8_t {
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
    struct MotorStatus {
        MotorId id;
        MotorDirection direction;
        MotorSpeed speed;
    };

    /**
     * Handles all interaction with the physical motor controller
     */
    class MotorController {
    public:
        /**
         * @param slave the slave on the i2c bus representing the motor controller
         */
        MotorController(i2c::I2cSlave &slave);

        /**
         * Sends a message to the physical controller to process.
         * @param command new motor status to process
         */
        void sendCommand(const MotorStatus &command);
        template<typename IteratorType>
        void sendCommands(const IteratorType &begin, const IteratorType &end);

        /**
         * Retrieves all motors from the motor controller, but only uses the first byte to determine the number of motors.
         * @return number of motors connected to the motor controller
         */
        size_t getNumberOfMotors();

        /**
         * Retrieves all motors from the motor controller.
         * @return vector of all motors connected
         */
        std::vector<MotorStatus> getMotors();
    private:
        /**
         * Maximum supported of connected motors.
         */
        const static size_t maxNumberOfMotors = 4;
        /**
         * Size of allocated buffer before receiving the command.
         */
        const static size_t motorBufferSize = 1 + sizeof(MotorStatus) * maxNumberOfMotors;

        i2c::I2cSlave& slave;
    };

    template<typename IteratorType>
    void MotorController::sendCommands(const IteratorType &begin, const IteratorType &end) {
        slave.write(begin, end);
    }
}

