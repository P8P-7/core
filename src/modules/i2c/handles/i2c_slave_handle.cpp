#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <boost/log/trivial.hpp>
#include <goliath/i2c.h>

#include "../i2c_error.h"

using namespace goliath::handles;
using namespace goliath::i2c;

i2c_slave_handle::i2c_slave_handle(const i2c_address& address) : address(address) { }

const i2c_address& i2c_slave_handle::get_slave_address() const {
    return address;
}