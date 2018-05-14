#include "webcam_handle.h"

using namespace goliath::handles;

webcam_handle::webcam_handle(const int &device)
        : device(device) {
}

const int& webcam_handle::get_device() const {
    return device;
}
