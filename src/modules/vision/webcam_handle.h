#pragma once

#include <goliath/handle.h>

namespace goliath::handles {
    class webcam_handle: public handle {
    public:
        explicit webcam_handle(const std::string& device);

        const std::string& get_device() const;
    private:
        const std::string device;
    };
}