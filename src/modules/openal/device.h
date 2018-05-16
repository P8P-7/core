#pragma once

#include <iostream>
#include <memory>
#include <AL/al.h>
#include <AL/alc.h>

#include "util/openal_error.h"

/**
 * @file device.h
 * @author Group 7 - Informatica
 */

namespace goliath::openal {
    /**
     * @class goliath::openal::device
     * @brief Base class for any audio device
     */
    class device {
    public:
        /**
         * @param devicename Location of device (Default NULL)
         */
        device(ALchar *devicename = NULL);
        virtual ~device();

        ALCdevice* get_device() const;

    protected:
        ALCdevice *alc_device;
    };
}
