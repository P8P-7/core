#include <cerrno>
#include <boost/format.hpp>

#include "i2c_error.h"

using namespace goliath::exceptions;

i2c_error::i2c_error(std::string device, std::string reason) : reason(
        (boost::format("Device %1%: %2% (%3%)") % device % reason % errno).str()) {}

const char *i2c_error::what() const noexcept {
    return reason.c_str();
}
