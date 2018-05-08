#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdio>
#include <unistd.h>
#include <boost/format.hpp>

#include "i2c_slave.h"

using namespace goliath::i2c;

i2c_slave::i2c_slave(const handles::i2c_bus_handle &bus_handle,
                     const std::uint8_t &slave_address) : device(bus_handle.get_device_path()),
                                                          address(slave_address) {}

i2c_slave::i2c_slave(const handles::i2c_bus_handle &bus_handle,
                     const handles::i2c_slave_handle &slave_handle) : i2c_slave(bus_handle,
                                                                                slave_handle.get_slave_address()) {}

ssize_t i2c_slave::write(const char *buffer, size_t length) {
    int file_descriptor = ::open(device.c_str(), O_RDWR);
    if (file_descriptor == -1) {
        throw exceptions::i2c_error(device, "Couldn't open device");
    }

    int set_slave_result = ::ioctl(file_descriptor, I2C_SLAVE, address);
    if (set_slave_result == -1) {
        throw exceptions::i2c_error(device, str(boost::format("Could not set slave address to %1$#x")
                                                % static_cast<int>(address)));
    }

    ssize_t write_result = ::write(file_descriptor, buffer, length);
    if (write_result == -1) {
        throw exceptions::i2c_error(device, str(boost::format("Could not write to %1$#x") % static_cast<int>(address)));
    }

    int close_result = ::close(file_descriptor);
    if (close_result == -1) {
        throw exceptions::i2c_error(device, "Failed to close device");
    }

    return write_result;
}

i2c_slave &i2c_slave::operator<<(const char &value) {
    write(&value, 1);

    return *this;
}
