#pragma once

#include <string>

/**
 * @file audio_error.h
 * @author Group 7 - Informatica
 */


namespace goliath::exceptions {
    /**
     * @class goliath::exceptions::audio_error
     * @brief All errors related to the audio module
     */
    class audio_error : public std::exception {
    public:
        /**
         * @param reason Reason for exception
         */
        audio_error(const std::string reason);

        const char* what() const noexcept override;

    private:
        const std::string reason;
    };
}
