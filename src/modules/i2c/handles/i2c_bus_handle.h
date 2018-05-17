#pragma once

#include <string>
#include <goliath/core.h>

namespace goliath::handles {
    class I2cBusHandle : public Handle {
    public:
        I2cBusHandle(const std::string& device);

        /**
         * @return path to i2c device
         */
        const std::string& getDevicePath() const;
    private:
        std::string device;
    };
}

