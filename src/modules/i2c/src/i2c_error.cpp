#include <goliath/i2c/i2c_error.h>

#include <boost/format.hpp>

using namespace goliath::exceptions;

I2cError::I2cError(std::string device, std::string reason)
        : reason((boost::format("Device %1%: %2% (%3%)") % device % reason % errno).str()) {
}

const char *I2cError::what() const noexcept {
    return reason.c_str();
}
