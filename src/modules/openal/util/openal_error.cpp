#include "openal_error.h"

using namespace goliath::exceptions;

OpenalError::OpenalError(const std::string reason)
    : reason(reason) {
}

const char* OpenalError::what() const noexcept {
    return reason.c_str();
}
