#pragma once

#include <goliath/foundation.h>

#include "webcam.h"

/**
 * @file webcam_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::WebcamHandle
     * @brief Handle that locks webcams, passes the device to be used as VideoCapture
     */
    class WebcamHandle : public Handle {
    public:
        /**
         * @param id Handle ID
         * @param device The recording device
         */
        explicit WebcamHandle(const size_t &id, const int &device);

        /**
         * @return Currently set device
         */
        const vision::Webcam& getDevice() const;

    private:
        const vision::Webcam device;

    };
}
