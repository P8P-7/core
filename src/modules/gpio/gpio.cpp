#include "gpio.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/log/trivial.hpp>

using namespace goliath::gpio;

GPIO::GPIO() {
    this->gpionum = "";
}

GPIO::GPIO(std::string gnum) {
    this->gpionum = gnum;
}

GPIO::GPIO(int gnum) {
    this->gpionum = std::to_string(gnum);
}

GPIO::GPIO(std::string gnum, std::string dir, std::string val) {
    this->gpionum = gnum;
    this->exportGpio();
    this->setdirGpio(dir);
    this->setvalGpio(val);
}

GPIO::GPIO(int gnum, int dir, int val) {
    this->gpionum = std::to_string(gnum);
    this->exportGpio();
    std::string _dir = "out";
    if (dir == 1) {
        _dir = "in";
    }
    this->setdirGpio(_dir);
    this->setvalGpio(std::to_string(val));
}

void GPIO::setup(std::string gnum) {
    this->gpionum = gnum;
}

void GPIO::setup(int gnum) {
    this->gpionum = std::to_string(gnum);
}

void GPIO::setup(std::string gnum, std::string dir, std::string val) {
    this->gpionum = gnum;
    this->exportGpio();
    this->setdirGpio(dir);
    this->setvalGpio(val);
}

void GPIO::setup(int gnum, int dir, int val) {
    this->gpionum = std::to_string(gnum);
    this->exportGpio();
    std::string _dir = "out";
    if (dir == 1) {
        _dir = "in";
    }
    this->setdirGpio(_dir);
    this->setvalGpio(std::to_string(val));
}

int GPIO::exportGpio() {
    std::string exportStr = "/sys/class/gpio/export";
    std::ofstream exportgpio(exportStr.c_str());
    if (!exportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to export GPIO" << this->gpionum.c_str();
        return -1;
    }

    exportgpio << this->gpionum;
    exportgpio.close();
    return 0;
}

int GPIO::unexportGpio() {
    std::string unexportStr = "/sys/class/gpio/unexport";
    std::ofstream unexportgpio(unexportStr.c_str());
    if (!unexportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to unexport GPIO" << this->gpionum.c_str();
        return -1;
    }

    unexportgpio << this->gpionum;
    unexportgpio.close();
    return 0;
}

int GPIO::close() {
    return this->unexportGpio();
}

int GPIO::setdirGpio(std::string dir) {
    std::string setdir_str = "/sys/class/gpio/gpio" + this->gpionum + "/direction";
    std::ofstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set direction of GPIO" << this->gpionum.c_str();
        return -1;
    }
    setdirgpio << dir;
    setdirgpio.close();
    return 0;
}

int GPIO::setdirGpio(int dir) {
    std::string setdir_str = "/sys/class/gpio/gpio" + this->gpionum + "/direction";
    std::ofstream setdirgpio(setdir_str.c_str());
    if (!setdirgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set direction of GPIO" << this->gpionum.c_str();
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
    std::string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ofstream setvalgpio(setval_str.c_str());
    if (!setvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to set value of GPIO" << this->gpionum.c_str();
        return -1;
    }
    setvalgpio << val;
    setvalgpio.close();
    return 0;
}

int GPIO::setvalGpio(int val) {
    this->setvalGpio(std::to_string(val));
    return 0;
}

int GPIO::set(int val) {
    this->setvalGpio(val);
    return 0;
}

int GPIO::get() {
    int val;
    std::string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << this->gpionum.c_str();
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

int GPIO::getvalGpio(std::string& val) {
    std::string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << this->gpionum.c_str();
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

int GPIO::getvalGpio(int& val) {
    std::string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    std::ifstream getvalgpio(getval_str.c_str());
    if (!getvalgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to get value of GPIO" << this->gpionum.c_str();
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
    return this->gpionum;
}

int GPIO::getiGpionum() {
    return atoi(this->gpionum.c_str());
}
