#pragma once

#include <string>

/**
 * @file openal_error.h
 * @author Group 7 - Informatica
 */

namespace goliath::exceptions {
    /**
     * @class goliath::exceptions::OpenalError
     * @brief All errors related to the audio module
     */
    class OpenalError : public std::exception {
    public:
        /**
         * @param reason Reason for exception
         */
        OpenalError(const std::string reason);

        const char* what() const noexcept override;

    private:
        const std::string reason;
    };
}
