#pragma once

#include <goliath/core.h>

/**
 * @file webcam_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::WebcamHandle
     * @brief Handle that locks webcams, passes the device to be used as VideoCapture
     */
    class WebcamHandle : public handle {
    public:
        /**
         * @param device The recording device
         */
        explicit WebcamHandle(const int& device);

        /**
         * @return Currently set device
         */
        const int& getDevice() const;

    private:
        const int device;

    };
}
