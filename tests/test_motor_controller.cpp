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
        const i2c::i2c_address address = 0x30;

        handles::i2c_bus_handle bus_handle(device);
        handles::i2c_slave_handle slave_handle(address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::i2c_slave slave(bus_handle, slave_handle);
        motor_controller::motor_controller controller(slave);
        motor_controller::motor_status message = {
                0,
                motor_controller::motor_direction::FORWARDS,
                0
        };

        for (motor_controller::motor_speed i = 0;; i++) {
            BOOST_LOG_TRIVIAL(info) << "Speed: " << std::to_string(i);
            message.speed = i;
            controller.send_command(message);

            if (i == 255) {
                break;
            }
        }

        for (motor_controller::motor_speed i = 254;; i--) {
            BOOST_LOG_TRIVIAL(info) << "Speed: " << std::to_string(i);
            message.speed = i;
            controller.send_command(message);

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
        const i2c::i2c_address address = 0x30;

        handles::i2c_bus_handle bus_handle(device);
        handles::i2c_slave_handle slave_handle(address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::i2c_slave slave(bus_handle, slave_handle);
        motor_controller::motor_controller controller(slave);
        motor_controller::motor_status message = {
                0,
                motor_controller::motor_direction::FORWARDS,
                0
        };

        message.speed = 255 / 2;
        message.direction = motor_controller::motor_direction::FORWARDS;
        controller.send_command(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255;
        message.direction = motor_controller::motor_direction::FORWARDS;
        controller.send_command(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255;
        message.direction = motor_controller::motor_direction::BACKWARDS;
        controller.send_command(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        message.speed = 255 / 2;
        message.direction = motor_controller::motor_direction::BACKWARDS;
        controller.send_command(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        BOOST_LOG_TRIVIAL(info) << "Stopping";
        message.speed = 0;
        message.direction = motor_controller::motor_direction::FORWARDS;
        controller.send_command(message);

        slave_handle.unlock();
        bus_handle.unlock();

        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE(get_number_of_motors) {
        const std::string device = "/dev/i2c-1";
        const i2c::i2c_address address = 0x30;

        handles::i2c_bus_handle bus_handle(device);
        handles::i2c_slave_handle slave_handle(address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::i2c_slave slave(bus_handle, slave_handle);
        motor_controller::motor_controller controller(slave);
        size_t number_of_motors = controller.get_number_of_motors();
        BOOST_CHECK_EQUAL(number_of_motors, 1);

        slave_handle.unlock();
        bus_handle.unlock();
    }

    BOOST_AUTO_TEST_CASE(get_motor_status) {
        const std::string device = "/dev/i2c-1";
        const i2c::i2c_address address = 0x30;

        handles::i2c_bus_handle bus_handle(device);
        handles::i2c_slave_handle slave_handle(address);
        bus_handle.lock(999);
        slave_handle.lock(999);

        i2c::i2c_slave slave(bus_handle, slave_handle);
        motor_controller::motor_controller controller(slave);
        auto motors = controller.get_motors();
        for (auto motor : motors) {
            BOOST_LOG_TRIVIAL(debug) << "Received motor: " << std::to_string(motor.id)
                                     << ", direction: "  << std::to_string((int)motor.direction)
                                     << ", speed: " << std::to_string(motor.speed);
        }

        slave_handle.unlock();
        bus_handle.unlock();
    }

BOOST_AUTO_TEST_SUITE_END()
