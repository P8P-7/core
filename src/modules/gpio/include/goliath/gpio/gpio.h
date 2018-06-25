#pragma once

#include <string>

/**
 * @file gpio/gpio.h
 * @author Group 7 - Informatica
 */

namespace goliath::gpio {
    class GPIO {
    public:
        enum class MapPin {
            GPIO2 = 2,
            GPIO3 = 3,
            GPIO4 = 4,
            GPIO5 = 5,
            GPIO6 = 6,
            GPIO7 = 7,
            GPIO8 = 8,
            GPIO9 = 9,
            GPIO10 = 10,
            GPIO11 = 11,
            GPIO12 = 12,
            GPIO13 = 13,
            GPIO14 = 14,
            GPIO15 = 15,
            GPIO16 = 16,
            GPIO17 = 17,
            GPIO18 = 18,
            GPIO19 = 19,
            GPIO20 = 20,
            GPIO21 = 21,
            GPIO22 = 22,
            GPIO23 = 23,
            GPIO24 = 24,
            GPIO25 = 25,
            GPIO26 = 26,
            GPIO27 = 27,
        };

        enum class State {
            Low = 0,
            High = 1,
        };

        enum class Direction {
            Out = 0,
            In = 1
        };

        GPIO();

        explicit GPIO(MapPin x);

        GPIO(MapPin x, Direction dir);

        GPIO(MapPin x, Direction dir, State val);

        void setup(MapPin x);

        void setup(MapPin x, Direction dir);

        void setup(MapPin x, Direction dir, State val);

        int exportGpio();

        int unexportGpio();

        int close();

        int setdirGpio(std::string dir);

        int setdirGpio(int dir);

        int setvalGpio(std::string val);

        int setvalGpio(int val);

        int set(State val);

        int getvalGpio(std::string &val);

        int getvalGpio(int &val);

        int get();

        std::string getGpionum();

        int getiGpionum();

    private:
        std::string gpionum;
    };
}
