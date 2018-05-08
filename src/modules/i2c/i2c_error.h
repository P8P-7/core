#pragma once

#include <string>
#include <exception>

namespace goliath::exceptions {
    class i2c_error: public std::exception {
    public:
        i2c_error(std::string device, std::string reason);

        const char* what() const noexcept override;
    private:
        const std::string reason;
    };
}

