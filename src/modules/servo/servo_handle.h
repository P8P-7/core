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
        ServoHandle(const size_t &id, std::shared_ptr<Dynamixel> device, std::function<void(bool)> txCallback);

        /**
         * @return Currently set device
         */
        std::shared_ptr<Dynamixel> getDevice();

    private:
        std::shared_ptr<Dynamixel> device;
    };
}
