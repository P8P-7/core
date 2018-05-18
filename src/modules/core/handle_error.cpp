#include "handle_error.h"

using namespace goliath::exceptions;

const char* HandleError::what() const noexcept {
    return ("Handle " + std::to_string(handle_id) + ": " + reason).c_str();
}
