#pragma once

#include <chrono>
#include <AL/al.h>
#include <AL/alc.h>

#include "device.h"

/**
 * @file microphone.h
 * @author Group 7 - Informatica
 */

namespace goliath::openal {
    class Microphone : public Device {
    public:
        Microphone(ALCuint frequency, ALCenum format, ALCsizei bufferSize, ALchar* deviceName = nullptr);

        /**
         * @brief Records audio from the microphone and returns it into the buffer.
         * @warning Make the capture buffer is large enough to fit the audio within the specified duration!
         * @param captureBuffer Buffer where the recorded audio is stored
         * @param duration How long the microphone will record
         * @param interval Duration between each of the sample recordings
         */
        void record(ALubyte *captureBuffer, std::chrono::microseconds duration, std::chrono::microseconds interval);

    private:
        ALubyte buffer;
    };
}
