#pragma once

#include <goliath/foundation.h>
#include "gpio.h"

/**
 * @file gpio_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    /**
     * @class goliath::handles::gpio_handle
     * @brief Handle that locks GPIO pins
     */
    class GPIOHandle : public Handle {
    public:
        /**
         * @param device The GPIO device.
         */
        GPIOHandle(const size_t &id, std::shared_ptr<gpio::GPIO> device);

        /**
         * @return Currently set device
         */
        std::shared_ptr<gpio::GPIO> getDevice();

    private:
        std::shared_ptr<gpio::GPIO> device;
    };
}
