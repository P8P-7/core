#include "vision_error.h"

using namespace goliath::exceptions;

vision_error::vision_error(const std::string reason)
        : reason(reason) {
}

const char* vision_error::what() const noexcept {
    return reason.c_str();
}
