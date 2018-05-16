#include "webcam_handle.h"

using namespace goliath::handles;

WebcamHandle::WebcamHandle(const int &device)
        : device(device) {
}

const int& WebcamHandle::getDevice() const {
    return device;
}
