#define BOOST_TEST_MODULE test_motor_controller

#include <boost/test/included/unit_test.hpp>
#include <goliath/i2c.h>
#include <goliath/motor_controller.h>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

    BOOST_AUTO_TEST_CASE(change_acceleration_decellaration) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::MotorController controller(slave);
        controller::MotorStatus message = {
                0,
                controller::MotorDirection::FORWARDS,
                0
        };

        for (controller::MotorSpeed i = 0;; i++) {
            BOOST_LOG_TRIVIAL(info) << "Speed: " << std::to_string(i);
            message.speed = i;
            controller.sendCommand(message);

            if (i == 255) {
                break;
            }
        }

        for (controller::MotorSpeed i = 254;; i--) {
            BOOST_LOG_TRIVIAL(info) << "Speed: " << std::to_string(i);
            message.speed = i;
            controller.sendCommand(message);

            if (i == 0) {
                break;
            }
        }

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE(switch_directions) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::MotorController controller(slave);
        controller::MotorStatus message = {
                0,
                controller::MotorDirection::FORWARDS,
                0
        };

        message.speed = 255 / 2;
        message.direction = controller::MotorDirection::FORWARDS;
        controller.sendCommand(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255;
        message.direction = controller::MotorDirection::FORWARDS;
        controller.sendCommand(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255;
        message.direction = controller::MotorDirection::BACKWARDS;
        controller.sendCommand(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255 / 2;
        message.direction = controller::MotorDirection::BACKWARDS;
        controller.sendCommand(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        BOOST_LOG_TRIVIAL(info) << "Stopping";
        message.speed = 0;
        message.direction = controller::MotorDirection::FORWARDS;
        controller.sendCommand(message);

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE(get_number_of_motors) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::MotorController controller(slave);
        size_t number_of_motors = controller.getNumberOfMotors();
        BOOST_CHECK_EQUAL(number_of_motors, 1);

        slave_handle.unlock();
        bus_handle.unlock();
    }

    BOOST_AUTO_TEST_CASE(get_motor_status) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::MotorController controller(slave);
        auto motors = controller.getMotors();
        for (auto motor : motors) {
            BOOST_LOG_TRIVIAL(debug) << "Received motor: " << std::to_string(motor.id)
                                     << ", direction: "  << std::to_string((int)motor.direction)
                                     << ", speed: " << std::to_string(motor.speed);
        }

        slave_handle.unlock();
        bus_handle.unlock();
    }

    BOOST_AUTO_TEST_CASE(stress_test) {
        const std::string device = "/dev/i2c-1";
        const i2c::I2cAddress address = 0x30;

        handles::I2cBusHandle bus_handle(1, device);
        handles::I2cSlaveHandle slave_handle(2, address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::I2cSlave slave(bus_handle, slave_handle);
        controller::MotorController controller(slave);
        controller::MotorStatus message = {
                0,
                controller::MotorDirection::FORWARDS,
                255
        };

        for (int i = 0; i < 1200; i++) {
            message.direction = controller::MotorDirection::FORWARDS;
            controller.sendCommand(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            message.direction = controller::MotorDirection::BACKWARDS;
            controller.sendCommand(message);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        BOOST_LOG_TRIVIAL(info) << "Stopping";
        message.speed = 0;
        message.direction = controller::MotorDirection::FORWARDS;
        controller.sendCommand(message);

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END()