#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/log/trivial.hpp>
#include <goliath/gpio.h>

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
    this->export_gpio();
    this->setdir_gpio(dir);
    this->setval_gpio(val);
}

GPIO::GPIO(int gnum, int dir, int val) {
    this->gpionum = std::to_string(gnum);
    this->export_gpio();
    std::string _dir = "out";
    if (dir == 1) {
        _dir = "in";
    }
    this->setdir_gpio(_dir);
    this->setval_gpio(std::to_string(val));
}

void GPIO::setup(std::string gnum) {
    this->gpionum = gnum;
}

void GPIO::setup(int gnum) {
    this->gpionum = std::to_string(gnum);
}

void GPIO::setup(std::string gnum, std::string dir, std::string val) {
    this->gpionum = gnum;
    this->export_gpio();
    this->setdir_gpio(dir);
    this->setval_gpio(val);
}

void GPIO::setup(int gnum, int dir, int val) {
    this->gpionum = std::to_string(gnum);
    this->export_gpio();
    std::string _dir = "out";
    if (dir == 1) {
        _dir = "in";
    }
    this->setdir_gpio(_dir);
    this->setval_gpio(std::to_string(val));
}

int GPIO::export_gpio() {
    std::string export_str = "/sys/class/gpio/export";
    std::ofstream exportgpio(export_str.c_str());
    if (!exportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to export GPIO" << this->gpionum.c_str();
        return -1;
    }

    exportgpio << this->gpionum;
    exportgpio.close();
    return 0;
}

int GPIO::unexport_gpio() {
    std::string unexport_str = "/sys/class/gpio/unexport";
    std::ofstream unexportgpio(unexport_str.c_str());
    if (!unexportgpio.is_open()) {
        BOOST_LOG_TRIVIAL(warning) << "Unable to unexport GPIO" << this->gpionum.c_str();
        return -1;
    }

    unexportgpio << this->gpionum;
    unexportgpio.close();
    return 0;
}

int GPIO::close() {
    return this->unexport_gpio();
}

int GPIO::setdir_gpio(std::string dir) {
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

int GPIO::setdir_gpio(int dir) {
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

int GPIO::setval_gpio(std::string val) {
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

int GPIO::setval_gpio(int val) {
    this->setval_gpio(std::to_string(val));
    return 0;
}

int GPIO::set(int val) {
    this->setval_gpio(val);
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

int GPIO::getval_gpio(std::string &val) {
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

int GPIO::getval_gpio(int &val) {
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

std::string GPIO::get_gpionum() {
    return this->gpionum;
}

int GPIO::get_igpionum() {
    return atoi(this->gpionum.c_str());
}
