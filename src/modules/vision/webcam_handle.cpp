#include "webcam_handle.h"

using namespace goliath::handles;
using namespace goliath::vision;

WebcamHandle::WebcamHandle(const size_t &id, const int &device)
        : Handle(id), device(device) {
}

const Webcam& WebcamHandle::getDevice() const {
    return device;
}
