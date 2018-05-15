#include "device.h"

using namespace goliath::audio;
using namespace goliath::exceptions;

device::device(ALchar *devicename) {
    alc_device = alcOpenDevice(devicename);

    if(alc_device == NULL) {
        throw new openal_error("Unable to open device");
    }
}

device::~device() {
    alcCaptureCloseDevice(alc_device);
}

ALCdevice* device::get_device() const {
    return alc_device;
}
