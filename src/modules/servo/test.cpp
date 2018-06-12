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
    Dynamixel::Instruction instruction = Dynamixel::Instruction::Write;
    Dynamixel::Address address = Dynamixel::Address::MovingSpeed;
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
        } else if (!strcmp(argv[i], "--instruction")) {
            instruction = static_cast<Dynamixel::Instruction>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--address")) {
            address = static_cast<Dynamixel::Address>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--portName")) {
            portName = argv[++i];
        } else if (!strcmp(argv[i], "--data")) {
            iData = static_cast<short>(std::stoul(argv[++i]));
        }
    }

    data.push_back(static_cast<unsigned char>(address));

    if (numBytes == 1) {
        data.push_back(iData);
    } else if (numBytes == 2) {
        std::vector<unsigned char> hlData = Utils::convertToHL(iData);
        data.insert(data.end(), hlData.begin(), hlData.end());
    }

    auto port = std::make_shared<SerialPort>();
    BOOST_LOG_TRIVIAL(debug) << "Connecting to: " << portName << ":" << baudRate;

    GPIO gpio(GPIO::MapPin::GPIO18, GPIO::Direction::Out, GPIO::State::Low);
    if (port->connect(portName, baudRate)) {
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
        std::vector<unsigned char> buffer = motor.getInstructionPacket(instruction, data);

        std::string bufferStr;
        for (auto const &value: buffer) {
            bufferStr += (boost::format("0x%02X ") % static_cast<int>(value)).str();
        }

        BOOST_LOG_TRIVIAL(debug) << "Buffer: " << bufferStr;
        // End debugging

        std::vector<unsigned char> returnData = motor.send(instruction, data);

        // Remove the checksum
        returnData.pop_back();

        // Remove the first 5 elements, and shift everything else down by 5 indices.
        returnData.erase(returnData.begin(), returnData.begin() + 5);

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
