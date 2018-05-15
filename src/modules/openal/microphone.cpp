#include "microphone.h"

using namespace goliath::audio;
using namespace goliath::exceptions;

microphone::microphone(ALCuint frequency, ALCenum format, ALCsizei buffer_size, ALchar* devicename) {
    alc_device = alcCaptureOpenDevice(devicename, frequency, format, buffer_size);

    if(alc_device == NULL) {
        throw openal_error("Unable to create microphone");
    }
}

void microphone::record(ALubyte *capture_buffer, std::chrono::microseconds duration, std::chrono::microseconds interval) {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start + duration;

    alcCaptureStart(alc_device);

    while(std::chrono::high_resolution_clock::now() < finish) {
        ALint samples_available;
        alcGetIntegerv(alc_device, ALC_CAPTURE_SAMPLES, 1, &samples_available);

        if (samples_available > 0) {
            alcCaptureSamples(alc_device, capture_buffer, samples_available);
            capture_buffer += samples_available * 2;

            std::cout << samples_available << std::endl;
        }

        std::this_thread::sleep_for(interval);
    }

    alcCaptureStop(alc_device);
}
