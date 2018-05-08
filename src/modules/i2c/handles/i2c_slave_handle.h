#pragma once

#include <goliath/handle.h>
#include "../i2c_error.h"

namespace goliath::handles {
    class i2c_slave_handle: public handle {
    public:
        i2c_slave_handle(const std::string& device, const std::uint8_t& address);

        const std::string& get_device_path() const;
        const std::uint8_t& get_slave_address() const;
    private:
        std::string device;
        std::uint8_t address;
    };


}