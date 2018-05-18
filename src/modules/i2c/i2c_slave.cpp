#include "i2c_slave.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <boost/format.hpp>

#include "i2c_error.h"

using namespace goliath::i2c;

I2cSlave::I2cSlave(const handles::I2cBusHandle &busHandle, const i2c::i2cAddress &slaveAddress)
    : device(busHandle.getDevicePath()), address(slaveAddress) {
}

I2cSlave::I2cSlave(const handles::I2cBusHandle &busHandle, const handles::I2cSlaveHandle &slaveHandle)
    : I2cSlave(busHandle, slaveHandle.getSlaveAddress()) {
}

ssize_t I2cSlave::write(const char *buffer, size_t length) {
    int fileDescriptor = ::open(device.c_str(), O_RDWR);
    if (fileDescriptor == -1) {
        throw exceptions::I2cError(device, "Couldn't open device");
    }

    int setSlaveResult = ::ioctl(fileDescriptor, I2C_SLAVE, address);
    if (setSlaveResult == -1) {
        throw exceptions::I2cError(device, str(boost::format("Could not set slave address to %1$#x")
                                                % static_cast<int>(address)));
    }

    ssize_t writeResult = ::write(fileDescriptor, buffer, length);
    if (writeResult == -1) {
        throw exceptions::I2cError(device, str(boost::format("Could not write to %1$#x") % static_cast<int>(address)));
    }

    int closeResult = ::close(fileDescriptor);
    if (closeResult == -1) {
        throw exceptions::I2cError(device, "Failed to close device");
    }

    return writeResult;
}

I2cSlave &I2cSlave::operator<<(const char &value) {
    write(&value, 1);

    return *this;
}
