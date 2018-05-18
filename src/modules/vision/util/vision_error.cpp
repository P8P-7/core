#include "vision_error.h"

using namespace goliath::exceptions;

VisionError::VisionError(const std::string reason)
        : reason(reason) {
}

const char* VisionError::what() const noexcept {
    return reason.c_str();
}
