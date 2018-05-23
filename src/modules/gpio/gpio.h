#pragma once

#include <string>

/**
 * @file gpio/gpio.h
 * @author Group 7 - Informatica
 */


enum GPIO_MAP_PIN {
    GPIO2 = 2, GPIO14 = 14,
    GPIO3 = 3, GPIO15 = 15,
    GPIO4 = 4, GPIO18 = 18,
    GPIO17 = 17, GPIO23 = 23,
    GPIO27 = 27, GPIO24 = 24,
    GPIO22 = 22, GPIO25 = 25,
    GPIO10 = 10, GPIO8 = 8,
    GPIO9 = 9, GPIO7 = 7,
    GPIO11 = 11, GPIO12 = 12,
    GPIO5 = 5, GPIO16 = 16,
    GPIO6 = 6, GPIO20 = 20,
    GPIO13 = 13, GPIO21 = 21,
    GPIO19 = 19,
    GPIO26 = 26,
};

enum INTERNAL_STATE {
    LOW = 0,
    HIGH = 1,
    OUT = 0,
    IN = 1
};

namespace goliath::gpio {
    class GPIO {
    public:
        GPIO();

        explicit GPIO(std::string x);
        explicit GPIO(int x);
        GPIO(std::string x, std::string dir, std::string val);
        GPIO(int x, int dir, int val);

        void setup(std::string x);
        void setup(int x);
        void setup(std::string x, std::string dir, std::string val);
        void setup(int x, int dir, int val);

        int exportGpio();

        int unexportGpio();

        int close();

        int setdirGpio(std::string dir);
        int setdirGpio(int dir);

        int setvalGpio(std::string val);
        int setvalGpio(int val);

        int set(int val);

        int getvalGpio(std::string& val);
        int getvalGpio(int& val);

        int get();

        std::string getGpionum();
        int getiGpionum();

    private:
        std::string gpionum;
    };
}