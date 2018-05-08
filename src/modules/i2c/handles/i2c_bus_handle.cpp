//
// Created by obe on 8-5-18.
//

#include "i2c_bus_handle.h"

using namespace goliath::handles;

i2c_bus_handle::i2c_bus_handle(const std::string &device) : device(device) { }

const std::string &i2c_bus_handle::get_device_path() const {
    return device;
}
