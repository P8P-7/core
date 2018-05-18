#pragma once

#include <string>
#include <exception>

/**
 * @file i2c_error.h
 * @author Group 7 - Informatica
 */

namespace goliath::exceptions {
    class I2cError : public std::exception {
    public:
        /**
         * @param device i2c device for which the exception occurred
         * @param reason reason the exception occurred
         */
        I2cError(std::string device, std::string reason);

        /**
         * @see std::exception
         */
        const char* what() const noexcept override;
    private:
        const std::string reason;
    };
}
