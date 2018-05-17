#include "microphone.h"

#include <thread>

#include "util/openal_error.h"

using namespace goliath::openal;
using namespace goliath::exceptions;

Microphone::Microphone(ALCuint frequency, ALCenum format, ALCsizei bufferSize, ALchar* deviceName) {
    alcDevice = alcCaptureOpenDevice(deviceName, frequency, format, bufferSize);

    if(alcDevice == NULL) {
        throw openal_error("Unable to create microphone");
    }
}

void Microphone::record(ALubyte *capture_buffer, std::chrono::microseconds duration, std::chrono::microseconds interval) {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start + duration;

    alcCaptureStart(alcDevice);

    while(std::chrono::high_resolution_clock::now() < finish) {
        ALint samples_available;
        alcGetIntegerv(alcDevice, ALC_CAPTURE_SAMPLES, 1, &samples_available);

        if (samples_available > 0) {
            alcCaptureSamples(alcDevice, capture_buffer, samples_available);
            capture_buffer += samples_available * 2;

            std::cout << samples_available << std::endl;
        }

        std::this_thread::sleep_for(interval);
    }

    alcCaptureStop(alcDevice);
}
