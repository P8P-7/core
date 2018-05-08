#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <boost/log/trivial.hpp>
#include "i2c_slave_handle.h"

#include "../i2c_error.h"

using namespace goliath::handles;

i2c_slave_handle::i2c_slave_handle(const std::string& device, const std::uint8_t& address) : device(device),
                                                                                             address(address) { }

const std::string& i2c_slave_handle::get_device_path() const {
    return device;
}

const std::uint8_t& i2c_slave_handle::get_slave_address() const {
    return address;
}