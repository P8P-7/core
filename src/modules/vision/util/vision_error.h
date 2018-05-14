#pragma once

#include <exception>
#include <string>

namespace goliath::exceptions {
    class vision_error : public std::exception {
    public:
        vision_error(const std::string reason);

        const char* what() const noexcept override;

    private:
        const std::string reason;
    };
}
