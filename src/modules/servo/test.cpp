#include <iostream>
#include <boost/format.hpp>

#include <goliath/gpio.h>
#include "dynamixel/Dynamixel.h"

using namespace goliath::gpio;
using namespace goliath::dynamixel;

int main(int argc, char *argv[]) {
    unsigned char motorId = 4;
    int numBytes = 2;
    short iData = 512;
    Dynamixel::Commands command = Dynamixel::Commands::Set;
    Dynamixel::Addresses address = Dynamixel::Addresses::MovingSpeed;
    std::string portName = "/dev/serial0";
    unsigned int baudRate = 1000000;

    std::vector<unsigned char> data;

    // Parse command line args
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--baudRate")) {
            baudRate = static_cast<unsigned int>(std::stoul(argv[++i]));
        } else if (!strcmp(argv[i], "--motorId")) {
            motorId = static_cast<unsigned char>(std::stoul(argv[++i]));
        } else if (!strcmp(argv[i], "--numBytes")) {
            numBytes = std::stoi(argv[++i]);
        } else if (!strcmp(argv[i], "--command")) {
            command = static_cast<Dynamixel::Commands>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--address")) {
            address = static_cast<Dynamixel::Addresses>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--portName")) {
            portName = argv[++i];
        } else if (!strcmp(argv[i], "--data")) {
            iData = static_cast<short>(std::stoul(argv[++i]));
        }
    }

    if (numBytes == 1) {
        data.push_back(iData);
    } else if (numBytes == 2) {
        data = Utils::convertToHL(iData);
    }

    SerialPort port;
    BOOST_LOG_TRIVIAL(debug) <<  "Connecting to: " << portName << ":" << baudRate;

    GPIO gpio(GPIO::MapPin::GPIO18, GPIO::Direction::Out, GPIO::State::Low);
    if (port.connect(portName, baudRate)) {
        BOOST_LOG_TRIVIAL(debug) << "Connected successfully";

        std::function<void(bool)> callback = [&gpio](bool isTx) {
            if (isTx) {
                gpio.set(GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(20));
                gpio.set(GPIO::State::Low);
            }
        };

        // Configure the motor object
        Dynamixel motor(motorId, port);
        motor.setDirectionCallback(callback);

        // Debugging only
        std::vector<unsigned char> buffer = motor.getBuffer(command, address, data);

        std::string bufferStr;
        for (auto const &value: buffer) {
            bufferStr += (boost::format("0x%02X ") % static_cast<int>(value)).str();
        }

        BOOST_LOG_TRIVIAL(debug) << "Buffer: " << bufferStr;
        // End debugging

        std::vector<unsigned char> returnData = motor.sendReceiveCommand(command, address, data);

        int recvVal = 0;
        if (returnData.size() == 1) {
            recvVal = returnData[0];
        } else if (returnData.size() == 2) {
            recvVal = Utils::convertFromHL(returnData[0], returnData[1]);
        }

        BOOST_LOG_TRIVIAL(debug) << "Received: " << recvVal;
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Couldn't open " << portName << " at baudRate " << baudRate;
        return -1;
    }

    return 0;
}
