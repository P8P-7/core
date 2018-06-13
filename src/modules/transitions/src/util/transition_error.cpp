#include <goliath/transitions/util/transition_error.h>

using namespace goliath::exceptions;

TransitionError::TransitionError(const std::string &reason)
        : reason(reason) { }

const char *TransitionError::what() const noexcept {
    return reason.c_str();
}
