#pragma once

#include <exception>
#include <string>

/**
 * @file processor.h
 * @author Group 7 - Informatica
 */

 /**
  * @namespace goliath::exceptions
  * @brief Contains all custom exceptions
  */

namespace goliath::exceptions {
    /**
     * @class goliath::exceptions::vision_error
     * @brief All errors related to the vision module
     */
    class vision_error : public std::exception {
    public:
        /**
         * @param reason Reason for exception
         */
        vision_error(const std::string reason);

        const char* what() const noexcept override;

    private:
        const std::string reason;
    };
}
