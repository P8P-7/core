#pragma once

#include <exception>
#include <string>

namespace goliath::exceptions {
    class HandleError: public std::exception {
    public:
        HandleError(const size_t &handle_id, const std::string &reason) : handle_id(handle_id), reason(reason) { }

        const char* what() const noexcept override;
    private:
        const size_t handle_id;
        const std::string reason;
    };
}

