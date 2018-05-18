#pragma once

#include <goliath/foundation.h>
#include "dynamixel/Dynamixel.h"

/**
 * @file servo_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::servo_handle
     * @brief Handle that locks servos
     */
    class ServoHandle : public Handle {
    public:
        /**
         * @param device The dynamixel device.
         */
        explicit ServoHandle(const size_t &id, Dynamixel &device, std::function<void(bool)> txCallback);

        /**
         * @return Currently set device
         */
        Dynamixel &getDevice();

    private:
        Dynamixel device;
    };
}
