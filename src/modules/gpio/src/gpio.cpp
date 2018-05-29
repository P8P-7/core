#include <goliath/gpio/gpio.h>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/log/trivial.hpp>

using namespace goliath::gpio;

GPIO::GPIO() {
    gpionum = "";
}

GPIO::GPIO(MapPin gnum) {
    setup(gnum);
}

GPIO::GPIO(MapPin gnum, Direction dir, State val) {
    setup(gnum, dir, val);
}

void GPIO::setup(MapPin gnum) {
    gpionum = std::to_string(static_cast<int>(gnum));
}

void GPIO::setup(MapPin gnum, Direction dir, State val) {
    gpionum = std::to_string(static_cast<int>(gnum));
    exportGpio();
    setdirGpio(dir == Direction::Out ? "out" : "in");
    setvalGpio(std::to_string(static_cast<int>(val)));
}

int GPIO::exportGpio() {
    std::string exportStr = "/sys/class/gpio/export";
    std::ofstream exportgpio(exportStr.c_str());
    if (!exportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to export GPIO" << gpionum.c_str();
        return -1;
    }

    exportgpio << gpionum;
    exportgpio.close();
    return 0;
}

int GPIO::unexportGpio() {
    std::string unexportStr = "/sys/class/gpio/unexport";
    std::ofstream unexportgpio(unexportStr.c_str());
    if (!unexportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to unexport GPIO" << gpionum.c_str();
        return -1;
    }

    unexportgpio << gpionum;
    unexportgpio.close();
    return 0;
}

int GPIO::close() {
    return unexportGpio();
}

int GPIO::setdirGpio(std::string dir) {
    std::string setdir_str = "/sys/class/gpio/gpio" + gpionum + "/direction";
    std::ofstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set direction of GPIO" << gpionum.c_str();
        return -1;
    }
    setdirgpio << dir;
    setdirgpio.close();
    return 0;
}

int GPIO::setdirGpio(int dir) {
    std::string setdir_str = "/sys/class/gpio/gpio" + gpionum + "/direction";
    std::ofstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set direction of GPIO" << gpionum.c_str();
        return -1;
    }
    std::string _dir = "out";
    if (dir == 1) {
        _dir = "in";
    }
    setdirgpio << _dir;
    setdirgpio.close();
    return 0;
}

int GPIO::setvalGpio(std::string val) {
    std::string setval_str = "/sys/class/gpio/gpio" + gpionum + "/value";
    std::ofstream setvalgpio(setval_str.c_str());
    if (!setvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set value of GPIO" << gpionum.c_str();
        return -1;
    }
    setvalgpio << val;
    setvalgpio.close();
    return 0;
}

int GPIO::setvalGpio(int val) {
    setvalGpio(std::to_string(val));
    return 0;
}

int GPIO::set(State val) {
    setvalGpio(static_cast<int>(val));
    return 0;
}

int GPIO::get() {
    int val;
    std::string getval_str = "/sys/class/gpio/gpio" + gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << gpionum.c_str();
        return -1;
    }
    getvalgpio >> val;

    if (val != 0) {
        val = 1;
    } else {
        val = 0;
    }

    getvalgpio.close();
    return val;
}

int GPIO::getvalGpio(std::string &val) {
    std::string getval_str = "/sys/class/gpio/gpio" + gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << gpionum.c_str();
        return -1;
    }
    getvalgpio >> val;

    if (val != "0") {
        val = "1";
    } else {
        val = "0";
    }

    getvalgpio.close();
    return 0;
}

int GPIO::getvalGpio(int &val) {
    std::string getval_str = "/sys/class/gpio/gpio" + gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << gpionum.c_str();
        return -1;
    }
    std::string get_val;
    getvalgpio >> get_val;

    if (get_val != "0") {
        val = 1;
    } else {
        val = 0;
    }

    getvalgpio.close();
    return 0;
}

std::string GPIO::getGpionum() {
    return gpionum;
}

int GPIO::getiGpionum() {
    return atoi(gpionum.c_str());
}
