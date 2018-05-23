#include <iostream>
#include "dynamixel/Dynamixel.h"
#include <goliath/gpio.h>

using namespace goliath::gpio;

int main(int argc, char *argv[]) {
    byte motorId = 4;
    int numBytes = 2;
    short iData = 512;
    std::string command = "Set";
    std::string address = "MovingSpeed";
    std::string portName = "/dev/serial0";
    int baudRate = 1000000;

    std::vector<byte> data;
    std::vector<byte> recvData;

    // parse command line args
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--baudRate")) {
            baudRate = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--motorId")) {
            motorId = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--numBytes")) {
            numBytes = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--command")) {
            command = argv[++i];
        } else if (!strcmp(argv[i], "--address")) {
            address = argv[++i];
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

    GPIO gpio;
    gpio.setup(GPIO18, OUT, LOW);

    if (port.connect(portName, baudRate) != 0) {
        std::cout << "Success\n";
        std::function<void(bool)> callback = [&gpio](bool isTx) {
            if (isTx) {
                gpio.set(HIGH);
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(20));
                gpio.set(LOW);
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
        int length = motor.formatCommand(motor.getCommand(command),
                                         motor.getAddress(address),
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
