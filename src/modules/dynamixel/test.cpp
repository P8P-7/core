#include <iostream>
#include "goliath/gpio.h"
#include "dynamixel/Dynamixel.h"

using namespace goliath::gpio;

int main(int argc, char *argv[]) {
    byte motorId = 4;
    int numBytes = 2;
    short iData = 512;
    std::string command = "Set";
    std::string address = "Goal";
    std::string portName = "/dev/serial0";
    int baudRate = 1000000;

    // motor object
    Dynamixel *motor;

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

    if (port.connect((char *) portName.c_str(), baudRate) != 0) {
        std::cout << "Success\n";

        // configure the motor object
        motor = new AX12(motorId, &port);

        motor->configure();
        motor->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });

        // For debugging only
        byte buffer[1024];
        int length = motor->formatCommand(motor->getCommand(command),
                                          motor->getAddress(address),
                                          data,
                                          buffer);

        std::cout << "buffer: " <<
                  Utils::printBuffer(buffer, length) << std::endl;
        // end of debugging

        int retVal;
        retVal = motor->sendReceiveCommand(command,
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
                  motor->getPosition() << std::endl;
    } else {
        std::cout << "Couldn't open " <<
                  portName << " at baudRate " <<
                  baudRate << std::endl;
        return -1;
    }

    return 0;
}