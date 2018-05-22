#include "audio_error.h"

using namespace goliath::exceptions;

audio_error::audio_error(const std::string reason)
    : reason(reason) {
}

const char* audio_error::what() const noexcept {
    return reason.c_str();
}
