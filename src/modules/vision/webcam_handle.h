#pragma once

#include "../core/handle.h"

/**
 * @file webcam_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::webcam_handle
     * @brief Handle that locks webcams, passes the device to be used as VideoCapture
     */
    class webcam_handle: public handle {
    public:
        /**
         * @param device The recording device
         */
        explicit webcam_handle(const std::string& device);

        /**
         * @return Currently set device
         */
        const std::string& get_device() const;

    private:
        const std::string device;

    };
}