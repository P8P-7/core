#include <iostream>
#include "dynamixel/Dynamixel.h"
#include <goliath/gpio.h>

using namespace goliath;

int main(int argc, char *argv[]) {
    byte motorId = 4;
    int numBytes = 2;
    short iData = 512;
    Dynamixel::Commands command = Dynamixel::Commands::Set;
    Dynamixel::Addresses address = Dynamixel::Addresses::MovingSpeed;
    std::string portName = "/dev/serial0";
    int baudRate = 1000000;

    std::vector<byte> data;

    // parse command line args
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--baudRate")) {
            baudRate = std::stoi(argv[++i]);
        } else if (!strcmp(argv[i], "--motorId")) {
            motorId = std::stoi(argv[++i]);
        } else if (!strcmp(argv[i], "--numBytes")) {
            numBytes = std::stoi(argv[++i]);
        } else if (!strcmp(argv[i], "--command")) {
            command = static_cast<Dynamixel::Commands>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--address")) {
            address = static_cast<Dynamixel::Addresses>(std::stoi(argv[++i]));
        } else if (!strcmp(argv[i], "--portName")) {
            portName = argv[++i];
        } else if (!strcmp(argv[i], "--data")) {
            iData = std::strtoul(argv[++i], 0, 10);
        }
    }

    if (numBytes == 1) {
        data.push_back(iData);
    } else if (numBytes == 2) {
        byte h, l;
        Utils::convertToHL(iData, &h, &l);
        data.push_back(l);
        data.push_back(h);
    }

    SerialPort port;
    std::cout << "Connecting to: " <<
              portName << ":" << baudRate << std::endl;

    gpio::GPIO gpio(gpio::GPIO::MapPin::GPIO18, gpio::GPIO::Direction::Out,
                    gpio::GPIO::State::Low);
    if (port.connect(portName, baudRate) != 0) {
        std::cout << "Success\n";
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            if (isTx) {
                gpio.set(gpio::GPIO::State::High);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(20));
                gpio.set(gpio::GPIO::State::Low);
            }
        };

        // configure the motor object
        Dynamixel motor(motorId, port);
        motor.setDirectionCallback(callback);

        // for debugging only:
        std::vector<byte> buffer = motor.getBuffer(command, address, data);

        std::cout << "Buffer:";
        for (auto const &value: buffer) {
            printf(" %02X", value);
        }
        std::cout << std::endl;
        // end for debugging

        std::vector<byte> returnData = motor.sendReceiveCommand(command, address, data);

        int recvVal = 0;
        if (returnData.size() == 1) {
            recvVal = returnData[0];
        } else if (returnData.size() == 2) {
            recvVal = Utils::convertFromHL(returnData[0], returnData[1]);
        }

        std::cout << "Received: " << recvVal << std::endl;
    } else {
        std::cout << "Couldn't open " << portName << " at baudRate " << baudRate << std::endl;
        return -1;
    }

    return 0;
}
