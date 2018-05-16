#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <thread>
#include <AL/al.h>
#include <AL/alc.h>

#include "device.h"
#include "util/openal_error.h"

/**
 * @file microphone.h
 * @author Group 7 - Informatica
 */

namespace goliath::openal {
    class microphone : public device {
    public:
        microphone(ALCuint frequency, ALCenum format, ALCsizei buffer_size, ALchar* devicename = NULL);

        /**
         * @brief Records audio from the microphone and returns it into the buffer.
         * @warning Make the capture buffer is large enough to fit the audio within the specified duration!
         * @param capture_buffer Buffer where the recorded audio is stored
         * @param duration How long the microphone will record
         * @param interval Duration between each of the sample recordings
         */
        void record(ALubyte *capture_buffer, std::chrono::microseconds duration, std::chrono::microseconds interval);

    private:
        ALubyte buffer;
    };
}
