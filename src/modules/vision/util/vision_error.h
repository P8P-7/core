#pragma once

#include <exception>
#include <string>

/**
 * @file vision_error.h
 * @author Group 7 - Informatica
 */

 /**
  * @namespace goliath::exceptions
  * @brief Contains all custom exceptions
  */

namespace goliath::exceptions {
    /**
     * @class goliath::exceptions::VisionError
     * @brief All errors related to the vision module
     */
    class VisionError : public std::exception {
    public:
        /**
         * @param reason Reason for exception
         */
        VisionError(const std::string reason);

        const char* what() const noexcept override;

    private:
        const std::string reason;
    };
}
