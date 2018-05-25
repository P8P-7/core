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
    std::vector<byte> recvData;

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

        // meeded for MovingSpeed
        motor.setCWAngleLimit(0);
        motor.setCCWAngleLimit(0);

        // for debugging only:
        byte buffer[1024];
        int length = motor.formatCommand(command,
                                         address,
                                         data,
                                         buffer);

        std::cout << "buffer: " <<
                  Utils::printBuffer(buffer, length) << std::endl;
        // end for debugging

        int retVal;
        retVal = motor.sendReceiveCommand(command,
                                          address,
                                          data,
                                          &recvData);

        int recvVal = 0;
        if (recvData.size() == 1) {
            recvVal = recvData[0];
        } else if (recvData.size() == 2) {
            recvVal = Utils::convertFromHL(recvData[0], recvData[1]);
        }
        std::cout << "received: " <<
                  retVal << " : " << recvVal << std::endl;

        std::cout << "position: " <<
                  motor.getPosition() << std::endl;
    } else {
        std::cout << "Couldn't open " <<
                  portName << " at baudRate " <<
                  baudRate << std::endl;
        return -1;
    }

    return 0;
}
