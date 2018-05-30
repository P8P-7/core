#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <boost/format.hpp>

#include <goliath/i2c/i2c_slave.h>
#include <goliath/i2c/i2c_error.h>

using namespace goliath::i2c;

I2cSlave::I2cSlave(const handles::I2cBusHandle &busHandle, const i2c::I2cAddress &slaveAddress)
        : device(busHandle.getDevicePath()), address(slaveAddress) {
    file_descriptor = ::open(device.c_str(), O_RDWR);
    if (file_descriptor == -1) {
        throw exceptions::I2cError(device, "Couldn't open device");
    }
}

I2cSlave::I2cSlave(const handles::I2cBusHandle &busHandle, const handles::I2cSlaveHandle &slaveHandle)
        : I2cSlave(busHandle, slaveHandle.getSlaveAddress()) { }

I2cSlave::~I2cSlave() {
    if (file_descriptor != -1) {
        ::close(file_descriptor);
    }
}

void I2cSlave::setActiveSlaveOnBus(const i2c::I2cAddress &address) {
    int set_slave_result = ::ioctl(file_descriptor, I2C_SLAVE, address);
    if (set_slave_result == -1) {
        throw exceptions::I2cError(device, str(boost::format("Could not set slave address to %1$#x")
                                               % static_cast<int>(address)));
    }
}

ssize_t I2cSlave::write(const char *buffer, size_t length) {
    setActiveSlaveOnBus(address);

    ssize_t write_result = ::write(file_descriptor, buffer, length);
    if (write_result == -1) {
        throw exceptions::I2cError(device, str(boost::format("Could not write to slave %1$#x")
                                               % static_cast<int>(address)));
    }

    return write_result;
}

ssize_t I2cSlave::read(char *buffer, size_t length) {
    setActiveSlaveOnBus(address);

    ssize_t read_result = ::read(file_descriptor, buffer, length);
    if (read_result == -1) {
        throw exceptions::I2cError(device, str(boost::format("Could read from %1$#x") % static_cast<int>(address)));
    }

    return read_result;
}

I2cSlave &I2cSlave::operator<<(const char &value) {
    write(&value, 1);

    return *this;
}
