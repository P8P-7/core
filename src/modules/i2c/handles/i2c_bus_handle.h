#pragma once

#include <string>
#include <goliath/handle.h>

namespace goliath::handles {
    class i2c_bus_handle: public handle {
    public:
        i2c_bus_handle(const std::string& device);

        /**
         * @return path to i2c device
         */
        const std::string& get_device_path() const;
    private:
        std::string device;
    };
}

