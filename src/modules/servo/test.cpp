#include <iostream>
#include "dynamixel/Dynamixel.h"
#include <goliath/gpio.h>

using namespace goliath::gpio;

int main(int argc, char *argv[]) {
    byte motorId = 4;
    int numBytes = 2;
    short iData = 512;
    std::string command = "Set";
    std::string address = "Goal";
    std::string portName = "/dev/serial0";
    int baudRate = 1000000;

    // motor objects
    Dynamixel *motorManual;
    Dynamixel *motor1;
    Dynamixel *motor2;
    Dynamixel *motor3;
    Dynamixel *motor4;

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
        motorManual = new AX12(motorId, &port);
        motorManual->configure();
        motor1 = new AX12(1, &port);
        motor1->configure();
        motor2 = new AX12(2, &port);
        motor2->configure();
        motor3 = new AX12(3, &port);
        motor3->configure();
        motor4 = new AX12(4, &port);
        motor4->configure();

        motorManual->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });
        motor1->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });
        motor2->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });
        motor3->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });
        motor4->setDirectionCallback([&gpio](std::string direction) {
            if (direction == "tx") {
                gpio.set(HIGH);
            } else {
                usleep(20);
                gpio.set(LOW);
            }
        });


//        motorManual->setWheelMode(true);
//        motorManual->turn(1023,true);
//        motorManual->setWheelMode(false);
        motor4->setWheelMode(false);
//        motorManual->setGoalPosition(0);
        motor4->setGoalPosition(0);
        usleep(1000000);


        std::cout << "manual motor mode: " << motorManual->getCurrentMode() << std::endl;
//        std::cout << "motor" << 2 << " mode: " << motor2->getCurrentMode() << std::endl;
        std::cout << "motor" << 4 << " mode: " << motor4->getCurrentMode() << std::endl;

        motorManual->setWheelMode(true);
        motor4->setWheelMode(true);

        while (true) {
            motorManual->turn(1023, true);
            motor4->turn(128, true);
            usleep(1000000);
            motorManual->turn(0, true);
            motor4->turn(0, true);
            usleep(500000);
            std::cout << "motor manual:" << motorManual->getCurrentLoad() << std::endl;
            std::cout << "motor 4:" << motor4->getCurrentLoad() << std::endl;
            usleep(250000);
        }

        // For debugging only
        byte buffer[1024];
//        int length = motor->formatCommand(motor->getCommand(command),
//                                          motor->getAddress(address),
//                                          data,
//                                          buffer);

//        std::cout << "buffer: " <<
//                  Utils::printBuffer(buffer, length) << std::endl;
        // end of debugging

        int retVal;
        retVal = motorManual->sendReceiveCommand(command,
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
                  motorManual->getPosition() << std::endl;
    } else {
        std::cout << "Couldn't open " <<
                  portName << " at baudRate " <<
                  baudRate << std::endl;
        return -1;
    }

    return 0;
}