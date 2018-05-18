#include "microphone.h"

#include <thread>

#include "util/openal_error.h"

using namespace goliath::openal;
using namespace goliath::exceptions;

Microphone::Microphone(ALCuint frequency, ALCenum format, ALCsizei bufferSize, ALchar* deviceName) {
    alcDevice = alcCaptureOpenDevice(deviceName, frequency, format, bufferSize);

    if(alcDevice == NULL) {
        throw OpenalError("Unable to create microphone");
    }
}

void Microphone::record(ALubyte *capture_buffer, std::chrono::microseconds duration, std::chrono::microseconds interval) {
    auto start = std::chrono::high_resolution_clock::now();
    auto finish = start + duration;

    alcCaptureStart(alcDevice);

    while(std::chrono::high_resolution_clock::now() < finish) {
        ALint samplesAvailable;
        alcGetIntegerv(alcDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);

        if (samplesAvailable > 0) {
            alcCaptureSamples(alcDevice, capture_buffer, samplesAvailable);
            capture_buffer += samplesAvailable * 2;

            std::cout << samplesAvailable << std::endl;
        }

        std::this_thread::sleep_for(interval);
    }

    alcCaptureStop(alcDevice);
}
