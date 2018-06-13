#include <goliath/openal/device.h>
#include <goliath/openal/util/openal_error.h>

using namespace goliath::openal;
using namespace goliath::exceptions;

Device::Device(ALchar *deviceName) {
    alcDevice = alcOpenDevice(deviceName);

    if(alcDevice == nullptr) {
        throw OpenalError("Unable to open device");
    }
}

Device::~Device() {
    alcCaptureCloseDevice(alcDevice);
}

ALCdevice* Device::getDevice() const {
    return alcDevice;
}
