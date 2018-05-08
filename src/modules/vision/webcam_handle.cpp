#include "webcam_handle.h"

using namespace goliath::handles;

webcam_handle::webcam_handle(const std::string &device)
        : device(device) {
}

const std::string& webcam_handle::get_device() const {
    return device;
}