#include "device.h"

using namespace goliath::openal;
using namespace goliath::exceptions;

Device::Device(ALchar *deviceName) {
    alcDevice = alcOpenDevice(deviceName);

    if(alcDevice == NULL) {
        throw openal_error("Unable to open device");
    }
}

Device::~Device() {
    alcCaptureCloseDevice(alcDevice);
}

ALCdevice* Device::getDevice() const {
    return alcDevice;
}
