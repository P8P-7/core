#pragma once

#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>

/**
 * @file device.h
 * @author Group 7 - Informatica
 */

namespace goliath::openal {
    /**
     * @class goliath::openal::Device
     * @brief Base class for any audio device
     */
    class Device {
    public:
        /**
         * @param deviceName Location of device (Default NULL)
         */
        Device(ALchar *deviceName = NULL);
        virtual ~Device();

        ALCdevice* getDevice() const;

    protected:
        ALCdevice *alcDevice;
    };
}
