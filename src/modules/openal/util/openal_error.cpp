#include "openal_error.h"

using namespace goliath::exceptions;

openal_error::openal_error(const std::string reason)
    : reason(reason) {
}

const char* openal_error::what() const noexcept {
    return reason.c_str();
}
