#define BOOST_TEST_MODULE test_wing_controller

#include <thread>
#include <boost/test/included/unit_test.hpp>
#include <goliath/gpio.h>
#include <goliath/servo.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    const gpio::GPIO::MapPin directionPin = gpio::GPIO::MapPin::GPIO18;
    const std::string portName = "/dev/serial0";
    const unsigned int baudRate = 1000000u;
    const size_t handleId = 1;

    BOOST_AUTO_TEST_CASE(test_state) {
        servo::WingState state(4);

        BOOST_CHECK_EQUAL(state.getNumberOfSectors(), 4);
        BOOST_CHECK_EQUAL(state.getRatio(), 0.25);

        BOOST_CHECK_EQUAL(state.hasLostTracking(), true);
        state.reset(0);
        BOOST_CHECK_EQUAL(state.hasLostTracking(), false);

        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(0), 0);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(45), 0);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(90), 1);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(125), 1);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(180), 2);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(225), 2);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(270), 3);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(315), 3);
        BOOST_CHECK_EQUAL(state.getSectorFromWorldAngle(360), 0);

        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(0), 0);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(45), 180);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(75), 300);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(90), 0);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(135), 180);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(165), 300);
        BOOST_CHECK_EQUAL(state.getAngleFromWorldAngle(180), 0);

        BOOST_CHECK_EQUAL(state.getDirectionFromWorldAngle(90) == servo::Direction::CLOCKWISE, true);
        BOOST_CHECK_EQUAL(state.getDirectionFromWorldAngle(135) == servo::Direction::CLOCKWISE, true);
        BOOST_CHECK_EQUAL(state.getDirectionFromWorldAngle(180) == servo::Direction::CLOCKWISE, true);
        BOOST_CHECK_EQUAL(state.getDirectionFromWorldAngle(270) == servo::Direction::COUNTER_CLOCKWISE, true);
        BOOST_CHECK_EQUAL(state.getDirectionFromWorldAngle(315) == servo::Direction::COUNTER_CLOCKWISE, true);
    }

    BOOST_AUTO_TEST_CASE(test_handle) {
        handles::WingHandle handle(0, nullptr, 4, false, false, 10.0);
        handles::WingHandle invertedHandle(0, nullptr, 4, true, true, 10.0);

        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(0), 0);
        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(75), 75);
        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(150), 150);
        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(225), 225);
        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(300), 300);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(0), 300);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(75), 225);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(150), 150);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(225), 75);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(300), 0);

        BOOST_CHECK_EQUAL(handle.getTranslatedWorldAngle(0), 0);
        BOOST_CHECK_EQUAL(handle.getTranslatedWorldAngle(90), 90);
        BOOST_CHECK_EQUAL(handle.getTranslatedWorldAngle(180), 180);
        BOOST_CHECK_EQUAL(handle.getTranslatedWorldAngle(270), 270);
        BOOST_CHECK_EQUAL(handle.getTranslatedWorldAngle(360), 360);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedWorldAngle(0), 360);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedWorldAngle(90), 270);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedWorldAngle(180), 180);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedWorldAngle(270), 90);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedWorldAngle(360), 0);

        BOOST_CHECK_EQUAL(handle.getTranslatedDirection(servo::Direction::CLOCKWISE) == servo::Direction::COUNTER_CLOCKWISE, true);
        BOOST_CHECK_EQUAL(handle.getTranslatedDirection(servo::Direction::COUNTER_CLOCKWISE) == servo::Direction::CLOCKWISE, true);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedDirection(servo::Direction::CLOCKWISE) == servo::Direction::CLOCKWISE, true);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedDirection(servo::Direction::COUNTER_CLOCKWISE) == servo::Direction::COUNTER_CLOCKWISE, true);
    }

    BOOST_AUTO_TEST_CASE(test_handle_and_state) {
        servo::WingState state(4);
        handles::WingHandle handle(0, nullptr, 4, false, false, 10.0);
        handles::WingHandle invertedHandle(0, nullptr, 4, true, true, 10.0);

        double targetAngle = 300;
        double worldAngle = handle.getTranslatedWorldAngle(targetAngle);
        BOOST_CHECK_EQUAL(worldAngle, 300);
        BOOST_CHECK_EQUAL(handle.getTranslatedAngle(state.getAngleFromWorldAngle(targetAngle)), 120);
        BOOST_CHECK_EQUAL(handle.getTranslatedSector(state.getSectorFromWorldAngle(targetAngle)), 3);

        worldAngle = invertedHandle.getTranslatedWorldAngle(targetAngle);
        BOOST_CHECK_EQUAL(worldAngle, 60);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedAngle(state.getAngleFromWorldAngle(targetAngle)), 180);
        BOOST_CHECK_EQUAL(invertedHandle.getTranslatedSector(state.getSectorFromWorldAngle(targetAngle)), 1);
    }

    BOOST_AUTO_TEST_CASE(pure_angle) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(20));
                gpio.set(gpio::GPIO::State::Low);
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(1, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle, 0, 512);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 300);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSpeed(), 512);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        controller.setAngle(servoHandle, 180, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 180);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSpeed(), 1023);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        controller.setAngle(servoHandle, 300, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 180);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSpeed(), 1023);
    }

    BOOST_AUTO_TEST_CASE(test_angle_register) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(1, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setSpeed(servoHandle, 0);
        controller.setMode(servoHandle, servo::ServoMode::WHEEL);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::WHEEL, true);

        controller.setSpeed(servoHandle, 1023, servo::Direction::CLOCKWISE);
        while (true) {
            BOOST_LOG_TRIVIAL(trace) << "Speed: " << std::to_string(device->getPresentSpeed()) << ", position: "
                                     << std::to_string(device->getPresentPosition());

        }
    }

    BOOST_AUTO_TEST_CASE(set_angle_left) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int rpiDirectionSwitchDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(rpiDirectionSwitchDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(1, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);
        controller.calibrate(servoHandle);

        controller.setAngle(servoHandle, 0, 1023, true);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 0);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSector(), 0);

        controller.setWorldAngle(servoHandle, 135, 1023, servo::Direction::CLOCKWISE);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 0);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSector(), 1);
        controller.calibrate(servoHandle);
    }

    BOOST_AUTO_TEST_CASE(set_angle_right) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);

        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int rpiDirectionSwitchDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(rpiDirectionSwitchDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(3, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, true, true, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);
        controller.calibrate(servoHandle);

        controller.setAngle(servoHandle, 0, 1023, true);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 0);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSector(), 0);

        controller.setWorldAngle(servoHandle, 135, 1023, servo::Direction::CLOCKWISE);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 0);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getSector(), 1);
        controller.calibrate(servoHandle);
    }

    BOOST_AUTO_TEST_CASE(set_angles_front) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device1 = std::make_shared<dynamixel::Dynamixel>(1, port);
        device1->setDirectionCallback(callback);
        auto device2 = std::make_shared<dynamixel::Dynamixel>(3, port);
        device2->setDirectionCallback(callback);
        handles::WingHandle servoHandle1(handleId, device1, 4, false, false, 10.0);
        handles::WingHandle servoHandle2(handleId + 1, device2, 4, true, true, 10.0);

        std::vector<size_t> handles = {servoHandle1.getId(), servoHandle2.getId()};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        const auto &state1 = repo->getState(servoHandle1.getId());
        const auto &state2 = repo->getState(servoHandle2.getId());

        controller.setMode(servoHandle1, servo::ServoMode::JOINT);
        controller.setMode(servoHandle2, servo::ServoMode::JOINT);
        BOOST_CHECK_EQUAL(state1.getMode() == servo::ServoMode::JOINT, true);
        BOOST_CHECK_EQUAL(state2.getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle1, 0, 1023, false);
        controller.setAngle(servoHandle2, 0, 1023, false);
        while (device1->isMoving() || device2->isMoving()) ;

        std::vector<servo::WingCommand> commands;

        commands = {{
                            {servoHandle1, 1023, 90, servo::Direction::CLOCKWISE},
                            {servoHandle2, 1023, 90, servo::Direction::CLOCKWISE}
                    }};
        controller.execute(commands);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        commands = {{
                            {servoHandle1, 1023, 180, servo::Direction::CLOCKWISE},
                            {servoHandle2, 1023, 180, servo::Direction::CLOCKWISE}
                    }};
        controller.execute(commands);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        commands = {{
                            {servoHandle1, 1023, 135, servo::Direction::COUNTER_CLOCKWISE},
                            {servoHandle2, 1023, 135, servo::Direction::COUNTER_CLOCKWISE}
                    }};
        controller.execute(commands);
        std::this_thread::sleep_for(std::chrono::seconds(5));

        commands = {{
                            {servoHandle1, 1023, 225, servo::Direction::CLOCKWISE},
                            {servoHandle2, 1023, 225, servo::Direction::CLOCKWISE}
                    }};
        controller.execute(commands);
    }

    BOOST_AUTO_TEST_CASE(set_angles_back) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device1 = std::make_shared<dynamixel::Dynamixel>(4, port);
        device1->setDirectionCallback(callback);
        auto device2 = std::make_shared<dynamixel::Dynamixel>(2, port);
        device2->setDirectionCallback(callback);
        handles::WingHandle servoHandle1(handleId, device1, 4, false, false, 10.0);
        handles::WingHandle servoHandle2(handleId + 1, device2, 4, true, true, 10.0);

        std::vector<size_t> handles = {servoHandle1.getId(), servoHandle2.getId()};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        const auto &state1 = repo->getState(servoHandle1.getId());
        const auto &state2 = repo->getState(servoHandle2.getId());

        controller.setMode(servoHandle1, servo::ServoMode::JOINT);
        controller.setMode(servoHandle2, servo::ServoMode::JOINT);
        BOOST_CHECK_EQUAL(state1.getMode() == servo::ServoMode::JOINT, true);
        BOOST_CHECK_EQUAL(state2.getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle1, 0, 1023, false);
        controller.setAngle(servoHandle2, 0, 1023, false);
        while (device1->isMoving() || device2->isMoving()) ;

        std::vector<servo::WingCommand> commands;

        commands = {{
                            {servoHandle1, 1023, 315, servo::Direction::CLOCKWISE},
                            {servoHandle2, 1023, 315, servo::Direction::CLOCKWISE}
                    }};
        controller.execute(commands);
    }

    BOOST_AUTO_TEST_CASE(set_left_front_center) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(1, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle, 40, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 40);
    }

    BOOST_AUTO_TEST_CASE(set_right_front_center) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(3, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle, 260, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 260);
    }

    BOOST_AUTO_TEST_CASE(set_left_back_center) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(2, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle, 260, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 260);
    }

    BOOST_AUTO_TEST_CASE(set_right_back_center) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            const int txDelayTime = 20;
            const int readDelay = 100;
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(txDelayTime));
                gpio.set(gpio::GPIO::State::Low);
                std::this_thread::sleep_for(std::chrono::microseconds(readDelay));
            }
        };

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(4, port);
        device->setDirectionCallback(callback);
        handles::WingHandle servoHandle(handleId, device, 4, false, false, 10.0);

        std::vector<size_t> handles = {handleId};
        auto repo = std::make_shared<repositories::WingStateRepository>(handles);
        servo::WingController controller(repo);

        controller.setMode(servoHandle, servo::ServoMode::JOINT);

        BOOST_CHECK_EQUAL(repo->getState(handleId).getMode() == servo::ServoMode::JOINT, true);

        controller.setAngle(servoHandle, 40, 1023);
        BOOST_CHECK_EQUAL(repo->getState(handleId).getAngle(), 40);
    }

    BOOST_AUTO_TEST_CASE(test_is_moving) {
        gpio::GPIO gpio(directionPin, gpio::GPIO::Direction::Out, gpio::GPIO::State::Low);

        auto port = std::make_shared<dynamixel::SerialPort>();
        bool connectSuccess = port->connect(portName, baudRate);
        BOOST_CHECK_EQUAL(connectSuccess, true);

        auto device = std::make_shared<dynamixel::Dynamixel>(4, port);
        device->setDirectionCallback([&gpio](bool setTx) {
            gpio.set(setTx ? gpio::GPIO::State::High : gpio::GPIO::State::Low);
        });

        while (true) {
            device->setGoalPosition(1023);
            while (device->isMoving()) {
                BOOST_LOG_TRIVIAL(trace) << "Yes";
            }
            BOOST_LOG_TRIVIAL(trace) << "No";

            device->setCWAngleLimit(0);
            device->setCCWAngleLimit(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            device->setCWAngleLimit(0);
            device->setCCWAngleLimit(0x3FF);

            device->setGoalPosition(0);
            while (device->isMoving()) {
                BOOST_LOG_TRIVIAL(trace) << "Yes";
            }
            BOOST_LOG_TRIVIAL(trace) << "No";
        }
    }

BOOST_AUTO_TEST_SUITE_END()
