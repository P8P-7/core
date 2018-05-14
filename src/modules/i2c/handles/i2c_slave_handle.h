#pragma once

#include <goliath/i2c.h>
#include "../../core/handle.h"

#include "../i2c_error.h"

namespace goliath::handles {
    class i2c_slave_handle: public handle {
    public:
        i2c_slave_handle(const i2c::i2c_address& address);

        /**
         * @return slave address
         */
        const i2c::i2c_address& get_slave_address() const;
    private:
        std::string device;
        i2c::i2c_address address;
    };


}