#include "webcam_handle.h"

using namespace goliath::handles;
using namespace goliath::vision;

WebcamHandle::WebcamHandle(const int &device)
        : device(device) {
}

const Webcam& WebcamHandle::getDevice() const {
    return device;
}
