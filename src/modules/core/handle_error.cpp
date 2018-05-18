#include "handle_error.h"

using namespace goliath::exceptions;

const char* HandleError::what() const noexcept {
    return whatReason.c_str();
}
