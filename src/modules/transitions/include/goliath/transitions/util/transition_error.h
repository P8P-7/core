#pragma once

#include <exception>
#include <string>

/**
 * @file transition_error.h
 * @author Group 7 - Informatica
 */

namespace goliath::exceptions {
    /**
     * @class goliath::exceptions::TransitionError
     * @brief All errors related to the vision module
     */
    class TransitionError : public std::exception {
    public:
        /**
         * @param reason Reason for exception
         */
        explicit TransitionError(const std::string &reason);

        const char *what() const noexcept override;

    private:
        const std::string reason;
    };
}
