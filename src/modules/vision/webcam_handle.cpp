#include "webcam_handle.h"

using namespace goliath::handles;

webcam_handle::webcam_handle(const size_t &id, const int &device)
        : handle(id), device(device) {
}

const int& webcam_handle::get_device() const {
    return device;
}
